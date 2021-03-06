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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "reg.h"
#include "reg_private.h"

typedef struct {
    char *name;
    off_t pos;
} symbol_t;

typedef struct {
    struct {
        uint8_t *bin;
        off_t len;
        size_t size;
    } text;
    void *symbols;
} code_t;

typedef struct {
    int scale;
    x86_64_reg_t index;
    x86_64_reg_t base;
    int disp;
} x86_64_mem_t;
/*
 * Operand type
 */
typedef enum {
    X86_64_OPERAND_REG,
    X86_64_OPERAND_MEM,
    X86_64_OPERAND_IMM,
} x86_64_operand_type_t;

/*
 * Operand
 */
typedef struct {
    x86_64_operand_type_t type;
    union {
        x86_64_reg_t reg;
        x86_64_mem_t mem;
        int imm;
    } u;
} x86_64_operand_t;

typedef enum {
    OPEN_MR,
    OPEN_RM,
    OPEN_MI,
} open_t;

typedef struct {
    open_t type;
    int len;
    uint8_t data[32];
} encoded_operand_t;


/*
 * try to append the bytes to the code
 */
static int
try_append(code_t *code, uint8_t *arr, size_t sz)
{
    ssize_t i;

    if ( code->text.len + sz > code->text.size ) {
        return -1;
    }
    for ( i = 0; i < (ssize_t)sz; i++ ) {
        printf("<%02x>\n", arr[i]);
        code->text.bin[code->text.len++] = arr[i];
    }

    return 0;
}

#define NARGS(...) ((int)(sizeof((int[]){ __VA_ARGS__ })/sizeof(int)))
#define TRY_APPEND(code, ...)                               \
    do {                                                    \
        if ( try_append((code), (uint8_t[]){__VA_ARGS__},   \
                        NARGS(__VA_ARGS__)) < 0 ) {         \
            return -1;                                      \
        }                                                   \
    } while ( 0 )
#define TRY_APPEND_ARRAY(code, data, n)                         \
    do {                                                        \
        if ( try_append((code), (uint8_t *)(data), (n)) < 0 ) { \
            return -1;                                          \
        }                                                       \
    } while ( 0 )

#define REX_EN          (1 << 6)
#define REX_W           (1 << 3)
#define REX_R           (1 << 2)
#define REX_X           (1 << 1)
#define REX_B           (1)
#define REX(w, r, x, b) (REX_EN | ((w) << 3)  | ((r) << 2) | ((x) << 1) | (b))

#define MODRM(m, r, rm) (((m) << 6) | ((r) << 3) | (rm))
#define SIB(s, i, b)    (((s) << 6) | ((i) << 3) | (b))


/*
 * Determine the operand size
 */
static int
_determin_size(x86_64_operand_t op1, x86_64_operand_t op2)
{
    int sz1;
    int sz2;

    if ( X86_64_OPERAND_REG == op1.type && X86_64_OPERAND_REG == op2.type ) {
        sz1 = x86_64_reg_size(op1.u.reg);
        sz2 = x86_64_reg_size(op2.u.reg);

        /* Check the operand size */
        if ( sz1 != sz2 ) {
            return -1;
        }
        return sz1;
    } else if ( X86_64_OPERAND_REG == op2.type ) {
        sz2 = x86_64_reg_size(op2.u.reg);
        return sz2;
    } else if ( X86_64_OPERAND_REG == op1.type ) {
        sz1 = x86_64_reg_size(op1.u.reg);
        return sz1;
    } else {
        return -1;
    }
}


/*
 * Resolve the operand encoding type and encode the operand
 */
static open_t
_encode(x86_64_operand_t op1, x86_64_operand_t op2, int *opsize,
        encoded_operand_t *enc, int *rex)
{
    open_t open;
    int sz1;
    int sz2;
    int reg1;
    int reg2;
    int rex1;
    int rex2;

    enc->len = 0;
    if ( X86_64_OPERAND_REG == op1.type && X86_64_OPERAND_REG == op2.type ) {
        /* MR */
        open = OPEN_MR;

        sz1 = x86_64_reg_size(op1.u.reg);
        sz2 = x86_64_reg_size(op2.u.reg);
        if ( sz1 != sz2 ) {
            return -1;
        }
        /* Check the operand size */
        if ( *opsize > 0 && *opsize != sz1 ) {
            return -1;
        }
        *opsize = sz1;

        /* Get register values */
        if ( _reg_value(op1.u.reg, &reg1, &rex1) < 0 ) {
            return -1;
        }
        if ( _reg_value(op2.u.reg, &reg2, &rex2) < 0 ) {
            return -1;
        }
        enc->data[enc->len++] = MODRM(3, reg2, reg1);
        if ( rex1 || rex2 ) {
            *rex = REX(1, rex2, 0, rex1);
        } else {
            *rex = 0;
        }

    } else if ( X86_64_OPERAND_MEM == op1.type
                && X86_64_OPERAND_REG == op2.type ) {
        /* MR */
        open = OPEN_MR;

        /* Determine the operand size */
        sz2 = x86_64_reg_size(op2.u.reg);
        if ( *opsize > 0 && *opsize != sz2 ) {
            return -1;
        }
        *opsize = sz2;

        /* Get register values */
        if ( _reg_value(op1.u.mem.index, &reg1, &rex1) < 0 ) {
            return -1;
        }
        if ( _reg_value(op2.u.reg, &reg2, &rex2) < 0 ) {
            return -1;
        }
        if ( REG_INVAL != op1.u.mem.base ) {
            /* SIB */
            fprintf(stderr, "FIXME!\n");
            exit(1);
        } else {
            /* ModR/M and displacement */
            if ( op1.u.mem.disp >= -128 && op1.u.mem.disp <= 127 ) {
                enc->data[enc->len++] = MODRM(1, reg2, reg1);
                enc->data[enc->len++] = op1.u.mem.disp;
            } else {
                enc->data[enc->len++] = MODRM(2, reg2, reg1);
                (void)memcpy(enc->data + enc->len, &op1.u.mem.disp, 4);
                enc->len += 4;
            }
            if ( rex1 ) {
                *rex = REX(1, rex2, rex1, 0);
            } else {
                *rex = 0;
            }
        }
    } else if ( X86_64_OPERAND_REG == op1.type
                && X86_64_OPERAND_MEM == op2.type ) {
        /* RM */
        open = OPEN_RM;

        /* Determine the operand size */
        sz1 = x86_64_reg_size(op1.u.reg);
        if ( *opsize > 0 && *opsize != sz1 ) {
            return -1;
        }
        *opsize = sz1;

        /* Get register values */
        if ( _reg_value(op1.u.reg, &reg1, &rex1) < 0 ) {
            return -1;
        }
        if ( _reg_value(op2.u.mem.index, &reg2, &rex2) < 0 ) {
            return -1;
        }
        if ( REG_INVAL != op2.u.mem.base ) {
            /* SIB */
            fprintf(stderr, "FIXME!\n");
            exit(1);
        } else {
            /* ModR/M and displacement */
            if ( op2.u.mem.disp >= -128 && op2.u.mem.disp <= 127 ) {
                enc->data[enc->len++] = MODRM(1, reg1, reg2);
                enc->data[enc->len++] = op2.u.mem.disp;
            } else {
                enc->data[enc->len++] = MODRM(2, reg1, reg2);
                (void)memcpy(enc->data + enc->len, &op2.u.mem.disp, 4);
                enc->len += 4;
            }
            if ( rex1 ) {
                *rex = REX(1, rex1, rex2, 0);
            } else {
                *rex = 0;
            }
        }
    } else if ( X86_64_OPERAND_MEM == op1.type
                && X86_64_OPERAND_IMM == op2.type ) {
        /* MI */
        open = OPEN_MI;

        /* Get register values */
        if ( _reg_value(op1.u.mem.index, &reg1, &rex1) < 0 ) {
            return -1;
        }
        if ( REG_INVAL != op1.u.mem.base ) {
            /* SIB */
            fprintf(stderr, "FIXME!\n");
            exit(1);
        } else {
            /* ModR/M and displacement */
            if ( op1.u.mem.disp >= -128 && op1.u.mem.disp <= 127 ) {
                enc->data[enc->len++] = MODRM(1, 0, reg1);
                enc->data[enc->len++] = op1.u.mem.disp;
            } else {
                enc->data[enc->len++] = MODRM(2, 0, reg1);
                (void)memcpy(enc->data + enc->len, &op1.u.mem.disp, 4);
                enc->len += 4;
            }
            /* Immendiate */
            (void)memcpy(enc->data + enc->len, &op2.u.imm, 4);
            enc->len += 4;
            if ( rex1 ) {
                *rex = REX(1, 0, rex1, 0);
            } else {
                *rex = 0;
            }
        }
    } else {
        /* FIXME */
        fprintf(stderr, "FIXME!\n");
        exit(1);
    }

    return open;
}


int
pushq(code_t *code)
{
    TRY_APPEND(code, 0x55);

    return 0;
}

int
popq(code_t *code)
{
    TRY_APPEND(code, 0x5d);

    return 0;
}

int
retq(code_t *code)
{
    TRY_APPEND(code, 0xc3);

    return 0;
}

/*
 * MOV (Vol. 2B 4-29)
 *
 *      Opcode          Instruction             Op/En   64-bit  Compat/Leg
 *      88 /r           MOV r/m8,r8             MR      Valid   Valid
 *      REX + 88 /r     MOV r/m8***,r8***       MR      Valid   N.E.
 *      89 /r           MOV r/m16,r16           MR      Valid   Valid
 *      89 /r           MOV r/m32,r32           MR      Valid   Valid
 *      REX.W + 89 /r   MOV r/m64,r64           MR      Valid   N.E.
 *      8A /r           MOV r8,r/m8             RM      Valid   Valid
 *      REX + 8A /r     MOV r8***,r/m8***       RM      Valid   N.E.
 *      8B /r           MOV r16,r/m16           RM      Valid   Valid
 *      8B /r           MOV r32,r/m32           RM      Valid   Valid
 *      REX.W + 8B /r   MOV r64,r/m64           RM      Valid   N.E.
 *      8C /r           MOV r/m16,Sreg**        MR      Valid   Valid
 *      REX.W 8C /r     MOV r/m64,Sreg**        MR      Valid   Valid
 *      8E /r           MOV Sreg,r/m16**        RM      Valid   Valid
 *      REX.W 8E /r     MOV Sreg,r/m16**        RM      Valid   Valid
 *      A0              MOV AL,moffs8*          FD      Valid   Valid
 *      REX.W + A0      MOV AL,moffs8*          FD      Valid   N.E.
 *      A1              MOV AX,moffs16*         FD      Valid   Valid
 *      A1              MOV EAX,moffs32*        FD      Valid   Valid
 *      REX.W + A1      MOV RAX,moffs64*        FD      Valid   N.E.
 *      A2              MOV moffs8,AL           TD      Valid   Valid
 *      REX.W + A2      MOV moffs8***,AL        TD      Valid   N.E.
 *      A3              MOV moffs16*,AX         TD      Valid   Valid
 *      A3              MOV moffs32*,EAX        TD      Valid   Valid
 *      REX.W + A3      MOV moffs64*,RAX        TD      Valid   N.E.
 *      B0+ rb ib       MOV r8,imm8             OI      Valid   Valid
 *      REX + B0+ rb ib MOV r8***,imm8          OI      Valid   N.E.
 *      B8+ rw iw       MOV r16,imm16           OI      Valid   Valid
 *      B8+ rd id       MOV r32,imm32           OI      Valid   Valid
 *      REX.W + B8+ rd io
 *                      MOV r64,imm64           OI      Valid   N.E.
 *      C6 /0 ib        MOV r/m8,imm8           MI      Valid   Valid
 *      REX + C6 /0 ib  MOV r/m8***,imm8        MI      Valid   N.E.
 *      C7 /0 iw        MOV r/m16,imm16         MI      Valid   Valid
 *      C7 /0 id        MOV r/m32,imm32         MI      Valid   Valid
 *      REX.W + C7 /0 io
 *                      MOV r/m64,imm32         MI      Valid   N.E.
 *
 *      *   The moffs8, moffs16, moffs32 and moffs64 operands specify a simple
 *          offset relative to the segment base, where 8, 16, 32 and 64 refer to
 *          the size of the data.  The address-size attribute of the instruction
 *          determines the size of the offset, either 16, 32 or 64 bits.
 *      **  In 32-bit mode, the assembler may insert the 16-bit operand-size
 *          prefix with this instruction
 *      *** In 64-bit mode, AH, BH, CH, DH cannot be accessed
 *
 *
 *      Op/En   Operand1        Operand2        Operand3        Operand4
 *      MR      ModRM:r/m(w)    ModRM:reg(r)    NA              NA
 *      RM      ModRM:reg(w)    ModRM:r/m(r)    NA              NA
 *      FD      AL/AX/EAX/RAX   Moffs           NA              NA
 *      TD      Moffs(w)        AL/AX/EAX/RAX   NA              NA
 *      OI      opcode + rd(w)  imm8/16/32/64   NA              NA
 *      MI      ModRM:r/m(w)    imm8/16/32/64   NA              NA
 */
int
mov(code_t *code, x86_64_operand_t op1, x86_64_operand_t op2)
{
    int rex;
    int op;
    open_t open;
    encoded_operand_t enc;
    int opsize;

    /* Determine the operand size */
    opsize = _determin_size(op1, op2);
    if ( opsize < 0 ) {
        return -1;
    }

    /* Initialize */
    enc.len = 0;

    open = _encode(op1, op2, &opsize, &enc, &rex);
    switch ( open ) {
    case OPEN_MR:
        op = 0x89;
        if ( 8 == opsize ) {
            rex |= REX(1, 0, 0, 0);
        }
        break;
    case OPEN_RM:
        op = 0x8b;
        break;
    case OPEN_MI:
        op = 0xc7;
        break;
    default:
        /* FIXME */
        fprintf(stderr, "FIXME: MOV!\n");
        exit(1);
    }

    if ( rex ) {
        TRY_APPEND(code, rex, op);
        TRY_APPEND_ARRAY(code, enc.data, enc.len);
    } else {
        TRY_APPEND(code, op);
        TRY_APPEND_ARRAY(code, enc.data, enc.len);
    }

    return 0;
}

#define OP_REG(v, r)                                 \
    do {                                             \
        (v).type = X86_64_OPERAND_REG;               \
        (v).u.reg = (r);                             \
    } while ( 0 )

#define OP_MEM(v, s, i, b, d)                        \
    do {                                             \
        (v).type = X86_64_OPERAND_MEM;               \
        (v).u.mem.scale = (s);                       \
        (v).u.mem.index = (i);                       \
        (v).u.mem.base = (b);                        \
        (v).u.mem.disp = (d);                        \
    } while ( 0 )

#define OP_IMM(v, i)                                 \
    do {                                             \
        (v).type = X86_64_OPERAND_IMM;               \
        (v).u.imm = (i);                             \
    } while ( 0 )


/*
 * Main routine
 */
int
main(void)
{
    code_t code;
    x86_64_operand_t op1;
    x86_64_operand_t op2;

    code.text.bin = malloc(sizeof(1024));
    code.text.len = 0;
    code.text.size = 1024;

    /* popq */
    pushq(&code);

    /* mov %rsp, %rbp */
    OP_REG(op1, REG_RBP);
    OP_REG(op2, REG_RSP);
    mov(&code, op1, op2);

    /* mov $0, -4(%rbp) */
    OP_MEM(op1, 1, REG_RBP, REG_INVAL, -4);
    OP_IMM(op2, 0);
    mov(&code, op1, op2);

    /* mov %edi, -8(%rbp) */
    OP_MEM(op1, 1, REG_RBP, REG_INVAL, -8);
    OP_REG(op2, REG_EDI);
    mov(&code, op1, op2);

    /* mov %rsi -0x10(%rbp) */
    OP_MEM(op1, 1, REG_RBP, REG_INVAL, -0x10);
    OP_REG(op2, REG_RSI);
    mov(&code, op1, op2);

    /* mov -8(%rbp), %eax */
    OP_REG(op1, REG_EAX);
    OP_MEM(op2, 1, REG_RBP, REG_INVAL, -8);
    mov(&code, op1, op2);

    /* popq */
    popq(&code);

    /* retq */
    retq(&code);

    return 0;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
