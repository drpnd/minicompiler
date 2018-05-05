/*_
 * Copyright (c) 2018 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _X86_64_REG_PRIVATE_H
#define _X86_64_REG_PRIVATE_H

#include "reg.h"

/*
 * Register to the value used for ModR/M
 */
static __inline__ int
_reg_value(x86_64_reg_t reg, int *idxp, int *rexp)
{
    int idx;
    int rex;

    rex = 0;
    switch ( reg ) {
    case REG_R8L:
    case REG_R8W:
    case REG_R8D:
    case REG_R8:
        rex = 1;
    case REG_AL:
    case REG_AX:
    case REG_EAX:
    case REG_RAX:
    case REG_MM0:
    case REG_XMM0:
        idx = 0;
        break;
    case REG_R9L:
    case REG_R9W:
    case REG_R9D:
    case REG_R9:
        rex = 1;
    case REG_CL:
    case REG_CX:
    case REG_ECX:
    case REG_RCX:
    case REG_MM1:
    case REG_XMM1:
        idx = 1;
        break;
    case REG_R10L:
    case REG_R10W:
    case REG_R10D:
    case REG_R10:
        rex = 1;
    case REG_DL:
    case REG_DX:
    case REG_EDX:
    case REG_RDX:
    case REG_MM2:
    case REG_XMM2:
        idx = 2;
        break;
    case REG_R11L:
    case REG_R11W:
    case REG_R11D:
    case REG_R11:
        rex = 1;
    case REG_BL:
    case REG_BX:
    case REG_EBX:
    case REG_RBX:
    case REG_MM3:
    case REG_XMM3:
        idx = 3;
        break;
    case REG_R12L:
    case REG_R12W:
    case REG_R12D:
    case REG_R12:
        rex = 1;
    case REG_AH:
    case REG_SP:
    case REG_ESP:
    case REG_RSP:
    case REG_MM4:
    case REG_XMM4:
        idx = 4;
        break;
    case REG_R13L:
    case REG_R13W:
    case REG_R13D:
    case REG_R13:
        rex = 1;
    case REG_CH:
    case REG_BP:
    case REG_EBP:
    case REG_RBP:
    case REG_MM5:
    case REG_XMM5:
        idx = 5;
        break;
    case REG_R14L:
    case REG_R14W:
    case REG_R14D:
    case REG_R14:
        rex = 1;
    case REG_DH:
    case REG_SI:
    case REG_ESI:
    case REG_RSI:
    case REG_MM6:
    case REG_XMM6:
        idx = 6;
        break;
    case REG_R15L:
    case REG_R15W:
    case REG_R15D:
    case REG_R15:
        rex = 1;
    case REG_BH:
    case REG_DI:
    case REG_EDI:
    case REG_RDI:
    case REG_MM7:
    case REG_XMM7:
        idx = 6;
        break;
    default:
        return -1;
    }

    *idxp = idx;
    *rexp = rex;

    return 0;
}

#endif /* _X86_64_REG_PRIVATE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
