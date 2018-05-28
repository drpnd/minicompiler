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
#ifndef _SYNTAX_H
#define _SYNTAX_H

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
} op_type_t;

typedef enum {
    FIX_INFIX,
    FIX_PREFIX,
} fix_t;

typedef enum {
    EXPR_LITERAL,
    EXPR_OP,
} expr_type_t;

typedef struct _expr expr_t;

typedef enum {
    LITERAL_INT,
} literal_type_t;
typedef struct {
    literal_type_t type;
    union {
        int i;
    } u;
} literal_t;

typedef struct {
    op_type_t type;
    fix_t fix;
    expr_t *e0;
    expr_t *e1;
} op_t;

struct _expr {
    expr_type_t type;
    union {
        literal_t *lit;
        op_t *op;
    } u;
};

#ifdef __cplusplus
extern "C" {
#endif

    literal_t * literal_int(int);
    expr_t * expr_lit(literal_t *);
    expr_t * expr_op(expr_t *, expr_t *, op_type_t);
    void debug(expr_t *e);

#ifdef __cplusplus
}
#endif

#endif /* _SYNTAX_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
