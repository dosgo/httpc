%{
#define YYSTYPE char*

#include <stdio.h>
#include "uri.tab.h"

%}
%%
[a-zA-Z_][a-zA-Z0-9_.+-]* {yylval = strdup(yytext); return IDENTIFIER;}
[0-9]+                 {yylval = strdup(yytext); return NUMBER;}
[\t]                   return TAB;
[:]                    return COLON;
[/]                    return SLASH;
[%]                    return PERCENT;
[?]                    return QUESTION;
[@]                    return AT;
[&]                    return AND;
[=]                    return EQUAL;
[#]                    return HASH;
[.]                    return DOT;
[ \n\r]+                /* ignore end of line */;
%%
