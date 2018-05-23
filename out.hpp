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

#ifndef _TILEGX_OUT_HPP
#define _TILEGX_OUT_HPP

#include <idp.hpp>

ssize_t tilegx_out_insn(outctx_t* ctx);
ssize_t tilegx_out_operand(outctx_t* ctx, const op_t* op);
ssize_t tilegx_out_mnem(outctx_t* outctx);

#endif /* _TILEGX_OUT_HPP */
