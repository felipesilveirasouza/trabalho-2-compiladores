%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylineno;

extern void register_global_allocated_string(char *str);

char *strip_quotes(char *str) {
    char *result_str;

    if (str && strlen(str) > 1 && str[0] == '"' && str[strlen(str)-1] == '"') {
        result_str = (char *)malloc(strlen(str) - 2 + 1);
        if (result_str) {
            strncpy(result_str, str + 1, strlen(str) - 2);
            result_str[strlen(str) - 2] = '\0';
        }
    } else {
        result_str = strdup(str);
    }

    if (result_str) {
        register_global_allocated_string(result_str);
    }
    return result_str;
}

%}

%option noyywrap
%option yylineno

%%

"\"agent_code\""    { return AGENT_CODE; }
"\"beliefs\""       { return BELIEFS; }
"\"goal\""          { return GOAL; }
"\"plans\""         { return PLANS; }
"\"trigger\""       { return TRIGGER; }
"\"ctx\""           { return CTX; }
"\"body\""          { return BODY; }

\"([^"\\]|\\.)*\"   {
                        yylval.str_val = strip_quotes(yytext);
                        return STRING_LITERAL;
                    }

"{"                 { return OBRACE; }
"}"                 { return CBRACE; }
"["                 { return OBRACKET; }
"]"                 { return CBRACKET; }
":"                 { return COLON; }
","                 { return COMMA; }

[ \t\n\r]+          { }


.                   {
                      fprintf(stderr, "Erro léxico na linha %d: Caractere inesperado '%s'\n", yylineno, yytext);
                    }

%%