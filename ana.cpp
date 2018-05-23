/**
  * Copyright 2019 Cisco
  * 
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * 
  *    http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

//config.h from libbfd
#include <config.h>

//Our own imports
#include "log.hpp"
#include "ana.hpp"
#include "reg.hpp"
#include "ins.hpp"

//libbfd imports
#include <bfd.h>
#include <dis-asm.h>

//stdlib imports
#include <map>
#include <sstream>
#include <regex>

//IDA Pro imports
#include <bytes.hpp>


enum BfdOperandType
{
    OP_REG,
    OP_IMM,
    OP_MEM
};

struct BfdOperand
{
    std::string op;
    BfdOperandType type;
    int64_t value;

    BfdOperand(std::string op, BfdOperandType type, uint64_t value = 0) : op(op), type(type), value(value) {}
};

struct BfdInstruction
{
    std::string mnem;
    std::vector< BfdOperand > ops;

    bool is_return_inst() const {
        return mnem == "jrp";
    }

    bool is_call_inst() const {
        return mnem == "jal";
    }

    bool is_branch_inst() const {
        return mnem.compare(0, 1, "b") == 0;
    }

    bool is_invalid() const {
        return mnem == "";
    }
};

std::map< ea_t, std::vector< BfdInstruction > > bfd_instructions;

struct BfdDisassembler
{
    disassembler_ftype disfn;
    disassemble_info info;
    std::string text;

    BfdDisassembler(const char* arch) 
    {
        bfd  abfd= {0};
        const bfd_arch_info_type* arch_info = bfd_scan_arch(arch);
         //Allowed values are "tilegx" and "tilegx32"
        if (!arch_info) {
            msg("Unsupported architecture!\n");
            exit(1);
        }
        bfd_set_arch_info(&abfd, arch_info);
        disfn = disassembler(bfd_get_arch(&abfd), !inf_is_be(), bfd_get_mach(&abfd), nullptr);

        memset(&info,0,sizeof(info));

        info.flavour = bfd_target_elf_flavour;
        info.arch = bfd_get_arch(&abfd);
        info.mach = bfd_get_mach(&abfd);
        info.octets_per_byte = 1;

        info.endian_code = inf_is_be() ? BFD_ENDIAN_BIG : BFD_ENDIAN_LITTLE;
        info.bytes_per_chunk= 4;
        info.display_endian= BFD_ENDIAN_LITTLE;
      

        info.read_memory_func       = read_memory;
        info.memory_error_func      = perror;
        info.print_address_func     = print_address;
        info.symbol_at_address_func = symbol_at_address;
        info.symbol_is_valid        = symbol_is_valid;
        info.application_data        = this;

        info.fprintf_func = &BfdDisassembler::printf;
        info.stream = this;

        disassemble_init_for_target(&info);   
    }


    std::string disassemble(ea_t ea)
    {
        this->text.clear();
        disfn(ea, &info);
        return this->text;
    }

private:
    __attribute__((__format__ (__printf__, 2, 0)))
    static int printf(void* self, const char*fmt, ...)
    {
        char buf[256];

        va_list va;
        va_start(va, fmt);
        int rc = vsnprintf(buf, sizeof(buf), fmt, va);
        va_end(va);

        static_cast< BfdDisassembler* >(self)->text.append(buf);

        return rc;
    }

    static int read_memory(bfd_vma memaddr, uint8_t *myaddr, unsigned int len, struct disassemble_info *info)
    {
        if (!get_bytes(myaddr, len, static_cast<ea_t>(memaddr)))
            return 1;
        else
            return 0;
    }

    static void perror(int status, bfd_vma memaddr, struct disassemble_info *info)
    {
        error("tilegx: error(%d, %08lx)\n", status, memaddr);
    }

    // defined after tilegx_disasm
    static void print_address(bfd_vma addr, struct disassemble_info *info)
    {
        std::stringstream ss;
        ss << std::hex << addr;
        static_cast< BfdDisassembler* >(info->application_data)->text += "0x" + ss.str();
    }

    static int symbol_at_address(bfd_vma addr, struct disassemble_info *info)
    {
        log("tilegx: symataddr(%08lx)\n", addr);
        return 0;
    }

    static bfd_boolean symbol_is_valid(asymbol *sym, struct disassemble_info *info)
    {
        log("tilegx: valid(%08lx, '%s', 0x%x)\n", sym->value, sym->name, sym->flags);
        return true;
    }
};

static BfdDisassembler disasm("tilegx");

static int64_t to_int64(const std::string& str, bool is_hex = false)
{
    std::stringstream is;
    int64_t val;

    if (is_hex) {
        is << std::hex << str;
    }
    else {
        is << str;
    }
    is >> val;
    return val;
}

static std::map< std::string, int > buildRegToIdx()
{
    std::map< std::string, int > map;

    for (size_t i = 0; i < NUM_REGISTER_NAMES; ++i) {
        map.insert(std::make_pair(std::string(REGISTER_NAMES[i]), i));
    }

    return map;
}

static const std::map< std::string, int > regToIdx = buildRegToIdx();

static std::map< std::string, uint16_t > buildMnemonicToIndex()
{
    std::map< std::string, uint16_t > map;

    for (size_t idx = 0; idx < NUM_INSTRUCTIONS; ++idx) {
        map.insert(std::make_pair(std::string(INSTRUCTIONS[idx].name), static_cast<uint16_t>(idx)));
    }

    return map;
}

static const std::map< std::string, uint16_t >  mnemonicToIdx = buildMnemonicToIndex();

bool parse_instruction_packet(ea_t ea, const std::string& line)
{
    std::regex r_packet("^\\{ (.*) \\}$");
    std::smatch m_packet;
    if (!std::regex_match(line, m_packet, r_packet)) {
        msg("Can't parse line '%s' at ea 0x%llx\n", line.c_str(), ea);
        return false;
    }

    assert(m_packet.size() == 2);

    std::vector< BfdInstruction >& insts = bfd_instructions[ea & ~7];

    if (line.find("<invalid>") != std::string::npos) {
        insts.push_back(BfdInstruction{});
        return false;
    }
    
    for (size_t inst_start = m_packet.position(1), inst_end = line.find(";");
         inst_start < m_packet.position(1) + m_packet.length(1);) 
    {
        if (inst_end == std::string::npos) {
            inst_end = m_packet.position(1) + m_packet.length(1);
        }

        std::regex r_inst("^\\s*([a-z][a-z0-9_]*)\\s*(.*)$");
        std::smatch m_inst;
        std::string str_inst = line.substr(inst_start, inst_end - inst_start);

        if (!std::regex_match(str_inst, m_inst, r_inst)) {
            msg("Can't match instruction '%s' at ea 0x%llx\n", str_inst.c_str(), ea);
            continue;
        }

        assert(m_inst.size() == 3);

        insts.push_back(BfdInstruction {m_inst[1].str()});
        BfdInstruction& inst = insts.back();
        const std::string& str_ops = m_inst[2].str();
        for (size_t op_start = 0, op_end = str_ops.find(","); op_start < str_ops.size(); ) {
            std::regex r_imm("^\\s*(-?[0-9]+)\\s*$");
            std::regex r_mem("^\\s*0x([0-9a-f]+)\\s*$");
            std::regex r_strip("^\\s*([^\\s]+)\\s*$");
            std::smatch m_match;
            std::string str_op = str_ops.substr(op_start, op_end - op_start);

            if (op_end == std::string::npos) {
                op_end = str_ops.size();
            }

            if (std::regex_match(str_op, m_match, r_imm)) {
                inst.ops.push_back(BfdOperand(m_match[1].str(), OP_IMM, to_int64(m_match[1].str())));
            }
            else if (std::regex_match(str_op, m_match, r_mem)) {
                inst.ops.push_back(BfdOperand(m_match[1].str(), OP_MEM, to_int64(m_match[1].str(), true)));
            }
            else if (std::regex_match(str_op, m_match, r_strip)) {
                if (regToIdx.find(m_match[1].str()) != regToIdx.end()) {
                    inst.ops.push_back(BfdOperand(m_match[1].str(), OP_REG));
                }
                else {
                    msg("Can't parse operand '%s' in inst '%s' packet '%s' at ea 0x%" FMT_EA "x\n", 
                        m_match[1].str().c_str(), 
                        str_inst.c_str(),
                        line.c_str(), 
                        ea);
                    break;
                }
            }
            else {
                msg("Operand is empty in inst '%s' packet '%s' at ea 0x%" FMT_EA "x\n",
                    str_inst.c_str(),
                    line.c_str(),
                    ea);
            }
            op_start = op_end + 1;
            op_end = str_ops.find(",", op_start);
        }

        inst_start = inst_end + 1;
        inst_end = line.find(";", inst_start);
    }

    return false;
}

ssize_t tilegx_ana_insn(insn_t* cmd)
{
    static int packetflags = 0;
    static ea_t last_ea = -1;

    log("ana(%08" FMT_EA "x)\n", cmd->ea);

    //If this is a new instruction packet, reset packet flags
    if (last_ea != cmd->ea & ~7) {
        packetflags = 0;
    }

    auto itr = bfd_instructions.find(cmd->ea & ~7);
    if (itr == bfd_instructions.end()) {
        std::string text = disasm.disassemble(cmd->ea & ~7);
        parse_instruction_packet(cmd->ea, text);
        itr = bfd_instructions.find(cmd->ea & ~7);
    }

    int idx = cmd->ea & 7;

    BfdInstruction& inst = itr->second[idx];
    if (inst.is_invalid()) {
        cmd->itype = 0;
        return 0;
    }

    bool last_in_packet;
    if (idx == itr->second.size() - 1) {
        cmd->size = 8 - idx;
        last_in_packet = true;
    }
    else {
        cmd->size = 1;
        last_in_packet = false;
    }

    auto m_itr = mnemonicToIdx.find(inst.mnem);
    if (m_itr != mnemonicToIdx.end()) {
        cmd->itype = m_itr->second;
    }
    else {
        cmd->itype = 0;
        msg("Don't know how to handle mnemonic %s\n", inst.mnem.c_str());
    }

    const instruc_t& ida_inst = INSTRUCTIONS[cmd->itype];
    if (ida_inst.feature & CF_STOP) {
        packetflags |= CF_STOP;
    }
    if (ida_inst.feature & CF_CALL) {
        packetflags |= CF_CALL;
    }
    if (ida_inst.feature & CF_JUMP) {
        packetflags |= CF_JUMP;
    }


    // translate operands

    op_t *op= cmd->ops;
    op_t *opend = cmd->ops+6;

    for (const BfdOperand& asm_op : inst.ops) {
        switch (asm_op.type) {
            case OP_REG:
            {
                op->type = o_reg;
                auto itr_reg = regToIdx.find(asm_op.op);
                if (itr_reg != regToIdx.end()) {
                    op->reg = itr_reg->second;
                }
                else {
                    op->reg = 0;
                    msg("ERROR: Unknown register %s in ea 0x%" FMT_EA "x\n", asm_op.op.c_str(), cmd->ea);
                }
                break;
            }
            case OP_MEM:
                if (inst.is_branch_inst() || inst.is_call_inst()) {
                    op->type = o_near;
                    op->addr = asm_op.value;
                    op->dtype = dt_code;
                }
                else {
                    op->type = o_mem;
                    op->addr = asm_op.value;
                    op->dtype = dt_code;
                }
                break;
            case OP_IMM:
                op->type = o_imm;
                op->value = asm_op.value;
                op->dtype = dt_dword;
                break;
        }

        ++op;
    }

    last_ea = cmd->ea & ~7;
    return cmd->size;
}

ssize_t tilegx_is_call_insn(const insn_t* insn)
{
    return insn->itype == mnemonicToIdx.at("jal");
}

ssize_t tilegx_is_ret_insn(const insn_t* insn, bool strict)
{
    return insn->itype == mnemonicToIdx.at("jrp") ||
           insn->itype == mnemonicToIdx.at("jr");
}

