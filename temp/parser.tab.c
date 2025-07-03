/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser.y"

#define YYDEBUG 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaração de funções do Flex
extern int yylex();
extern int yylineno;
extern FILE *yyin; // Entrada do Flex

// Variáveis globais para a geração do código MASPY
FILE *maspy_output_file;
int agent_class_counter = 0; // Para gerar Agente1, Agente2, etc.
// Armazenar nomes dos agentes para gerar o rodapé dinamicamente
char *agent_names[100]; // Suporta até 100 agentes
int num_agents = 0;

// ====================================================================
// VARIÁVEIS E FUNÇÕES PARA GERENCIAMENTO GLOBAL DE MEMÓRIA (Corrigido)
// ====================================================================
#define MAX_ALLOCATED_STRINGS 2000
char *global_allocated_strings[MAX_ALLOCATED_STRINGS];
int global_num_allocated_strings = 0;

void register_global_allocated_string(char *str) {
    if (str && global_num_allocated_strings < MAX_ALLOCATED_STRINGS) {
        global_allocated_strings[global_num_allocated_strings++] = str;
    } else if (str) {
        fprintf(stderr, "Aviso: Limite de strings alocadas globalmente atingido. Memória para '%s' pode vazar.\n", str);
    }
}

void cleanup_global_allocated_strings() {
    for (int i = 0; i < global_num_allocated_strings; i++) {
        if (global_allocated_strings[i] != NULL) {
            free(global_allocated_strings[i]);
            global_allocated_strings[i] = NULL;
        }
    }
    global_num_allocated_strings = 0;
}
// ====================================================================

// Declaração de funções de erro do Bison
void yyerror(const char *s);

// Funções auxiliares para gerar código MASPY
void generate_maspy_header();
void generate_agent_class_start(const char *agent_json_name);
void generate_agent_class_end();
void generate_belief_maspy(const char *belief_name);
void generate_goal_maspy(const char *goal_name);
void generate_plan_start_maspy(const char *trigger_name, const char *ctx_name, const char *func_name_in_maspy, int is_empty_body, const char *plan_body_str);
void generate_maspy_footer();

// Funções para manipulação de strings (Corrigido)
char* clean_for_python_id(const char* str) {
    if (!str) return NULL;
    char* cleaned = strdup(str); // ALOCA MEMÓRIA
    if (!cleaned) return NULL;

    register_global_allocated_string(cleaned); // REGISTRA ESTA ALOCAÇÃO

    for (int i = 0; cleaned[i] != '\0'; i++) {
        if (cleaned[i] == ' ') cleaned[i] = '_';
    }
    return cleaned;
}


#line 143 "temp/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_OBRACE = 3,                     /* OBRACE  */
  YYSYMBOL_CBRACE = 4,                     /* CBRACE  */
  YYSYMBOL_OBRACKET = 5,                   /* OBRACKET  */
  YYSYMBOL_CBRACKET = 6,                   /* CBRACKET  */
  YYSYMBOL_COLON = 7,                      /* COLON  */
  YYSYMBOL_COMMA = 8,                      /* COMMA  */
  YYSYMBOL_AGENT_CODE = 9,                 /* AGENT_CODE  */
  YYSYMBOL_BELIEFS = 10,                   /* BELIEFS  */
  YYSYMBOL_GOAL = 11,                      /* GOAL  */
  YYSYMBOL_PLANS = 12,                     /* PLANS  */
  YYSYMBOL_TRIGGER = 13,                   /* TRIGGER  */
  YYSYMBOL_CTX = 14,                       /* CTX  */
  YYSYMBOL_BODY = 15,                      /* BODY  */
  YYSYMBOL_STRING_LITERAL = 16,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 17,                  /* $accept  */
  YYSYMBOL_program = 18,                   /* program  */
  YYSYMBOL_agent_definitions = 19,         /* agent_definitions  */
  YYSYMBOL_agent_definition = 20,          /* agent_definition  */
  YYSYMBOL_agent_name = 21,                /* agent_name  */
  YYSYMBOL_agent_details = 22,             /* agent_details  */
  YYSYMBOL_belief_list = 23,               /* belief_list  */
  YYSYMBOL_goal_definition = 24,           /* goal_definition  */
  YYSYMBOL_string_list = 25,               /* string_list  */
  YYSYMBOL_plan_object = 26,               /* plan_object  */
  YYSYMBOL_plan_definitions = 27,          /* plan_definitions  */
  YYSYMBOL_plan_definition = 28,           /* plan_definition  */
  YYSYMBOL_plan_json_name = 29,            /* plan_json_name  */
  YYSYMBOL_plan_details = 30,              /* plan_details  */
  YYSYMBOL_trigger_val = 31,               /* trigger_val  */
  YYSYMBOL_ctx_val = 32,                   /* ctx_val  */
  YYSYMBOL_body_elements = 33,             /* body_elements  */
  YYSYMBOL_body_item_list = 34,            /* body_item_list  */
  YYSYMBOL_body_item = 35                  /* body_item  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   54

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  17
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  25
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  70

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   271


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    98,    98,   108,   109,   114,   123,   140,   152,   157,
     161,   163,   164,   169,   174,   175,   180,   188,   193,   212,
     217,   222,   228,   241,   244,   257
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "OBRACE", "CBRACE",
  "OBRACKET", "CBRACKET", "COLON", "COMMA", "AGENT_CODE", "BELIEFS",
  "GOAL", "PLANS", "TRIGGER", "CTX", "BODY", "STRING_LITERAL", "$accept",
  "program", "agent_definitions", "agent_definition", "agent_name",
  "agent_details", "belief_list", "goal_definition", "string_list",
  "plan_object", "plan_definitions", "plan_definition", "plan_json_name",
  "plan_details", "trigger_val", "ctx_val", "body_elements",
  "body_item_list", "body_item", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-29)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -2,    -7,     3,    -3,   -29,     2,   -10,   -29,     4,    -1,
       5,     7,   -10,     6,   -29,   -29,     8,   -29,     9,    10,
       1,    11,    12,    15,    13,     1,   -29,    16,   -29,    14,
     -29,    17,    19,    20,    23,    18,    24,   -29,    25,    27,
      26,   -29,   -29,    18,    29,   -29,     0,   -29,    30,    22,
     -29,    28,    31,    32,    33,   -29,    34,    35,    36,    39,
      -6,    37,   -29,   -29,    40,    43,   -29,   -29,    38,   -29
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     6,     0,     3,
       0,     0,     0,     0,     2,     4,     0,     5,     0,     0,
      10,     0,    11,     0,     0,    10,     8,     0,    12,     0,
       9,     0,     0,     0,     0,     0,     0,    17,     0,    14,
       0,     7,    13,     0,     0,    15,     0,    16,     0,     0,
      19,     0,     0,     0,     0,    20,     0,     0,     0,     0,
       0,     0,    21,    25,     0,    23,    18,    22,     0,    24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -29,   -29,    41,   -29,   -29,   -29,   -29,   -29,   -11,   -29,
     -21,   -29,   -29,   -29,   -29,   -29,   -29,   -28,   -29
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     8,     9,    10,    17,    21,    31,    23,    36,
      38,    39,    40,    47,    51,    56,    61,    64,    65
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      62,     1,     3,     4,     5,     6,     7,    12,    11,    16,
      63,    14,    13,    48,    28,    20,    19,    22,    18,    24,
      25,    26,    45,    29,    27,    32,    35,    34,    41,    42,
      30,    33,    46,    44,    37,    43,    52,    49,    50,    54,
      69,    66,    57,    59,    60,    53,    67,     0,     0,    55,
      58,    68,     0,    15,    63
};

static const yytype_int8 yycheck[] =
{
       6,     3,     9,     0,     7,     3,    16,     8,     4,     3,
      16,     4,     7,    13,    25,     5,     7,    16,    10,     8,
       8,     6,    43,     7,    11,     8,     3,     7,     4,     4,
      16,    12,     3,     7,    16,     8,     8,     7,    16,     7,
      68,     4,     8,     7,     5,    14,     6,    -1,    -1,    16,
      15,     8,    -1,    12,    16
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    18,     9,     0,     7,     3,    16,    19,    20,
      21,     4,     8,     7,     4,    19,     3,    22,    10,     7,
       5,    23,    16,    25,     8,     8,     6,    11,    25,     7,
      16,    24,     8,    12,     7,     3,    26,    16,    27,    28,
      29,     4,     4,     8,     7,    27,     3,    30,    13,     7,
      16,    31,     8,    14,     7,    16,    32,     8,    15,     7,
       5,    33,     6,    16,    34,    35,     4,     6,     8,    34
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    17,    18,    19,    19,    20,    21,    22,    23,    24,
      25,    25,    25,    26,    27,    27,    28,    29,    30,    31,
      32,    33,    33,    34,    34,    35
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     7,     1,     3,     3,     1,    13,     3,     1,
       0,     1,     3,     3,     1,     3,     3,     1,    13,     1,
       1,     2,     3,     1,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: OBRACE AGENT_CODE COLON OBRACE agent_definitions CBRACE CBRACE  */
#line 99 "src/parser.y"
    {
        // A liberação final de memória será feita por cleanup_global_allocated_strings()
        // generate_maspy_footer() é chamado antes do cleanup_global_allocated_strings
        // para que os nomes dos agentes ainda estejam disponíveis.
    }
#line 1199 "temp/parser.tab.c"
    break;

  case 5: /* agent_definition: agent_name COLON agent_details  */
#line 115 "src/parser.y"
    {
        // REMOVIDO: free($<str_val>1);
        // Gerenciamento de memória agora é feito globalmente.
    }
#line 1208 "temp/parser.tab.c"
    break;

  case 6: /* agent_name: STRING_LITERAL  */
#line 124 "src/parser.y"
    {
        (yyval.str_val) = (yyvsp[0].str_val); // Passa a string literal para o símbolo. O gerenciador global a liberará.
        generate_agent_class_start((yyvsp[0].str_val));
        if (num_agents < 100) {
            char *agent_name_copy = strdup((yyvsp[0].str_val)); // Cria uma CÓPIA para a lista agent_names
            register_global_allocated_string(agent_name_copy); // REGISTRA A CÓPIA
            agent_names[num_agents] = agent_name_copy;
            num_agents++;
        } else {
            fprintf(stderr, "Aviso: Limite de agentes no rodapé atingido (100). Não será possível adicionar %s.\n", (yyvsp[0].str_val));
        }
    }
#line 1225 "temp/parser.tab.c"
    break;

  case 7: /* agent_details: OBRACE BELIEFS COLON belief_list COMMA GOAL COLON goal_definition COMMA PLANS COLON plan_object CBRACE  */
#line 145 "src/parser.y"
    {
        generate_agent_class_end(); // Termina a definição da classe do agente
    }
#line 1233 "temp/parser.tab.c"
    break;

  case 9: /* goal_definition: STRING_LITERAL  */
#line 157 "src/parser.y"
                   { generate_goal_maspy((yyvsp[0].str_val)); /* REMOVIDO: free($1); */ }
#line 1239 "temp/parser.tab.c"
    break;

  case 11: /* string_list: STRING_LITERAL  */
#line 163 "src/parser.y"
                   { generate_belief_maspy((yyvsp[0].str_val)); /* REMOVIDO: free($1); */ }
#line 1245 "temp/parser.tab.c"
    break;

  case 12: /* string_list: STRING_LITERAL COMMA string_list  */
#line 164 "src/parser.y"
                                     { generate_belief_maspy((yyvsp[-2].str_val)); /* REMOVIDO: free($1); */ }
#line 1251 "temp/parser.tab.c"
    break;

  case 16: /* plan_definition: plan_json_name COLON plan_details  */
#line 181 "src/parser.y"
    {
        /* REMOVIDO: free($<str_val>1); */
    }
#line 1259 "temp/parser.tab.c"
    break;

  case 17: /* plan_json_name: STRING_LITERAL  */
#line 188 "src/parser.y"
                   { (yyval.str_val) = (yyvsp[0].str_val); }
#line 1265 "temp/parser.tab.c"
    break;

  case 18: /* plan_details: OBRACE TRIGGER COLON trigger_val COMMA CTX COLON ctx_val COMMA BODY COLON body_elements CBRACE  */
#line 198 "src/parser.y"
    {
        // $12 agora é o resultado da regra body_elements (a estrutura body_result_t)
        generate_plan_start_maspy(
            (yyvsp[-9].str_val),                        // CORRIGIDO: trigger_val é o 4º símbolo da regra
            (yyvsp[-5].str_val),                        // CORRIGIDO: ctx_val é o 8º símbolo da regra
            (yyvsp[-9].str_val),                        // func_name_in_maspy (nome do trigger) é o 4º símbolo
            (yyvsp[-1].body_result_t).is_empty,       // Flag se o corpo está vazio
            (yyvsp[-1].body_result_t).content         // A string com o conteúdo do corpo
        );
    }
#line 1280 "temp/parser.tab.c"
    break;

  case 19: /* trigger_val: STRING_LITERAL  */
#line 212 "src/parser.y"
                   { (yyval.str_val) = (yyvsp[0].str_val); }
#line 1286 "temp/parser.tab.c"
    break;

  case 20: /* ctx_val: STRING_LITERAL  */
#line 217 "src/parser.y"
                   { (yyval.str_val) = (yyvsp[0].str_val); }
#line 1292 "temp/parser.tab.c"
    break;

  case 21: /* body_elements: OBRACKET CBRACKET  */
#line 223 "src/parser.y"
    {
        (yyval.body_result_t).content = strdup(""); // Conteúdo é uma string vazia
        register_global_allocated_string((yyval.body_result_t).content);
        (yyval.body_result_t).is_empty = 1; // Flag indica que é vazio
    }
#line 1302 "temp/parser.tab.c"
    break;

  case 22: /* body_elements: OBRACKET body_item_list CBRACKET  */
#line 229 "src/parser.y"
    {
        (yyval.body_result_t).content = (yyvsp[-1].plan_body_content); // $2 é o conteúdo acumulado de body_item_list
        (yyval.body_result_t).is_empty = 0; // Flag indica que NÃO é vazio
    }
#line 1311 "temp/parser.tab.c"
    break;

  case 23: /* body_item_list: body_item  */
#line 241 "src/parser.y"
              {
        (yyval.plan_body_content) = (yyvsp[0].plan_body_content); // Passa a string do único item
    }
#line 1319 "temp/parser.tab.c"
    break;

  case 24: /* body_item_list: body_item COMMA body_item_list  */
#line 244 "src/parser.y"
                                   {
        // Concatena a string do item atual com o restante da lista
        char *combined_str = (char *)malloc(strlen((yyvsp[-2].plan_body_content)) + strlen((yyvsp[0].plan_body_content)) + 1);
        if (combined_str) {
            strcpy(combined_str, (yyvsp[-2].plan_body_content));
            strcat(combined_str, (yyvsp[0].plan_body_content));
            register_global_allocated_string(combined_str);
        }
        (yyval.plan_body_content) = combined_str;
    }
#line 1334 "temp/parser.tab.c"
    break;

  case 25: /* body_item: STRING_LITERAL  */
#line 257 "src/parser.y"
                   {
        char buffer[512]; // Buffer temporário para formatar a string
        int len;
        char *generated_line = NULL;

        if (strncmp((yyvsp[0].str_val), "A_", 2) == 0) {
            char *action_name = (yyvsp[0].str_val) + 2; // Extrai o nome da ação (removendo "A_")
            // AGORA GERA UMA LINHA DE PRINT ANTES DA EXECUÇÃO DA AÇÃO
            len = snprintf(buffer, sizeof(buffer),"        self.print(\"Executando Ação: %s\")\n", //"        self.do(Action(\"%s\"))\n",
                           action_name); // action_name é usado para os dois %s.

        } else if (strncmp((yyvsp[0].str_val), "G_", 2) == 0) {
            // Pode adicionar um print similar para Goal se quiser ver quando um Goal é adicionado
            len = snprintf(buffer, sizeof(buffer), "        self.add(Goal(\"%s\"))\n", (yyvsp[0].str_val) + 2);
        } else if (strncmp((yyvsp[0].str_val), "B_", 2) == 0) {
            // Pode adicionar um print similar para Belief se quiser ver quando uma Belief é adicionada
            len = snprintf(buffer, sizeof(buffer), "        self.add(Belief(\"%s\"))\n", (yyvsp[0].str_val) + 2);
        } else {
            fprintf(stderr, "Erro na linha %d: Elemento de body inesperado: '%s'\n", yylineno, (yyvsp[0].str_val));
            len = snprintf(buffer, sizeof(buffer), "        # Erro: Elemento de body inesperado: \"%s\"\n", (yyvsp[0].str_val));
        }

        generated_line = strdup(buffer); // Aloca memória para a linha combinada
        register_global_allocated_string(generated_line); // Registra para limpeza
        (yyval.plan_body_content) = generated_line; // Passa a string gerada para o símbolo
    }
#line 1365 "temp/parser.tab.c"
    break;


#line 1369 "temp/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 285 "src/parser.y"


// Implementações das funções auxiliares para geração de código MASPY

void generate_maspy_header() {
    fprintf(maspy_output_file, "#\n");
    fprintf(maspy_output_file, "# Trabalho de Compiladores\n");
    fprintf(maspy_output_file, "# tradutor: json2maspy\n");
    fprintf(maspy_output_file, "#\n");
    fprintf(maspy_output_file, "from maspy import *\n\n");
}

void generate_agent_class_start(const char *agent_json_name) {
    agent_class_counter++;
    fprintf(maspy_output_file, "class Agente%d(Agent):\n", agent_class_counter);
    fprintf(maspy_output_file, "    def __init__(self, agt_name):\n");
    fprintf(maspy_output_file, "        super().__init__(agt_name)\n");
    fprintf(maspy_output_file, "        # Original JSON Agent Name: %s\n", agent_json_name);
}

void generate_agent_class_end() {
    fprintf(maspy_output_file, "\n"); // Adiciona uma linha em branco para espaçamento
}

void generate_belief_maspy(const char *belief_name) {
    fprintf(maspy_output_file, "        self.add(Belief(\"%s\"))\n", belief_name);
}

void generate_goal_maspy(const char *goal_name) {
    fprintf(maspy_output_file, "        self.add(Goal(\"%s\"))\n", goal_name);
}

// DEFINIÇÃO ATUALIZADA: Agora recebe a string do corpo do plano
void generate_plan_start_maspy(const char *trigger_name, const char *ctx_name, const char *func_name_in_maspy, int is_empty_body_flag, const char *plan_body_str) {
    char *clean_func_name = clean_for_python_id(func_name_in_maspy);

    fprintf(maspy_output_file, "    @pl(gain, Goal(\"%s\"), Belief(\"%s\"))\n", trigger_name, ctx_name);
    fprintf(maspy_output_file, "    def %s_(self, src):\n", clean_func_name);

    if (is_empty_body_flag) { // Se o corpo estiver vazio
        fprintf(maspy_output_file, "        pass\n");
    } else {
        // Imprime a string do corpo acumulado, que já contém a indentação
        fprintf(maspy_output_file, "%s", plan_body_str);
    }
    // clean_func_name é gerenciado globalmente pela função clean_for_python_id
}

void generate_maspy_footer() {
    fprintf(maspy_output_file, "\n# Configuração do Ambiente e Agentes\n");
    // Cria uma instância de um ambiente genérico. O nome "MeuAmbienteGeral" é arbitrário.
    fprintf(maspy_output_file, "my_env = Environment(\"MeuAmbienteGeral\")\n\n");

    // Itera sobre os agentes registrados e gera o código para instanciá-los e conectá-los ao ambiente
    for (int i = 0; i < num_agents; i++) {
        // Gera um nome de variável Python limpo para o agente (ex: agent_bob, agent_alice)
        char *python_agent_var_name = clean_for_python_id(agent_names[i]);
        
        // Instancia o agente (ex: agent_bob = Agente1("bob"))
        fprintf(maspy_output_file, "%s = Agente%d(\"%s\")\n", python_agent_var_name, i + 1, agent_names[i]);
        
        // Conecta o agente ao ambiente (ex: agent_bob.connect_env(my_env))
        fprintf(maspy_output_file, "%s.connect_to(my_env)\n\n", python_agent_var_name); // CORRIGIDO: método 'connect_to'
    }
    // Inicia o sistema MASPY
    fprintf(maspy_output_file, "Admin().start_system()\n");
}

// Função principal que abre os arquivos e chama o parser
int main(int argc, char **argv) {

    //#ifdef YYDEBUG
    //yydebug = 1;
    //#endif

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_json_entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo de entrada JSON
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir arquivo JSON de entrada");
        return 1;
    }

    // Abre o arquivo de saída MASPY (.py)
    maspy_output_file = fopen("maspy_output.py", "w");
    if (!maspy_output_file) {
        perror("Erro ao criar arquivo MASPY de saída (maspy_output.py)");
        fclose(yyin);
        return 1;
    }

    generate_maspy_header(); // Gera o cabeçalho do arquivo MASPY

    yyparse(); // Inicia a análise sintática

    generate_maspy_footer(); // Gera o rodapé do arquivo MASPY (precisa dos nomes dos agentes)

    cleanup_global_allocated_strings(); // NOVO E CRUCIAL: Libera TODA a memória alocada

    fclose(yyin); // Fecha o arquivo de entrada
    fclose(maspy_output_file); // Fecha o arquivo de saída
    return 0;
}

// Função de tratamento de erros do Bison
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", yylineno, s);
}
