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

#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Literal
 */
literal_t *
literal_int(int i)
{
    literal_t *lit;

    lit = malloc(sizeof(literal_t));
    if ( NULL == lit ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    lit->type = LITERAL_INT;
    lit->u.i = i;

    return lit;
}

/*
 * Literal expression
 */
expr_t *
expr_lit(literal_t *lit)
{
    expr_t *e;

    e = malloc(sizeof(expr_t));
    if ( NULL == e ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    e->type = EXPR_LITERAL;
    e->u.lit = lit;

    return e;
}

/*
 * ID
 */
expr_t *
expr_id(char *id)
{
    expr_t *e;

    e = malloc(sizeof(expr_t));
    if ( NULL == e ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    e->type = EXPR_ID;
    e->u.id = strdup(id);
    if ( NULL == e->u.id ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }

    return e;
}

/*
 * Expression
 */
expr_t *
expr_op(expr_t *e0, expr_t *e1, op_type_t type)
{
    expr_t *e;
    op_t *op;

    op = malloc(sizeof(op_t));
    if ( NULL == op ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    op->type = type;
    op->fix = FIX_INFIX;
    op->e0 = e0;
    op->e1 = e1;

    e = malloc(sizeof(expr_t));
    if ( NULL == e ) {
        free(op);
        COMPILER_ERROR(EXIT_FAILURE);
    }
    e->type = EXPR_OP;
    e->u.op = op;

    return e;
}


/* Evaluator for testing */
typedef enum {
    VAL_INT,
} val_type_t;
typedef struct {
    val_type_t type;
    union {
        int i;
    } u;
} val_t;


val_t * eval_expr(expr_t *);

val_t *
eval_literal(literal_t *lit)
{
    val_t *v;

    v = malloc(sizeof(val_t));
    if ( NULL == v ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    v->type = VAL_INT;
    v->u.i = lit->u.i;

    return v;
}

val_t *
eval_id(char *id)
{
    val_t *v;

    v = malloc(sizeof(val_t));
    if ( NULL == v ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    v->type = VAL_INT;
    v->u.i = 0;

    return v;
}

val_t *
eval_op(op_t *op)
{
    val_t *v0;
    val_t *v1;
    val_t *v;

    v = malloc(sizeof(val_t));
    if ( NULL == v ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }

    switch ( op->fix ) {
    case FIX_INFIX:
        v0 = eval_expr(op->e0);
        v1 = eval_expr(op->e1);
        switch ( op->type ) {
        case OP_ADD:
            v->type = VAL_INT;
            v->u.i = v0->u.i + v1->u.i;
            break;
        case OP_SUB:
            v->type = VAL_INT;
            v->u.i = v0->u.i - v1->u.i;
            break;
        case OP_MUL:
            v->type = VAL_INT;
            v->u.i = v0->u.i * v1->u.i;
            break;
        case OP_DIV:
            v->type = VAL_INT;
            v->u.i = v0->u.i / v1->u.i;
            break;
        }
        break;
    case FIX_PREFIX:
        v0 = eval_expr(op->e0);
        memcpy(v, v0, sizeof(val_t));
        break;
    }

    return v;
}
val_t *
eval_expr(expr_t *e)
{
    val_t *v;

    v = NULL;
    switch ( e->type ) {
    case EXPR_LITERAL:
        v = eval_literal(e->u.lit);
        break;
    case EXPR_ID:
        v = eval_id(e->u.id);
        break;
    case EXPR_OP:
        v = eval_op(e->u.op);
        break;
    default:
        COMPILER_ERROR(EXIT_FAILURE);
    }

    return v;
}

void
debug(expr_t *e)
{
    val_t *v;

    v = eval_expr(e);
    printf(">> %d\n", v->u.i);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
