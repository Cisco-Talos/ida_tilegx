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

#include <functional>

#include <idp.hpp>
#include "reg.hpp"
#include "ins.hpp"
#include "ana.hpp"
#include "emu.hpp"
#include "out.hpp"
#include "log.hpp"


#define PLFM_TILEGX 0x8369

template<typename ReturnType> ReturnType invoke_variadic(ReturnType(*fnc)(), va_list va)
{
    return fnc();
}

template<typename ReturnType, typename TA1> ReturnType invoke_variadic(ReturnType(*fnc)(TA1), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    return fnc(arg1);
}

template<typename ReturnType, typename TA1, typename TA2> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    return fnc(arg1, arg2);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    return fnc(arg1, arg2, arg3);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3, typename TA4> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3, TA4), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    TA4 arg4 = va_arg(va, TA4);
    return fnc(arg1, arg2, arg3, arg4);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3, typename TA4, typename TA5> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3, TA4, TA5), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    TA4 arg4 = va_arg(va, TA4);
    TA5 arg5 = va_arg(va, TA5);
    return fnc(arg1, arg2, arg3, arg4, arg5);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3, typename TA4, typename TA5, typename TA6> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3, TA4, TA5, TA6), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    TA4 arg4 = va_arg(va, TA4);
    TA5 arg5 = va_arg(va, TA5);
    TA6 arg6 = va_arg(va, TA6);
    return fnc(arg1, arg2, arg3, arg4, arg5, arg6);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3, typename TA4, typename TA5, typename TA6, typename TA7> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3, TA4, TA5, TA6, TA7), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    TA4 arg4 = va_arg(va, TA4);
    TA5 arg5 = va_arg(va, TA5);
    TA6 arg6 = va_arg(va, TA6);
    TA7 arg7 = va_arg(va, TA7);
    return fnc(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3, typename TA4, typename TA5, typename TA6, typename TA7, typename TA8> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3, TA4, TA5, TA6, TA7, TA8), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    TA4 arg4 = va_arg(va, TA4);
    TA5 arg5 = va_arg(va, TA5);
    TA6 arg6 = va_arg(va, TA6);
    TA7 arg7 = va_arg(va, TA7);
    TA8 arg8 = va_arg(va, TA8);
    return fnc(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template<typename ReturnType, typename TA1, typename TA2, typename TA3, typename TA4, typename TA5, typename TA6, typename TA7, typename TA8, typename TA9> ReturnType invoke_variadic(ReturnType(*fnc)(TA1, TA2, TA3, TA4, TA5, TA6, TA7, TA8, TA9), va_list va)
{
    TA1 arg1 = va_arg(va, TA1);
    TA2 arg2 = va_arg(va, TA2);
    TA3 arg3 = va_arg(va, TA3);
    TA4 arg4 = va_arg(va, TA4);
    TA5 arg5 = va_arg(va, TA5);
    TA6 arg6 = va_arg(va, TA6);
    TA7 arg7 = va_arg(va, TA7);
    TA8 arg8 = va_arg(va, TA8);
    TA8 arg9 = va_arg(va, TA9);
    return fnc(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}
/************************************************************************/
/* Short names of processor                                             */
/************************************************************************/
static const char *const SHORT_PROCESSOR_NAMES[] = { "tilegx", NULL };

/************************************************************************/
/* Long names of processor                                              */
/************************************************************************/
static const char *const LONG_PROCESSOR_NAMES[] = { "Tilera Tile-GX", NULL };

/************************************************************************/
/* Definition of Tile-GX assembler                                      */
/************************************************************************/
static asm_t gas = {
    ASH_HEXF3|ASD_DECF0|ASO_OCTF1|ASB_BINF3|AS_N2CHR|AS_LALIGN|AS_1TEXT|AS_ONEDUP|AS_COLON,
    1,                            // uflag
  "GNU assembler",              // name
    0,                            // help
    NULL,                         // header
  ".org",                       // origin
  ".end",                       // end

  "@",          // comment string
  '"',          // string delimiter
  '\'',         // char delimiter
  "\"'",        // special symbols in char and string constants

  ".ascii",     // ascii
  ".byte",      // byte
  ".short",     // word
  ".long",      // dword
  ".quad",      // qword
    NULL,         // oword  (16 bytes)
  ".float",     // float
  ".double",    // double
    NULL,         // tbyte (no information about this directive)
    NULL,         // packreal
  ".ds.#s(b,w,l,d) #d, #v", // arrays (#h,#d,#v,#s(...)
  ".ds.b %s",   // bss
  ".equ",       // equ
    NULL,         // seg
  ".",          // char *a_curip;
    NULL,         // function header
    NULL,         // function footer
  ".globl",     // public
    NULL,         // weak
  ".extern",    // extrn
    NULL,         // comdef
    NULL,         // get name of type
  ".align",     // align
  '(', ')',	// lbrace, rbrace
  "%",     // mod
  "&",     // and
  "|",     // or
  "^",     // xor
  "~",     // not
  ">>",    // shl
  "<<",    // shr
    NULL,    // sizeof
    0,       // flag2
    NULL,    // cmnt2
    NULL,    // low8
    NULL,    // high8
    NULL,    // low16
    NULL,    // high16
    NULL,    // include
    NULL,    // vstruc
    NULL,    // rva
    NULL,    // yword
};

/************************************************************************/
/* Assemblers supported by this module                                  */
/************************************************************************/
asm_t *asms[]       = { &gas, NULL };


/************************************************************************/
/* Register names                                                       */
/************************************************************************/
const char *const REGISTER_NAMES[] =
{
    "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",
    "r10", "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19",
    "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29",
    "r30", "r31", "r32", "r33", "r34", "r35", "r36", "r37", "r38", "r39",
    "r40", "r41", "r42", "r43", "r44", "r45", "r46", "r47", "r48", "r49",
    "r50", "r51", "r52", "r53", "sp",  "lr",  "tp", "idn0", "idn1", "udn0",
    "udn1", "udn2", "udn3", "zero",

    //Special registers
    "CMPEXCH_VALUE", "EX_CONTEXT_0_0", "EX_CONTEXT_0_1", "INTERRUPT_CRITICAL_SECTION",
    "SIM_CONTROL",

    //Fake segment registers
    "CS", "DS"
};

const size_t NUM_REGISTER_NAMES = sizeof(REGISTER_NAMES) / sizeof(REGISTER_NAMES[0]);

/************************************************************************/
/* File headers for Tile-GX assembler                                   */
/************************************************************************/
static const char *const sama_headers[] =
{
  "",
  "; Filename of DEF file describing the chip in use",
  "CHIP <DEF Filename>",
  "",
  "; External memory EQU definitions",
  "; These will appear here when output using the samaout plugin",
  NULL
};


ssize_t newfile(const char *fname)
{
    // extra linefeeds to increase visibility of this message
    msg("\n\n");
    msg("TILE-Gx Processor support, author: Jonas Zaddach <jzaddach@cisco.com>\n");
    msg("based on objdump and hexagon plugin from Willem Jan Hengeveld <itsme@gsmk.de.\n");
    msg("\n");

    return 1;
}


ssize_t is_sane_insn(const insn_t* cmd, int no_crefs)
{
    return cmd->itype != 0;
}

ssize_t loader_elf_machine(linput_t* li, int machine_type, const char **p_procname, proc_def_t **p_pd)
{
    if (machine_type == 191) {
        *p_procname = "tilegx";
    }

    return machine_type;
}

/*
 * Kernel event handler
 *
 * @param msgid Message ID to handle
 * @param ... Variable list of arguments
 * @return 1 on success
 */
static ssize_t idaapi notify(void *, int msgid, va_list va)
{
    switch (msgid) {
        case processor_t::ev_ana_insn:
            return invoke_variadic(&tilegx_ana_insn, va);
        case processor_t::ev_emu_insn:
            return invoke_variadic(&tilegx_emu_insn, va);
        case processor_t::ev_out_insn:
            return invoke_variadic(&tilegx_out_insn, va);
        case processor_t::ev_out_operand:
            return invoke_variadic(&tilegx_out_operand, va);
        case processor_t::ev_out_mnem:
            return invoke_variadic(&tilegx_out_mnem, va);
        case processor_t::ev_loader_elf_machine:
            return invoke_variadic(&loader_elf_machine, va);
        case processor_t::ev_is_sane_insn:
            return invoke_variadic(&is_sane_insn, va);
        default:
            return 0;
    }
}


processor_t LPH =
{
    IDP_INTERFACE_VERSION,// version
    PLFM_TILEGX,               // id,  above 0x8000: thirdparty module
/*  flags used
= PR_USE32           // supports 32-bit addressing?
= PR_DEFSEG32        // segments are 32-bit by default

-- by hexagon
- PR_WORD_INS        // instruction codes are grouped 2bytes in binrary line prefix
h PR_NO_SEGMOVE      // the processor module doesn't support move_segm() (i.e. the user can't move segments)
h PRN_HEX            // default number representation: == hex
? PR_DELAYED         // has delayed jumps and calls if this flag is set, ph.is_basic_block_end should be implemented
-- by arm module
a PR_SEGS            // has segment registers?
a PR_RNAMESOK        // allow to user register names for location names
a PR_TYPEINFO        // the processor module supports type information callbacks ALL OF THEM SHOULD BE IMPLEMENTED!  (the ones >= decorate_name)
a PR_SGROTHER        // the segment registers don't contain the segment selectors, something else
a PR_USE_ARG_TYPES   // use ph.use_arg_types callback
a PR_CNDINSNS        // has conditional instructions
*/

    PR_CNDINSNS|PR_NO_SEGMOVE|PR_USE64|PR_DEFSEG64|PRN_HEX|PR_ALIGN,             // flags
    0,                                                                  // flags2
    8,                    // int32 cnbits - 8 bits in a byte for code segments
    8,                    // int32 dnbits - 8 bits in a byte for other segments

    SHORT_PROCESSOR_NAMES,              // char **psnames -- names shorter than 9 chars.
    LONG_PROCESSOR_NAMES,               // char **plnames

    asms,                 // asm_t **assemblers

    &notify,        // hook_cb_t 

    REGISTER_NAMES,                     // Register names         char **reg_names;         
    NUM_REGISTER_NAMES,            // Number of registers    int32 regs_num;                       

    NUM_REGISTER_NAMES - 2,                         // first       int32 reg_first_sreg;                 
    NUM_REGISTER_NAMES - 1,                         // last        int32 reg_last_sreg;                  
    1,                            // size of a segment register   int32 segreg_size;                    
    NUM_REGISTER_NAMES - 2,                         // int32 reg_code_sreg;                  
    NUM_REGISTER_NAMES - 1,                         // int32 reg_data_sreg;                  

    NULL,                         // No known code start sequences  const bytes_t *codestart;             
    NULL,             // const bytes_t *retcodes;              

    0,                            // int32 instruc_start;                  
    static_cast<int32_t>(NUM_INSTRUCTIONS),        // int32 instruc_end;                    
    INSTRUCTIONS,                 // const instruc_t *instruc;             

};
