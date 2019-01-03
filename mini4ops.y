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

/* Headers and data type definitions */
%{
#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
int yylex();
int yyerror(char const *);
stmt_list_t *stmts;
%}
%union {
    int intval;
    char *idval;
    void *expr;
    void *stmt;
    void *stmt_list;
}
%token <intval>         TOK_LIT_INT
%token <idval>          TOK_ID
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_DEF TOK_NEWLINE
//                      %type <intval>
%type <stmt_list> statement_list
%type <stmt> statement def_stmt
%type <expr> expression a_expr m_expr primary
%locations
%%
/* Syntax and parser */
file:           statement_list
                {
                    stmts = $1;
                }
                ;
statement_list: statement
                {
                    $$ = stmt_list_append(NULL, $1);
                }
        |       statement_list statement
                {
                    $$ = stmt_list_append($1, $2);
                }
                ;
statement:      def_stmt TOK_NEWLINE
        |       expression TOK_NEWLINE
                {
                    $$ = stmt_expr($1);
                }
                ;
def_stmt:       TOK_ID TOK_DEF expression
                {
                    $$ = stmt_def($1, $3);
                }
                ;
expression:     a_expr
                ;
a_expr:         a_expr TOK_ADD m_expr
                {
                    $$ = expr_op($1, $3, OP_ADD);
                }
        |       a_expr TOK_SUB m_expr
                {
                    $$ = expr_op($1, $3, OP_SUB);
                }
        |       m_expr
                ;
m_expr:         m_expr TOK_MUL primary
                {
                    $$ = expr_op($1, $3, OP_MUL);
                }
        |       m_expr TOK_DIV primary
                {
                    $$ = expr_op($1, $3, OP_DIV);
                }
        |       primary
                ;
primary:        TOK_LIT_INT
                {
                    $$ = expr_lit(literal_int($1));
                }
        |       TOK_ID
                {
                    $$ = expr_id($1);
                }
                ;
%%

/*
 * Print usage and exit
 */
void
usage(const char *prog)
{
    fprintf(stderr, "Usage: %s <file>\n", prog);
    exit(EXIT_FAILURE);
}

/*
 * Error handler
 */
int
yyerror(char const *str)
{
    extern char *yytext;

    fprintf(stderr, "Parser error near %s (Line: %d)\n", yytext,
            yylloc.first_line);

    return 0;
}

/*
 * Main routine
 */
int
main(int argc, const char *const argv[])
{
    extern int yyparse(void);
    extern FILE *yyin;

    if ( argc < 2 ) {
        yyin = stdin;
        /* stdio is not supported. */
        usage(argv[0]);
    } else {
        /* Open the specified file */
        yyin = fopen(argv[1], "r");
        if ( NULL == yyin ) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
    }
    /* Parse the input file */
    stmts = NULL;
    if ( yyparse() ) {
        fprintf(stderr, "Parse error!\n");
        exit(EXIT_FAILURE);
    }
    printf("stmts=%p\n", stmts);
    debug(stmts);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
