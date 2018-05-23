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

#include "emu.hpp"
#include "log.hpp"
#include "ins.hpp"

ssize_t tilegx_emu_insn(const insn_t* cmd)
{
    log("emu(%08" FMT_EA "x), itype=%d\n", cmd->ea, cmd->itype);

    // note: insn_jump and insn_stop do not cause a cref fl_F
    if (INSTRUCTIONS[cmd->itype].feature & CF_STOP) {
        //TODO: This is wrong, the code reference should be from the last instruction in the packet
    }
    else {
        cmd->add_cref(cmd->ea + cmd->size, 0, fl_F);
    }

    for (int i=0 ; i<6 ; i++)
    {
        if (cmd->ops[i].type==o_near) {
            log("adding cref\n");

            if (INSTRUCTIONS[cmd->itype].feature | CF_CALL) {
                cmd->add_cref(cmd->ops[i].addr, 0, fl_CN);
            }
            else if (INSTRUCTIONS[cmd->itype].feature | CF_JUMP) {
                cmd->add_cref(cmd->ops[i].addr, 0, fl_JN);
            }
            else {
                msg("WEIRD: o_near operand type in an instruction that is neither call nor jump at ea 0x%" FMT_EA "x\n", cmd->ea);
            }
        }
        else if (cmd->ops[i].type==o_mem) {
            log("adding dref\n");
            // todo: figure out if we are loading or storing.
            cmd->add_dref(cmd->ops[i].addr, i, dr_R);
        }
        else if (cmd->ops[i].type==o_imm) {
//                if (!is_immext(insn)) {
//                    if (is_mapped(cmd->ops[i].value))
//                        force_offset(cmd, i, 0);
//                    if (is_off(get_flags(cmd->ea), i)) {
//                        dbgprintf("adding drefs\n");
//                        //cmd->add_dref(target, i, dr_O);
//                        cmd->add_off_drefs(cmd->ops[i], dr_O, 0*//*outflags*//*);
//                    }
//
//                }
//                else {
                op_num(cmd->ea, i);
//                }
        }
    }

    // trace stack pointer -> add_auto_stkpnt2(get_func(cmd->ea), cmd->ea+cmd->size, delta);
    // r29 = {add|sub}(r29, #)
    //
    // create stackvars:
    //     ua_stkvar2(x, x.addr, 0) && op_stkvar(cmd->ea, x.n)
    return 1;
}
