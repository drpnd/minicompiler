%{
#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
%}
%union {
    int intval;
    void *expr;
}
%token <intval>         LIT_INT
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_NEWLINE
//                      %type <intval>
%type <expr> expression a_expr m_expr primary
%%
statement_list: statement
        |       statement_list statement
                ;
statement:      expression TOK_NEWLINE
                {
                    debug($1);
                }
                ;
expression:     a_expr
                ;
a_expr:         m_expr TOK_ADD a_expr
                {
                    $$ = expr_op($1, $3, OP_ADD);
                }
        |       m_expr TOK_SUB a_expr
                {
                    $$ = expr_op($1, $3, OP_SUB);
                }
        |       m_expr
                ;
m_expr:         primary TOK_MUL m_expr
                {
                    $$ = expr_op($1, $3, OP_MUL);
                }
        |       primary TOK_DIV m_expr
                {
                    $$ = expr_op($1, $3, OP_DIV);
                }
        |       primary
                ;
primary:        LIT_INT
                {
                    $$ = expr_lit(literal_int($1));
                }
                ;
%%
int
yyerror(char const *str)
{
    extern char *yytext;
    fprintf(stderr, "Parser error near %s\n", yytext);
    return 0;
}

int main(void)
{
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = stdin;
    if ( yyparse() ) {
        fprintf(stderr, "Error!\n");
        exit(1);
    }
}
