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

#include "ins.hpp"

const instruc_t INSTRUCTIONS[] = {
    {"",                   0},                           //Cannot decode instruction
    {"add",                CF_CHG1 | CF_USE2 | CF_USE3}, //Add arithmetic
    {"addi",               CF_CHG1 | CF_USE2 | CF_USE3}, //Add immediate
    {"addli",              CF_CHG1 | CF_USE2 | CF_USE3}, //Add long immediate
    {"addx",               CF_CHG1 | CF_USE2 | CF_USE3}, //Add and extend
    {"addxi",              CF_CHG1 | CF_USE2 | CF_USE3}, //Add and extend immediate
    {"addxli",             CF_CHG1 | CF_USE2 | CF_USE3}, //Add and extend long immediate
    {"addxsc",             CF_CHG1 | CF_USE2 | CF_USE3}, //Add signed clamped and extend
    {"and",                CF_CHG1 | CF_USE2 | CF_USE3}, //And
    {"andi",               CF_CHG1 | CF_USE2 | CF_USE3}, //And immediate
    {"beqz",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch equal zero
    {"beqzt",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch equal zero predict taken
    {"bfexts",             CF_CHG1 | CF_USE2 | CF_USE3 | CF_USE4}, //Bit field extract signed 
    {"bfextu",             CF_CHG1 | CF_USE2 | CF_USE3 | CF_USE4}, //Bit field extract unsigned
    {"bfins",              CF_USE1 | CF_USE2 | CF_USE3 | CF_USE4}, //Bit field insert
    {"bgez",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch greater than or equal to zero
    {"bgezt",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch greater than or equal to zero predict taken           
    {"bgtz",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch greater than zero
    {"bgtzt",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch greater than zero predict taken
    {"blbc",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch low bit clear
    {"blbct",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch low bit clear predict taken
    {"blbs",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch low bit set
    {"blbst",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch low bit set predict taken
    {"blez",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch less than or equal to zero
    {"blezt",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch less than or equal to zero predict taken
    {"bltz",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch less than zero
    {"bltzt",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch less than zero predict taken
    {"bnez",               CF_USE1 | CF_USE2 | CF_JUMP}, //Branch not equal zero
    {"bnezt",              CF_USE1 | CF_USE2 | CF_JUMP}, //Branch not equal zero predict taken
    {"bpt",                0},                           //Breakpoint
    {"clz",                CF_CHG1 | CF_USE2},           //Count leading zeros
    {"cmoveqz",            CF_CHG1 | CF_USE2 | CF_USE3}, //Conditional move if equal zero
    {"cmovnez",            CF_CHG1 | CF_USE2 | CF_USE3}, //Conditional move if not equal zero
    {"cmpeq",              CF_CHG1 | CF_USE2 | CF_USE3}, //compare equal
    {"cmpeqi",             CF_CHG1 | CF_USE2 | CF_USE3}, //Compare equal immediate
    {"cmpexch",            CF_CHG1 | CF_USE2 | CF_USE3}, //compare and exchange
    {"cmpexch4",           CF_CHG1 | CF_USE2 | CF_USE3}, //Compare and exchange four bytes
    {"cmples",             CF_CHG1 | CF_USE2 | CF_USE3}, //Compare less than or equal signed
    {"cmpleu",             CF_CHG1 | CF_USE2 | CF_USE3}, //Compare less than or equal unsigned
    {"cmplts",             CF_CHG1 | CF_USE2 | CF_USE3}, //Compare less than signed
    {"cmpltsi",            CF_CHG1 | CF_USE2 | CF_USE3}, //Compare less than signed immediate
    {"cmpltu",             CF_CHG1 | CF_USE2 | CF_USE3}, //Compare less than unsigned
    {"cmpltui",            CF_CHG1 | CF_USE2 | CF_USE3}, //Compare less than unsigned immediate
    {"cmpne",              CF_CHG1 | CF_USE2 | CF_USE3}, //Compare not equal
    {"cmul",               CF_CHG1 | CF_USE2 | CF_USE3}, //Complex Multiply
    {"cmula",              CF_USE1 | CF_CHG1 | CF_USE2 | CF_USE3}, //complex multiply accumulate
    {"cmulaf",             CF_USE1 | CF_CHG1 | CF_USE2 | CF_USE3}, //complex multiply accumulate fixed point
    {"cmulf",              CF_CHG1 | CF_USE2 | CF_USE3}, //complex multiply fixed point
    {"cmulfr",             CF_CHG1 | CF_USE2 | CF_USE3}, //complex multiply fixed point round
    {"cmulh",              CF_CHG1 | CF_USE2 | CF_USE3}, //complex multiply high result
    {"cmulhr",             CF_CHG1 | CF_USE2 | CF_USE3}, //complex multiply high result round
    {"crc32_32",           CF_CHG1 | CF_USE2 | CF_USE3}, //CRC32 32-bit step
    {"crc32_32",           CF_CHG1 | CF_USE2 | CF_USE3}, //CRC32 8-bit step
    {"ctz",                CF_CHG1 | CF_USE2},           //Count trailing zeros
    {"dblalign",           CF_USE1 | CF_CHG1 | CF_USE2 | CF_USE3}, //Double align
    {"dblalign2",          CF_USE1 | CF_CHG1 | CF_USE2 | CF_USE3}, //Double align by two bytes
    {"dblalign4",          CF_USE1 | CF_CHG1 | CF_USE2 | CF_USE3}, //Double align by four bytes
    {"dblalign6",          CF_USE1 | CF_CHG1 | CF_USE2 | CF_USE3}, //Double align by six bytes
    {"drain",              0},                           //Drain instruction
    {"dtlbbpr",            CF_USE1},                     //Data TLB proble
    {"exch",               CF_CHG1 | CF_USE2 | CF_USE3}, //Exchange 
    {"exch4",              CF_CHG1 | CF_USE2 | CF_USE3}, //Exchange four bytes
    {"fdouble_add_flags",  CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision add flags
    {"fdouble_addsub",     CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision add or subtract
    {"fdouble_mul_flags",  CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision multiply flags
    {"fdouble_pack1",      CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision pack part 1
    {"fdouble_pack2",      CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision pack part 2
    {"fdouble_sub_flags",  CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision subtract flags
    {"fdouble_unpack_max", CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision unpack max
    {"fdouble_unpack_min", CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point double precision unpack min
    {"fetchadd",           CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and add
    {"fetchadd4",          CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and add four bytes
    {"fetchaddgez",        CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and add if greater or equal zero
    {"fetchaddgez4",       CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and add four bytes if greater or equal zero
    {"fetchand",           CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and and
    {"fetchand4",          CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and and four bytes
    {"fetchor",            CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and or
    {"fetchor4",           CF_CHG1 | CF_USE2 | CF_USE3}, //Fetch and or four bytes
    {"finv",               CF_USE1},                     //Flush and invalidate cache line
    {"flush",              CF_USE1},                     //Flush cache line
    {"flushwb",            0},                           //Flush write buffers
    {"fnop",               0},                           //Filter no operation
    {"fsingle_add1",       CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point single precision add part 1
    {"fsingle_addsub2",    CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point single precision add or subtract part 2
    {"fsingle_mul1",       CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point single precision multiply part 1
    {"fsingle_mul2",       CF_CHG1 | CF_USE2 | CF_USE3}, //Floating point single precision multiply part 2
    {"fsingle_pack1",      CF_CHG1 | CF_USE2},           //Floating point single precision pack part 1
    {"fsingle_pack2",      CF_CHG1 | CF_USE2},           //Floating point single precision pack part 2
    {"icoh",               CF_USE1},                     //Instruction stream coherence
    {"ill",                0},                           //Illegal instruction
    {"info",               CF_USE1},                     //Informational note
    {"info1",              CF_USE1},                     //Long informational note
    {"iret",               CF_STOP},                     //Interrupt return            
    {"j",                  CF_USE1 | CF_JUMP | CF_STOP}, //Jump
    {"jal",                CF_USE1 | CF_CALL},           //Jump and link
    {"jalr",               CF_USE1 | CF_CALL},           //Jump and link register
    {"jalrp",              CF_USE1 | CF_CALL},           //Jump and link register predict
    {"jr",                 CF_USE1 | CF_JUMP | CF_STOP}, //Jump register
    {"jrp",                CF_USE1 | CF_JUMP | CF_STOP}, //Jump register predict
    {"ld",                 CF_CHG1 | CF_USE2},           //Load
    {"ld1s",               CF_CHG1 | CF_USE2},           //Load one byte signed
    {"ld1s_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load one byte signed and add
    {"ld1u",               CF_CHG1 | CF_USE2},           //Load one byte unsigned
    {"ld1u_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load one byte unsigned and add
    {"ld2s",               CF_CHG1 | CF_USE2},           //Load two bytes signed
    {"ld2s_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load two bytes signed and add
    {"ld2u",               CF_CHG1 | CF_USE2},           //Load two bytes unsigned
    {"ld2u_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load two bytes unsigned and add
    {"ld4s",               CF_CHG1 | CF_USE2},           //Load four bytes signed
    {"ld4s_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load four bytes signed and add
    {"ld4u",               CF_CHG1 | CF_USE2},           //Load four bytes unsigned
    {"ld4u_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load four bytes unsigned and add
    {"ld_add",             CF_CHG1 | CF_USE2 | CF_USE3}, //Load and add
    {"ldna",               CF_CHG1 | CF_USE2},           //Load no alignment trap
    {"ldna_add",           CF_CHG1 | CF_USE2 | CF_USE3}, //Load no alignment trap and add
    {"ldnt",               CF_CHG1 | CF_USE2},           //Load non-temporal
    {"ldnt1s",             CF_CHG1 | CF_USE2},           //Load non-temporal one byte signed
    {"ldnt1s_add",         CF_CHG1 | CF_USE2 | CF_USE3}, //Load non-temporal one byte signed and add
    {"ldnt1u",             CF_CHG1 | CF_USE2},           //Load non-temporal one byte unsigned
    {"ldnt1u_add",         CF_CHG1 | CF_USE2 | CF_USE3}, //Load non-temporal one byte unsigned and add
    {"ldnt2s",             CF_CHG1 | CF_USE2},           //Load non-temporal two bytes signed
    {"ldnt2s_add",         CF_CHG1 | CF_USE2 | CF_USE3}, //Load non-temporal two bytes signed and add
    {"ldnt2u",             CF_CHG1 | CF_USE2},           //Load non-temporal two bytes unsigned
    {"ldnt2u_add",         CF_CHG1 | CF_USE2 | CF_USE3}, //Load non-temporal two bytes unsigned and add
    {"ldnt4s",             CF_CHG1 | CF_USE2},           //Load non-temporal four bytes signed
    {"ldnt4s_add",         CF_CHG1 | CF_USE2 | CF_USE3}, //Load non-temporal four bytes signed and add
    {"ldnt4u",             CF_CHG1 | CF_USE2},           //Load non-temporal four bytes unsigned
    {"ldnt4u_add",         CF_CHG1 | CF_USE2 | CF_USE3}, //Load non-temporal four bytes unsigned and add
    {"lnk",                CF_CHG1},                     //Link
    {"mf",                 0},                           //Memory fence
    {"mfspr",              CF_CHG1},                     //Move from special purpose register word
    {"mm",                 CF_CHG1 | CF_USE2},           //Masked merge
    {"mnz",                CF_CHG1 | CF_USE2 | CF_USE3}, //Masked not zero
    {"move",               CF_CHG1 | CF_USE2},           //Move
    {"movei",              CF_CHG1 | CF_USE2},           //Move immediate
    {"moveli",             CF_CHG1 | CF_USE2},           //Move long immediate
    {"mtspr",              CF_USE1 | CF_USE2},           //Move to special purpose register word
    {"mula_hs_hs",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high signed high signed
    {"mula_hs_hu",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high signed high unsigned
    {"mula_hs_ls",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high signed low signed
    {"mula_hs_lu",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high signed low unsigned
    {"mula_hu_hu",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high unsigned high unsigned
    {"mula_hu_ls",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high unsigned low signed
    {"mula_hu_lu",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate high unsigned low unsigned
    {"mula_ls_ls",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate low signed low signed
    {"mula_ls_lu",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate low signed low unsigned
    {"mula_lu_lu",         CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate low unsigned low unsigned
    {"mulax",              CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply accumulate and extend
    {"mulx",               CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply and extend
    {"mul_hs_hs",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high signed high signed
    {"mul_hs_hu",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high signed high unsigned
    {"mul_hs_ls",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high signed low signed
    {"mul_hs_lu",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high signed low unsigned
    {"mul_hu_hu",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high unsigned high unsigned
    {"mul_hu_ls",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high unsigned low signed
    {"mul_hu_lu",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply high unsigned low unsigned
    {"mul_ls_ls",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply low signed low signed
    {"mul_ls_lu",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply low signed low unsigned
    {"mul_lu_lu",          CF_CHG1 | CF_USE2 | CF_USE3}, //Multiply low unsigned low unsigned
    {"mz",                 CF_CHG1 | CF_USE2 | CF_USE3}, //Mask zero
    {"nap",                0},                           //Nap
    {"nop",                0},                           //Nop
    {"nor",                CF_CHG1 | CF_USE2 | CF_USE3}, //Nor
    {"or",                 CF_CHG1 | CF_USE2 | CF_USE3}, //Or
    {"ori",                CF_CHG1 | CF_USE2 | CF_USE3}, //Or immediate
    {"pcnt",               CF_CHG1 | CF_USE2},           //Population count
    {"prefetch",           CF_USE1},                     //Prefetch to L1 with no faults
    {"prefetch_add_l1",    CF_USE1},                     //Prefetch to L1 and add with no faults
    {"prefetch_add_l1_fault", CF_USE1},                  //Prefetch to L1 and add with faults
    {"prefetch_add_l2",    CF_USE1},                     //Prefetch to L2 and add with no faults
    {"prefetch_add_l2_fault", CF_USE1},                  //Prefetch to L2 and add with faults
    {"prefetch_add_l3",    CF_USE1},                     //Prefetch to L3 and add with no faults
    {"prefetch_add_l3_fault", CF_USE1},                  //Prefetch to L3 and add with faults
    {"prefetch_l1",        CF_USE1},                     //Prefetch to L1 with no faults
    {"prefetch_l1_fault",  CF_USE1},                     //Prefetch to L1 with faults
    {"prefetch_l2",        CF_USE1},                     //Prefetch to L2 with no faults
    {"prefetch_l2_fault",  CF_USE1},                     //Prefetch to L2 with faults
    {"prefetch_l3",        CF_USE1},                     //Prefetch to L3 with no faults
    {"prefetch_l3_fault",  CF_USE1},                     //Prefetch to L3 with faults
    {"raise",              CF_STOP},                     //Raise signal
    {"revbits",            CF_CHG1 | CF_USE2},           //Reverse bits
    {"revbytes",           CF_CHG1 | CF_USE2},           //Reverse bytes
    {"rotl",               CF_CHG1 | CF_USE2 | CF_USE3}, //Rotate left 
    {"rotli",              CF_CHG1 | CF_USE2 | CF_USE3}, //Rotate left immediate
    {"shl",                CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left
    {"shl16insli",         CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left 16 insert long immediate
    {"shl1add",            CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left one and add
    {"shl1addx",           CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left one add and extend
    {"shl2add",            CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left two and add
    {"shl2addx",           CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left two add and extend
    {"shl3add",            CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left three and add
    {"shl3addx",           CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left three add and extend
    {"shli",               CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left immediate
    {"shlx",               CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left and extend
    {"shlxi",              CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift left and extend immediate
    {"shrs",               CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift right signed
    {"shrsi",              CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift right signed immediate
    {"shru",               CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift right unsigned
    {"shrui",              CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift right unsigned immediate
    {"shrux",              CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift right unsigned and extend
    {"shruxi",             CF_CHG1 | CF_USE2 | CF_USE3 | CF_SHFT}, //Shift right unsigned and extend immediate
    {"shufflebytes",       CF_CHG1 | CF_USE2 | CF_USE3}, //Shuffle bytes
    {"st",                 CF_USE1 | CF_USE2},           //Store
    {"st1",                CF_USE1 | CF_USE2},           //Store byte
    {"st1_add",            CF_USE1 | CF_USE2 | CF_USE3}, //Store byte and add
    {"st2",                CF_USE1 | CF_USE2},           //Store two bytes
    {"st2_add",            CF_USE1 | CF_USE2 | CF_USE3}, //Store two bytes and add
    {"st4",                CF_USE1 | CF_USE2},           //Store four bytes
    {"st4_add",            CF_USE1 | CF_USE2 | CF_USE3}, //Store four bytes and add
    {"st_add",             CF_USE1 | CF_USE2 | CF_USE3}, //Store and add
    {"stnt",               CF_USE1 | CF_USE2},           //Store non-temporal
    {"stnt1",              CF_USE1 | CF_USE2},           //Store non-temporal byte
    {"stnt1_add",          CF_USE1 | CF_USE2 | CF_USE3}, //Store non-temporal byte and add
    {"stnt2",              CF_USE1 | CF_USE2},           //Store non-temporal two bytes
    {"stnt2_add",          CF_USE1 | CF_USE2 | CF_USE3}, //Store non-temporal two bytes and add
    {"stnt4",              CF_USE1 | CF_USE2},           //Store non-temporal four bytes
    {"stnt4_add",          CF_USE1 | CF_USE2 | CF_USE3}, //Store non-temporal four bytes and add
    {"stnt_add",           CF_USE1 | CF_USE2 | CF_USE3}, //Store non-temporal and add
    {"sub",                CF_CHG1 | CF_USE2 | CF_USE3}, //Subtract
    {"subx",               CF_CHG1 | CF_USE2 | CF_USE3}, //Subtract and extend
    {"subxsc",             CF_CHG1 | CF_USE2 | CF_USE3}, //Subtract signed clamped and extend
    {"swint0",             CF_STOP},                     //Software interrupt 0
    {"swint1",             CF_STOP},                     //Software interrupt 1
    {"swint2",             CF_STOP},                     //Software interrupt 2
    {"swint3",             CF_STOP},                     //Software interrupt 3
    {"tblidxb0",           CF_CHG1 | CF_USE2},           //Table index byte 0
    {"tblidxb1",           CF_CHG1 | CF_USE2},           //Table index byte 1
    {"tblidxb2",           CF_CHG1 | CF_USE2},           //Table index byte 2
    {"tblidxb3",           CF_CHG1 | CF_USE2},           //Table index byte 3
    //TODO: Vectored instructions missing
    {"v1add",              CF_CHG1 | CF_USE2 | CF_USE3}, //Vector one byte add 
    {"v1addi",             CF_CHG1 | CF_USE2 | CF_USE3}, //Vector one byte add immediate
    {"v1cmpeq",            CF_CHG1 | CF_USE2 | CF_USE3}, //Vector one byte set equal to
    {"v1cmpeqi",           CF_CHG1 | CF_USE2 | CF_USE3}, //Vector one byte set equal to immediate
    {"v1int_l",            CF_CHG1 | CF_USE2 | CF_USE3}, //Vector one byte interleave low
    {"v1shrui",            CF_CHG1 | CF_USE2 | CF_USE3}, //Vector one byte shift right unsigned immediate
    {"v4int_l",            CF_CHG1 | CF_USE2 | CF_USE3}, //Vector four bytes interleave low

    {"wh64",               CF_USE1},                     //Write hint 64 bytes
    {"xor",                CF_CHG1 | CF_USE2 | CF_USE3}, //Exlusive or
    {"xori",               CF_CHG1 | CF_USE2 | CF_USE3} }; //Exclusive or immediate

const size_t NUM_INSTRUCTIONS = sizeof(INSTRUCTIONS) / sizeof(INSTRUCTIONS[0]);
