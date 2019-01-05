/*_
 * Copyright (c) 2018-2019 Hirochika Asai <asai@jar.jp>
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
 * Statement list
 */
stmt_list_t *
stmt_list_append(stmt_list_t *sl, stmt_t *stmt)
{
    if ( NULL == sl ) {
        sl = malloc(sizeof(stmt_list_t));
        if ( NULL == sl ) {
            COMPILER_ERROR(EXIT_FAILURE);
        }
        sl->head = NULL;
        sl->tail = NULL;
    }

    stmt->next = NULL;

    if ( NULL == sl->tail ) {
        sl->head = stmt;
        sl->tail = stmt;
    } else {
        sl->tail->next = stmt;
        sl->tail = stmt;
    }

    return sl;
}

/*
 * Statement
 */
stmt_t *
stmt_def(char *id, expr_t *e)
{
    stmt_t *stmt;

    stmt = malloc(sizeof(stmt_t));
    if ( NULL == stmt ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    stmt->type = STMT_DEF;
    stmt->u.def.id = strdup(id);
    if ( NULL == stmt->u.def.id ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    stmt->u.def.e = e;
    stmt->next = NULL;

    return stmt;
}

/*
 * Expression statement
 */
stmt_t *
stmt_expr(expr_t *e)
{
    stmt_t *stmt;

    stmt = malloc(sizeof(stmt_t));
    if ( NULL == stmt ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    stmt->type = STMT_EXPR;
    stmt->u.expr = e;
    stmt->next = NULL;

    return stmt;
}

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

/*
 * Variable
 */
typedef struct {
    char *id;
    val_t val;
} var_t;

/*
 * Context
 */
typedef struct {
    int nvars;
    var_t vars[MAX_VARS];
} ctx_t;

val_t * eval_expr(ctx_t *, expr_t *);

static var_t *
_find_var(ctx_t *ctx, char *id)
{
    int i;
    for ( i = 0; i < ctx->nvars; i++ ) {
        if ( 0 == strcmp(ctx->vars[i].id, id) ) {
            return &ctx->vars[i];
        }
    }

    return NULL;
}

static void
_update_var(ctx_t *ctx, char *id, val_t *val)
{
    var_t *var;

    var = _find_var(ctx, id);
    if ( NULL == var ) {
        if ( ctx->nvars >= MAX_VARS ) {
            COMPILER_ERROR(EXIT_FAILURE);
        }
        var = &ctx->vars[ctx->nvars];
        var->id = strdup(id);
        if ( NULL == var->id ) {
            COMPILER_ERROR(EXIT_FAILURE);
        }
        ctx->nvars++;
    }
    memcpy(&var->val, val, sizeof(val_t));
}

val_t *
eval_literal(ctx_t *ctx, literal_t *lit)
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
eval_id(ctx_t *ctx, char *id)
{
    val_t *v;
    var_t *var;

    var = _find_var(ctx, id);
    if ( NULL == var ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }

    v = malloc(sizeof(val_t));
    if ( NULL == v ) {
        COMPILER_ERROR(EXIT_FAILURE);
    }
    memcpy(v, &var->val, sizeof(val_t));
    //v->type = VAL_INT;
    //v->u.i = 0;

    return v;
}

val_t *
eval_op(ctx_t *ctx, op_t *op)
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
        v0 = eval_expr(ctx, op->e0);
        v1 = eval_expr(ctx, op->e1);
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
        v0 = eval_expr(ctx, op->e0);
        memcpy(v, v0, sizeof(val_t));
        break;
    }

    return v;
}
val_t *
eval_expr(ctx_t *ctx, expr_t *e)
{
    val_t *v;

    v = NULL;
    switch ( e->type ) {
    case EXPR_LITERAL:
        v = eval_literal(ctx, e->u.lit);
        break;
    case EXPR_ID:
        v = eval_id(ctx, e->u.id);
        break;
    case EXPR_OP:
        v = eval_op(ctx, e->u.op);
        break;
    default:
        COMPILER_ERROR(EXIT_FAILURE);
    }

    return v;
}

void
eval_stmt_def(ctx_t *ctx, stmt_def_t *def)
{
    val_t *v;

    v = eval_expr(ctx, def->e);
    _update_var(ctx, def->id, v);
}

void
debug(stmt_list_t *sl)
{
    ctx_t ctx;
    stmt_t *stmt;
    val_t *v;

    memset(&ctx, 0, sizeof(ctx_t));

    if ( NULL == sl ) {
        return;
    }
    stmt = sl->head;

    while ( NULL != stmt ) {
        switch ( stmt->type ) {
        case STMT_EXPR:
            v = eval_expr(&ctx, stmt->u.expr);
            printf(">> %d\n", v->u.i);
            break;
        case STMT_DEF:
            eval_stmt_def(&ctx, &stmt->u.def);
            break;
        }
        stmt = stmt->next;
    }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
