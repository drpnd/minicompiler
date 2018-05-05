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

#include "reg.h"

int
x86_64_reg_size(x86_64_reg_t reg)
{
    int size;

    switch ( reg ) {
    case REG_AL:
    case REG_AH:
    case REG_CL:
    case REG_CH:
    case REG_DL:
    case REG_DH:
    case REG_BL:
    case REG_BH:
    case REG_SPL:
    case REG_BPL:
    case REG_SIL:
    case REG_DIL:
    case REG_R8L:
    case REG_R9L:
    case REG_R10L:
    case REG_R11L:
    case REG_R12L:
    case REG_R13L:
    case REG_R14L:
    case REG_R15L:
        size = 1;
        break;
    case REG_AX:
    case REG_CX:
    case REG_DX:
    case REG_BX:
    case REG_SP:
    case REG_BP:
    case REG_SI:
    case REG_DI:
    case REG_R8W:
    case REG_R9W:
    case REG_R10W:
    case REG_R11W:
    case REG_R12W:
    case REG_R13W:
    case REG_R14W:
    case REG_R15W:
    case REG_CS:
    case REG_DS:
    case REG_ES:
    case REG_FS:
    case REG_GS:
    case REG_FLAGS:
        size = 2;
        break;
    case REG_EIP:
    case REG_EAX:
    case REG_ECX:
    case REG_EDX:
    case REG_EBX:
    case REG_ESP:
    case REG_EBP:
    case REG_ESI:
    case REG_EDI:
    case REG_R8D:
    case REG_R9D:
    case REG_R10D:
    case REG_R11D:
    case REG_R12D:
    case REG_R13D:
    case REG_R14D:
    case REG_R15D:
    case REG_EFLAGS:
        size = 4;
        break;
    case REG_RIP:
    case REG_RAX:
    case REG_RCX:
    case REG_RDX:
    case REG_RBX:
    case REG_RSP:
    case REG_RBP:
    case REG_RSI:
    case REG_RDI:
    case REG_R8:
    case REG_R9:
    case REG_R10:
    case REG_R11:
    case REG_R12:
    case REG_R13:
    case REG_R14:
    case REG_R15:
    case REG_RFLAGS:
        size = 8;
        break;
    case REG_ST0:
    case REG_ST1:
    case REG_ST2:
    case REG_ST3:
    case REG_ST4:
    case REG_ST5:
    case REG_ST6:
    case REG_ST7:
        /* 80 bits (x87 registers) */
        size = -1;
        break;
    case REG_MM0:
    case REG_MM1:
    case REG_MM2:
    case REG_MM3:
    case REG_MM4:
    case REG_MM5:
    case REG_MM6:
    case REG_MM7:
        /* FP64 */
        size = 8;
        break;
    case REG_XMM0:
    case REG_XMM1:
    case REG_XMM2:
    case REG_XMM3:
    case REG_XMM4:
    case REG_XMM5:
    case REG_XMM6:
    case REG_XMM7:
        /* FP128 */
        size = 16;
        break;
    case REG_YMM0:
    case REG_YMM1:
    case REG_YMM2:
    case REG_YMM3:
    case REG_YMM4:
    case REG_YMM5:
    case REG_YMM6:
    case REG_YMM7:
        /* FP256 */
        size = 32;
        break;
    case REG_ZMM0:
    case REG_ZMM1:
    case REG_ZMM2:
    case REG_ZMM3:
    case REG_ZMM4:
    case REG_ZMM5:
    case REG_ZMM6:
    case REG_ZMM7:
        /* FP512 */
        size = 64;
        break;
    default:
        size = -1;
    }

    return size;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
