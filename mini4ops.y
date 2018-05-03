%{
#include <stdio.h>
#include <stdlib.h>
%}
%union {
    int intval;
}
%token <intval>         LIT_INT
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_NEWLINE
%type <intval> expression a_expr m_expr primary
%%
statement_list: statement
        |       statement_list statement
                ;
statement:      expression TOK_NEWLINE
                {
                    printf(">> %d\n", $1);
                }
                ;
expression:     a_expr
                ;
a_expr:         m_expr TOK_ADD a_expr
                {
                    $$ = $1 + $3;
                }
        |       m_expr TOK_SUB a_expr
                {
                    $$ = $1 - $3;
                }
        |       m_expr
                ;
m_expr:         primary TOK_MUL m_expr
                {
                    $$ = $1 * $3;
                }
        |       primary TOK_DIV m_expr
                {
                    $$ = $1 / $3;
                }
        |       primary
                ;
primary:        LIT_INT
                ;
%%
int
yyerror(char const *str)
{
    extern char *yytext;
    fprintf(stderr, "parser error near %s\n", yytext);
    return 0;
}

int main(void)
{
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = stdin;
    if (yyparse()) {
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
}
