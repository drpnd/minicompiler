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

#define MAX_VARS    256

/*
 * Operations
 */
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
} op_type_t;

/*
 * Type of fixes
 */
typedef enum {
    FIX_INFIX,
    FIX_PREFIX,
} fix_t;

/*
 * Expression type
 */
typedef enum {
    EXPR_LITERAL,
    EXPR_ID,
    EXPR_OP,
} expr_type_t;

typedef struct _expr expr_t;

/*
 * Literal
 */
typedef enum {
    LITERAL_INT,
} literal_type_t;
typedef struct {
    literal_type_t type;
    union {
        int i;
    } u;
} literal_t;

/*
 * Operation
 */
typedef struct {
    op_type_t type;
    fix_t fix;
    expr_t *e0;
    expr_t *e1;
} op_t;

/*
 * Expression
 */
struct _expr {
    expr_type_t type;
    union {
        literal_t *lit;
        char *id;
        op_t *op;
    } u;
};

typedef enum {
    STMT_DEF,
    STMT_EXPR,
} stmt_type_t;

typedef struct {
    char *id;
    expr_t *e;
} stmt_def_t;

/*
 * Statement
 */
typedef struct stmt stmt_t;
struct stmt {
    stmt_type_t type;
    union {
        stmt_def_t def;
        expr_t *expr;
    } u;
    stmt_t *next;
};

/*
 * Statements
 */
typedef struct stmt_list stmt_list_t;
struct stmt_list {
    stmt_t *head;
    stmt_t *tail;
};

#define COMPILER_ERROR(err)    do {                             \
        fprintf(stderr, "Fatal error on compiling the code\n"); \
        exit(err);                                              \
    } while ( 0 )

#ifdef __cplusplus
extern "C" {
#endif

    stmt_list_t * stmt_list_append(stmt_list_t *, stmt_t *);
    literal_t * literal_int(int);
    stmt_t * stmt_def(char *, expr_t *);
    stmt_t * stmt_expr(expr_t *);
    expr_t * expr_lit(literal_t *);
    expr_t * expr_id(char *);
    expr_t * expr_op(expr_t *, expr_t *, op_type_t);
    void debug(stmt_list_t *);

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
