/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_TEMP_PARSER_TAB_H_INCLUDED
# define YY_YY_TEMP_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    OBRACE = 258,                  /* OBRACE  */
    CBRACE = 259,                  /* CBRACE  */
    OBRACKET = 260,                /* OBRACKET  */
    CBRACKET = 261,                /* CBRACKET  */
    COLON = 262,                   /* COLON  */
    COMMA = 263,                   /* COMMA  */
    AGENT_CODE = 264,              /* AGENT_CODE  */
    BELIEFS = 265,                 /* BELIEFS  */
    GOAL = 266,                    /* GOAL  */
    PLANS = 267,                   /* PLANS  */
    TRIGGER = 268,                 /* TRIGGER  */
    CTX = 269,                     /* CTX  */
    BODY = 270,                    /* BODY  */
    STRING_LITERAL = 271           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 73 "src/parser.y"

    char *str_val;
    // int is_empty_body_flag; // REMOVER: Não precisamos mais de uma flag separada aqui
    char *plan_body_content; // Mantém este para body_item_list e body_item
    struct {
        char *content; // O conteúdo acumulado do corpo
        int is_empty;  // 1 se o corpo está vazio, 0 se não está
    } body_result_t; // NOVO: Estrutura para o resultado do corpo

#line 90 "temp/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_TEMP_PARSER_TAB_H_INCLUDED  */
