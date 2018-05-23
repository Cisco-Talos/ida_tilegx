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

#include "out.hpp"
#include "log.hpp"
#include "ins.hpp"
#include "reg.hpp"

#include <name.hpp>

ssize_t tilegx_out_insn(outctx_t* ctx)
{
	auto &cmd = ctx->insn;

	//char buf[MAXSTR];
	//init_output_buffer(buf, sizeof(buf));
	log("out(%08" FMT_EA "x)\n", cmd.ea);

	//Output symbol to indicate that the instruction is in the same packet
	if (cmd.ea & 7) {
		ctx->out_char('+');
	}

	ctx->out_mnemonic();

	int16_t feature = INSTRUCTIONS[cmd.itype].feature;

	if (feature & (CF_CHG1 | CF_USE1)) {
		ctx->out_one_operand(0);
	}

	if (feature & (CF_CHG2 | CF_USE2)) {
		ctx->out_symbol(',');
		ctx->out_char(' ');
		ctx->out_one_operand(1);
	}

	if (feature & (CF_CHG3 | CF_USE3)) {
		ctx->out_symbol(',');
		ctx->out_char(' ');
		ctx->out_one_operand(2);
	}

	if (feature & (CF_CHG4 | CF_USE4)) {
		ctx->out_symbol(',');
		ctx->out_char(' ');
		ctx->out_one_operand(3);
	}

	//term_output_buffer();

	//dbgprintf("out:%s\n", buf);
	//gl_comm = 1;                  // generate a user defined comment on this line
	//MakeLine(buf, -1);
	ctx->out_immchar_cmts();
	ctx->flush_outbuf();
}

ssize_t tilegx_out_operand(outctx_t* ctx, const op_t* op)
{
	auto &cmd = ctx->insn;

	//dbgprintf("op %d: d:%x/f:%x/t:%x, value=%x, addr=%x\n", op->n, op->dtype, op->flags, op->type, op->value, op->addr);
	if (op->type == o_reg) {
		ctx->out_register(REGISTER_NAMES[op->reg]);
		return 1;
	}
	else if (op->type==o_near || op->type==o_mem) {
		qstring symbuf;
		ssize_t n= get_name_expr(&symbuf, cmd.ea+op->offb, op->n, op->addr, op->addr);
		if (n>0)
			ctx->out_line(symbuf.c_str());
		else
			ctx->out_value(*op, OOF_ADDR);
		return 1;
	}
	else {
		ctx->out_value(*op, OOFW_IMM);
		return 1;
	}
	return -1;
	//ctx->gen_printf(-1, "op");
}
// ----- output functions
ssize_t tilegx_out_header(outctx_t* ctx)
{
	log("added header\n");
	ctx->gen_cmt_line("Processor       : %s", inf.procname);
	ctx->gen_cmt_line("Target assembler: %s", ash.name);
	ctx->gen_cmt_line("Byte sex        : %s", inf_is_be() ? "Big endian" : "Little endian");
	ctx->gen_cmt_line("%s", "");
	ctx->gen_cmt_line("Hexagon processor module (c) 2017 GSMK");
	ctx->gen_cmt_line("author: Willem Jan Hengeveld, itsme@gsmk.de");

	return 1;
//if ( ash.header != NULL )
//  for ( auto ptr=ash.header; *ptr != NULL; ptr++ )
//    ctx->gen_printf(0,COLSTR("%s",SCOLOR_ASMDIR),*ptr);
}

ssize_t tilegx_out_footer(outctx_t* ctx)
{
	log("added footer\n");
	qstring name = get_colored_name(BADADDR, inf.start_ea);
	ctx->gen_printf(-1,COLSTR("%s",SCOLOR_ASMDIR) " %s", ash.end, name.c_str());

	return 1;
}

ssize_t tilegx_out_segstart(outctx_t* ctx, ea_t ea)
{
	log("segment start(%08" FMT_EA "x)\n", ea);
	ctx->gen_cmt_line("segstart");

	return 1;
}

ssize_t tilegx_out_segend(outctx_t* ctx, ea_t ea)
{
	log("segment end(%08" FMT_EA "x)\n", ea);
	ctx->gen_cmt_line("segend");

	return 1;
}

ssize_t tilegx_out_mnem(outctx_t* outctx)
{ 
	if (outctx->insn.ea & 7) {
		outctx->out_mnem(15);
	} else {
		outctx->out_mnem(16);
	}

	return 1;
}
