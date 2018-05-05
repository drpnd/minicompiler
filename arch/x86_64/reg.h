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

#ifndef _X86_64_REG_H
#define _X86_64_REG_H

typedef enum {
    REG_INVAL = -1,
    /* IP */
    REG_EIP,
    REG_RIP,
    /* AX */
    REG_AL,
    REG_AH,
    REG_AX,
    REG_EAX,
    REG_RAX,
    /* CX */
    REG_CL,
    REG_CH,
    REG_CX,
    REG_ECX,
    REG_RCX,
    /* DX */
    REG_DL,
    REG_DH,
    REG_DX,
    REG_EDX,
    REG_RDX,
    /* BX */
    REG_BL,
    REG_BH,
    REG_BX,
    REG_EBX,
    REG_RBX,
    /* SP */
    REG_SPL,
    REG_SP,
    REG_ESP,
    REG_RSP,
    /* BP */
    REG_BPL,
    REG_BP,
    REG_EBP,
    REG_RBP,
    /* SI */
    REG_SIL,
    REG_SI,
    REG_ESI,
    REG_RSI,
    /* DI */
    REG_DIL,
    REG_DI,
    REG_EDI,
    REG_RDI,
    /* R8 */
    REG_R8L,
    REG_R8W,
    REG_R8D,
    REG_R8,
    /* R9 */
    REG_R9L,
    REG_R9W,
    REG_R9D,
    REG_R9,
    /* R10 */
    REG_R10L,
    REG_R10W,
    REG_R10D,
    REG_R10,
    /* R11 */
    REG_R11L,
    REG_R11W,
    REG_R11D,
    REG_R11,
    /* R12 */
    REG_R12L,
    REG_R12W,
    REG_R12D,
    REG_R12,
    /* R13 */
    REG_R13L,
    REG_R13W,
    REG_R13D,
    REG_R13,
    /* R14 */
    REG_R14L,
    REG_R14W,
    REG_R14D,
    REG_R14,
    /* R15 */
    REG_R15L,
    REG_R15W,
    REG_R15D,
    REG_R15,
    /* Segment registers */
    REG_CS,
    REG_DS,
    REG_ES,
    REG_FS,
    REG_GS,
    /* Flags */
    REG_FLAGS,
    REG_EFLAGS,
    REG_RFLAGS,
    /* ST (x87 registers) */
    REG_ST0,
    REG_ST1,
    REG_ST2,
    REG_ST3,
    REG_ST4,
    REG_ST5,
    REG_ST6,
    REG_ST7,
    /* MM */
    REG_MM0,
    REG_MM1,
    REG_MM2,
    REG_MM3,
    REG_MM4,
    REG_MM5,
    REG_MM6,
    REG_MM7,
    /* XMM */
    REG_XMM0,
    REG_XMM1,
    REG_XMM2,
    REG_XMM3,
    REG_XMM4,
    REG_XMM5,
    REG_XMM6,
    REG_XMM7,
    /* YMM */
    REG_YMM0,
    REG_YMM1,
    REG_YMM2,
    REG_YMM3,
    REG_YMM4,
    REG_YMM5,
    REG_YMM6,
    REG_YMM7,
    /* ZMM */
    REG_ZMM0,
    REG_ZMM1,
    REG_ZMM2,
    REG_ZMM3,
    REG_ZMM4,
    REG_ZMM5,
    REG_ZMM6,
    REG_ZMM7,
} x86_64_reg_t;

#ifdef __cplusplus
extern "C" {
#endif

    int x86_64_reg_size(x86_64_reg_t);

#ifdef __cplusplus
}
#endif

#endif /* _X86_64_REG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
