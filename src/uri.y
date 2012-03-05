%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YYSTYPE char*

extern int yylineno;

void yyerror(char** scheme, char** host, char** port, char** path, const char *str)
{
}

int yywrap()
{
    return 1;
}

char * strconcat(const char * lhs, const char* rhs)
{
    char * result = (char*)malloc(strlen(lhs) + strlen(rhs) + 1);

    strcpy(result, lhs);
    strcat(result, rhs);

    return result;
}

%}

%token NUMBER IDENTIFIER TAB COLON SLASH PERCENT QUESTION AT AND EQUAL HASH DOT
%glr-parser

%parse-param {char **scheme}
%parse-param {char **host}
%parse-param {char **port}
%parse-param {char **path}

%%

uri: scheme host portinfo optional_path optional_query fragment {
    char* pathquery = strconcat($4, $5);
    char* complete_path = strconcat(pathquery, $6);
    *path = complete_path;
    free(pathquery);
    free($4);
    free($5);
    free($6);
};

scheme: /* empty */ {*scheme = strdup("http");}
        |
        IDENTIFIER COLON SLASH SLASH {*scheme = strdup($1);}
        ;

host: ipv4 {*host = strdup($1);}
      |
      IDENTIFIER {*host = strdup($1);}
      ;

portinfo: /* empty */ {*port = strdup("80");}
          |
          COLON port {*port = strdup($2);}
          ;

port: NUMBER {$$ = $1}
      ;

optional_path: /*empty*/ {$$ = strdup("/");}
               |
               path { $$ = $1;}
               ;

path: path SLASH IDENTIFIER {
          char *pathslash = strconcat($1, "/");
          $$ = strconcat(pathslash, $3);
          free($1);
          free(pathslash);
      }
      |
      SLASH IDENTIFIER {$$ = strconcat("/", $2);}
      ;

optional_query: /*empty*/ { $$ = strdup("");}
                |
                query { $$ = $1; }
                ;

query: QUESTION {$$ = strdup("?");}
       |
       QUESTION key_value_pairs {
           $$ = strconcat("?", $2);
           free($2);
       }
       ;

key_value_pairs: key_value {$$=$1}
                 |
                 key_value_pairs AND key_value {
                     char* kvpand = strconcat($1, "&");
                     $$ = strconcat(kvpand, $3);
                     free($1);
                     free(kvpand);
                     free($3);
                 }
                 ;

key_value: key EQUAL value {
                     char* keyequal = strconcat($1, "=");
                     $$ = strconcat(keyequal, $3);
                     free($1);
                     free(keyequal);
                     free($3);
                 }
                 ;

key: IDENTIFIER {$$ = strdup($1);}
     ;

value: IDENTIFIER {$$ = strdup($1);}
       |
       NUMBER {$$ = strdup($1);}
       ;

fragment: /* empty */ {$$ = strdup("");}
          |
          HASH IDENTIFIER {$$ = strconcat("#", $2);}
          |
          HASH NUMBER {$$ = strconcat("#", $2);}
          ;

ipv4: NUMBER DOT NUMBER DOT NUMBER DOT NUMBER
      ;


/* end */
