%{
#include <stdio.h>
#include "y.tab.h"

int
yywrap(void)
{
    return 1;
}
%}
%%
"+"     return TOK_ADD;
"-"     return TOK_SUB;
"*"     return TOK_MUL;
"/"     return TOK_DIV;
[\t ]   ;                       /* ignore */
"\n"    return TOK_NEWLINE;
[1-9][0-9]* {
    int val;
    sscanf(yytext, "%d", &val);
    yylval.intval = val;
    return LIT_INT;
}
%%
