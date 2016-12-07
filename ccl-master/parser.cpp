/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     t_SEMICOLON = 258,
     t_INTEGER = 259,
     t_REAL = 260,
     t_EQUALS = 261,
     t_LCURLY = 262,
     t_RCURLY = 263,
     t_COMMA = 264,
     t_TRUE = 265,
     t_FALSE = 266,
     t_NOT = 267,
     t_LBBRACKET = 268,
     t_RBBRACKET = 269,
     t_LBRACKET = 270,
     t_RBRACKET = 271,
     t_HEAD = 272,
     t_TAIL = 273,
     t_CONCAT = 274,
     t_CONS = 275,
     t_DOT = 276,
     t_ASSIGN = 277,
     t_LAMBDA = 278,
     t_COLON = 279,
     t_STRCAT = 280,
     t_COMPOSE = 281,
     t_NEEDS = 282,
     t_VAR = 283,
     t_STRING = 284,
     t_OR = 285,
     t_AND = 286,
     t_LESS_THAN = 287,
     t_LESS_THAN_OR_EQUAL = 288,
     t_GREATER_THAN = 289,
     t_GREATER_THAN_OR_EQUAL = 290,
     t_EQUAL = 291,
     t_NOT_EQUAL = 292,
     t_MINUS = 293,
     t_MOD = 294,
     t_DIV = 295,
     t_POWER = 296,
     t_RECORD_ADD = 297,
     t_PLUS = 298,
     t_MULT = 299,
     t_LPAREN = 300,
     t_RPAREN = 301,
     t_APP = 302,
     t_LET = 303,
     t_IN = 304,
     t_FOREACH = 305,
     t_DO = 306,
     t_END = 307,
     t_IF = 308,
     t_THEN = 309,
     t_ELSE = 310,
     t_FUN = 311,
     t_EXTERNAL = 312,
     t_INTERNAL = 313,
     t_PROGRAM = 314,
     t_SHARING = 315,
     t_EXEC = 316,
     t_BACKQUOTE = 317,
     t_UNIT = 318,
     t_BOOL = 319,
     t_INT = 320,
     t_LIST = 321,
     t_VARARGS = 322,
     t_INTO = 323,
     t_MAP_TO_CELLS = 324,
     UNOT = 325,
     UMINUS = 326
   };
#endif
/* Tokens.  */
#define t_SEMICOLON 258
#define t_INTEGER 259
#define t_REAL 260
#define t_EQUALS 261
#define t_LCURLY 262
#define t_RCURLY 263
#define t_COMMA 264
#define t_TRUE 265
#define t_FALSE 266
#define t_NOT 267
#define t_LBBRACKET 268
#define t_RBBRACKET 269
#define t_LBRACKET 270
#define t_RBRACKET 271
#define t_HEAD 272
#define t_TAIL 273
#define t_CONCAT 274
#define t_CONS 275
#define t_DOT 276
#define t_ASSIGN 277
#define t_LAMBDA 278
#define t_COLON 279
#define t_STRCAT 280
#define t_COMPOSE 281
#define t_NEEDS 282
#define t_VAR 283
#define t_STRING 284
#define t_OR 285
#define t_AND 286
#define t_LESS_THAN 287
#define t_LESS_THAN_OR_EQUAL 288
#define t_GREATER_THAN 289
#define t_GREATER_THAN_OR_EQUAL 290
#define t_EQUAL 291
#define t_NOT_EQUAL 292
#define t_MINUS 293
#define t_MOD 294
#define t_DIV 295
#define t_POWER 296
#define t_RECORD_ADD 297
#define t_PLUS 298
#define t_MULT 299
#define t_LPAREN 300
#define t_RPAREN 301
#define t_APP 302
#define t_LET 303
#define t_IN 304
#define t_FOREACH 305
#define t_DO 306
#define t_END 307
#define t_IF 308
#define t_THEN 309
#define t_ELSE 310
#define t_FUN 311
#define t_EXTERNAL 312
#define t_INTERNAL 313
#define t_PROGRAM 314
#define t_SHARING 315
#define t_EXEC 316
#define t_BACKQUOTE 317
#define t_UNIT 318
#define t_BOOL 319
#define t_INT 320
#define t_LIST 321
#define t_VARARGS 322
#define t_INTO 323
#define t_MAP_TO_CELLS 324
#define UNOT 325
#define UMINUS 326




/* Copy the first part of user declarations.  */
#line 1 "../ccl-master/parser.y"


////////////////////////////////////////////////////////////////////////////
//
//   ccli : The Computation and Control Language Tools
//   -------------------------------------------------
//
//   Copyright (c) 2003 Eric Klavins, California Institute of Technology
//   For more information, email klavins@caltech.edu
//
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//

#include <unistd.h>
//#include <dlfcn.h>
#include <stdio.h>
#include <sstream>
//#include "/usr/include/c++/4.2.1/sstream"
#include <string.h>
#include <list>
#include <string>
#include "Expr.h"
#include "Program.h"
#include "parser.h"
#include "Type.h"

#define NL printf ( "\n" )

// for g++ compatibility 
extern int yyparse ( void );
extern int yylex ( void );
extern int yyerror ( char * s );
extern void yyrestart ( FILE * new_file );

extern int lineNum[MAX_INCLUDE_DEPTH+1];
extern int include_stack_ptr;
extern char * current_file_name[MAX_INCLUDE_DEPTH+1];

// for lists
Value int_list_el = Value ( 0 );
Value empty ( &int_list_el );

#define EMPTY new Expr ( new Value ( new std::list<Value *> ) )
#define UC { printf ( "under construction\n" ); $$ = NULL; }

// for lambdas and recursions
Environment * main_env = NULL;

#define PUSH(__str__) {                                               \
    main_env = new Environment ( __str__, new TypeExpr(), main_env );  \
} 

#define PUSH_WITH_TYPE(__str__,__type__) {                            \
  main_env = new Environment ( __str__, __type__, main_env );         \
} 

#define POP {                                                         \
  if ( !main_env->is_freed() ) {                                      \
    delete main_env->get_type();                                      \
    main_env->set_freed ( true );				      \
  } \
  Environment * temp = main_env;                                      \
  main_env = main_env->get_tail();                                    \
  delete temp;                                                        \
}

#define POP_SIMP {                                                    \
  Environment * temp = main_env;                                      \
  main_env = main_env->get_tail();                                    \
  delete temp;                                                        \
}

#define ERROR(__msg__...) {                                             \
                                                                        \
    std::stringstream strm;                                             \
                                                                        \
     strm << "gro encountered an error in "                             \
       << current_file_name[include_stack_ptr]                          \
       << " on line " << lineNum[include_stack_ptr]                     \
       << ".<blockquote>" << __msg__ << "</blockquote>";                \
                                                                        \
     throw strm.str();                                                  \
                                                                        \
}

#define TYPE_ERROR(__msg__...) {                                        \
                                                                        \
    std::stringstream strm;                                             \
                                                                        \
     strm << "Type error in "                                           \
       << current_file_name[include_stack_ptr]                          \
       << " on line " << lineNum[include_stack_ptr]                     \
       << ".<blockquote>" << __msg__ << "</blockquote>";                \
                                                                        \
     throw strm.str();                                                  \
                                                                        \
}

#define TYPE_ERROR1(__e__,__t__,__msg__...) {                                         \
                                                                                      \
    std::stringstream strm;                                                           \
                                                                                      \
    strm << "Type error in "                                                          \
         << current_file_name[include_stack_ptr]              \
         << " on line " << lineNum[include_stack_ptr]                                                \
         << "<br><blockquote>" << __msg__ << " Here, "                                \
         << "<i>" << __e__->tostring() << "</i> has type <i>" << __t__->tostring()    \
         << ".</i></blockquote>";                                                     \
                                                                                      \
         throw strm.str();                                                            \
                                                                                      \
}

#define TYPE_ERROR2(__e1__,__t1__,__e2__,__t2__,__msg__...) {                         \
                                                                                      \
    std::stringstream strm;                                                           \
                                                                                      \
    strm << "Type error in "                                                          \
         <<  current_file_name[include_stack_ptr]              \
         << " on line " << lineNum[include_stack_ptr]                                                \
         << "<br><blockquote>" << __msg__ << " In particular, "                       \
         << "<i>" << __e1__->tostring() << "</i> has type <i>" << __t1__->tostring() << "</i>, while " \
         << "<i>" << __e2__->tostring() << "</i> has type <i>" << __t2__->tostring() << ".</i>"        \
         << "</blockquote>";                                                          \
                                                                                      \
         throw strm.str();                                                            \
                                                                                      \
}

#define SET_INFO(__e__) { __e__->set_line_info ( lineNum[include_stack_ptr], current_file_name[include_stack_ptr] ); }

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
    fflush ( stdout );                                    \
}

#define VERBOOSE printf ( ">> line %d of %s\n", lineNum[include_stack_ptr], current_file_name[include_stack_ptr] );
#define VERBOSE

// main scope 
Scope * top_scope;

// for computing types of locals in programs
std::list<TypeExpr *> program_garbage;

TypeExpr * fun_temp;

// auxilliary flags for external fuction type construction
bool rec_extensible_flag, rec_ret_ex_flag;

// list of internally defined ccl functions
std::list<InternalFunction *> ccl_function_list;

 int foreach_index = 0;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 172 "../ccl-master/parser.y"
{

  Expr * exp;
  char * str;        /* for variable names */
  int i;
  double real;
  std::list<Expr *> * elist;
  std::list<Expr::FIELD *> * field_list;
  std::list<char *> * vlist;
  Command * command;
  std::list<Command *> * command_list;
  Clause * cls;
  Statement * st;
  std::list<Statement *> * st_list;
  Program * prog;  
  TypeExpr * type;
  std::list<TypeExpr *> * type_list;
  Environment * env;

}
/* Line 193 of yacc.c.  */
#line 430 "../ccl-master/parser.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 443 "../ccl-master/parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  65
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   658

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNRULES -- Number of states.  */
#define YYNSTATES  246

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   326

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,     9,    11,    15,    21,    29,    30,
      39,    40,    44,    46,    47,    52,    63,    65,    68,    69,
      73,    78,    82,    85,    87,    90,    94,    97,    99,   105,
     112,   116,   118,   120,   122,   126,   128,   131,   133,   137,
     139,   141,   142,   148,   150,   154,   158,   162,   164,   168,
     172,   176,   180,   184,   188,   191,   193,   197,   201,   205,
     207,   211,   215,   219,   222,   226,   230,   233,   235,   239,
     242,   245,   247,   250,   254,   259,   264,   272,   274,   278,
     284,   292,   300,   304,   307,   312,   315,   319,   321,   322,
     324,   326,   328,   330,   332,   334,   338,   342,   346,   348,
     349,   355,   359,   360,   363,   365,   367,   369,   371,   373,
     376,   380,   382,   383,   388,   390,   396,   400,   404,   406,
     407,   411,   415,   417
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      73,     0,    -1,    73,     3,    74,    -1,    74,    -1,    88,
      -1,    88,    22,    88,    -1,    56,    28,    86,    21,    88,
      -1,    58,   103,    28,    45,   107,    46,    29,    -1,    -1,
      59,    28,    45,    76,    46,    75,    22,    77,    -1,    -1,
      76,     9,    28,    -1,    28,    -1,    -1,    77,    43,    79,
      78,    -1,    26,    28,    49,    88,    24,    28,    45,    99,
      46,    78,    -1,    79,    -1,    60,    76,    -1,    -1,     7,
      80,     8,    -1,    28,    45,    99,    46,    -1,    45,    77,
      46,    -1,    80,    81,    -1,    81,    -1,    85,     3,    -1,
      27,    87,     3,    -1,    82,     3,    -1,    82,    -1,    88,
      24,     7,    83,     8,    -1,    88,    24,     7,    83,    84,
       8,    -1,    83,    84,    85,    -1,    85,    -1,     3,    -1,
       9,    -1,    88,    22,    88,    -1,    88,    -1,    86,    28,
      -1,    28,    -1,    87,     9,    28,    -1,    28,    -1,    90,
      -1,    -1,    23,    28,    89,    21,    88,    -1,    91,    -1,
      90,    31,    91,    -1,    90,    30,    91,    -1,    90,    25,
      91,    -1,    92,    -1,    91,    32,    92,    -1,    91,    33,
      92,    -1,    91,    34,    92,    -1,    91,    35,    92,    -1,
      91,    36,    92,    -1,    91,    37,    92,    -1,    12,    92,
      -1,    93,    -1,    92,    43,    93,    -1,    92,    38,    93,
      -1,    92,    42,    93,    -1,    94,    -1,    93,    44,    94,
      -1,    93,    39,    94,    -1,    93,    40,    94,    -1,    38,
      93,    -1,    93,    19,    94,    -1,    93,    20,    94,    -1,
      62,    93,    -1,    95,    -1,    94,    41,    95,    -1,    17,
      94,    -1,    18,    94,    -1,    96,    -1,    95,    96,    -1,
      95,    21,    96,    -1,    95,    15,    88,    16,    -1,    28,
      45,    99,    46,    -1,    28,    45,    99,     9,    59,    77,
      46,    -1,   100,    -1,    45,    88,    46,    -1,    48,    98,
      49,    88,    52,    -1,    53,    88,    54,    88,    55,    88,
      52,    -1,    50,    28,    49,    88,    51,    88,    52,    -1,
      69,    88,    52,    -1,    28,    22,    -1,    97,    88,     9,
      98,    -1,    97,    88,    -1,    99,     9,    88,    -1,    88,
      -1,    -1,     4,    -1,    28,    -1,     5,    -1,    29,    -1,
      10,    -1,    11,    -1,     7,   101,     8,    -1,    15,   102,
      16,    -1,    88,     9,   101,    -1,    88,    -1,    -1,   102,
       9,    28,    22,    88,    -1,    28,    22,    88,    -1,    -1,
      62,    28,    -1,    63,    -1,    64,    -1,     5,    -1,    65,
      -1,    29,    -1,   103,    66,    -1,    45,   103,    46,    -1,
      67,    -1,    -1,    15,   104,   105,    16,    -1,   106,    -1,
      28,    22,   103,     9,   105,    -1,    28,    22,   103,    -1,
      67,     9,   105,    -1,    67,    -1,    -1,   103,    68,   103,
      -1,   103,     9,   107,    -1,   103,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   287,   287,   288,   293,   310,   353,   389,   414,   414,
     443,   448,   449,   450,   455,   462,   509,   514,   515,   520,
     690,   715,   721,   726,   735,   736,   737,   738,   743,   744,
     749,   754,   763,   763,   768,   769,   774,   779,   788,   793,
     802,   804,   804,   817,   818,   819,   820,   826,   827,   828,
     829,   830,   831,   832,   833,   838,   839,   840,   841,   846,
     847,   848,   849,   850,   851,   852,   854,   863,   864,   865,
     866,   871,   872,   873,   874,   876,   919,   947,   949,   951,
     995,  1004,  1031,  1059,  1068,  1075,  1086,  1094,  1101,  1106,
    1108,  1109,  1110,  1111,  1112,  1113,  1115,  1138,  1145,  1152,
    1157,  1166,  1175,  1180,  1199,  1200,  1201,  1202,  1203,  1204,
    1205,  1206,  1207,  1207,  1212,  1218,  1227,  1236,  1243,  1250,
    1256,  1262,  1271,  1280
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "t_SEMICOLON", "t_INTEGER", "t_REAL",
  "t_EQUALS", "t_LCURLY", "t_RCURLY", "t_COMMA", "t_TRUE", "t_FALSE",
  "t_NOT", "t_LBBRACKET", "t_RBBRACKET", "t_LBRACKET", "t_RBRACKET",
  "t_HEAD", "t_TAIL", "t_CONCAT", "t_CONS", "t_DOT", "t_ASSIGN",
  "t_LAMBDA", "t_COLON", "t_STRCAT", "t_COMPOSE", "t_NEEDS", "t_VAR",
  "t_STRING", "t_OR", "t_AND", "t_LESS_THAN", "t_LESS_THAN_OR_EQUAL",
  "t_GREATER_THAN", "t_GREATER_THAN_OR_EQUAL", "t_EQUAL", "t_NOT_EQUAL",
  "t_MINUS", "t_MOD", "t_DIV", "t_POWER", "t_RECORD_ADD", "t_PLUS",
  "t_MULT", "t_LPAREN", "t_RPAREN", "t_APP", "t_LET", "t_IN", "t_FOREACH",
  "t_DO", "t_END", "t_IF", "t_THEN", "t_ELSE", "t_FUN", "t_EXTERNAL",
  "t_INTERNAL", "t_PROGRAM", "t_SHARING", "t_EXEC", "t_BACKQUOTE",
  "t_UNIT", "t_BOOL", "t_INT", "t_LIST", "t_VARARGS", "t_INTO",
  "t_MAP_TO_CELLS", "UNOT", "UMINUS", "$accept", "stmts", "stmt", "@1",
  "arglist", "progexpr", "share", "atomic", "stat_list", "stat", "clause",
  "alist", "separator", "assignment", "varlist", "varlist_comma", "expr",
  "@2", "expr1", "expr12", "expr2", "expr3", "expr4", "expr5", "expr6",
  "let_sub", "let_list", "exprlist", "term", "list_body", "rec_el_list",
  "type_expr", "@3", "type_assign_list", "type_expr2", "type_expr_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    74,    74,    75,    74,
      74,    76,    76,    76,    77,    77,    77,    78,    78,    79,
      79,    79,    80,    80,    81,    81,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    86,    86,    87,    87,
      88,    89,    88,    90,    90,    90,    90,    91,    91,    91,
      91,    91,    91,    91,    91,    92,    92,    92,    92,    93,
      93,    93,    93,    93,    93,    93,    93,    94,    94,    94,
      94,    95,    95,    95,    95,    95,    95,    96,    96,    96,
      96,    96,    96,    97,    98,    98,    99,    99,    99,   100,
     100,   100,   100,   100,   100,   100,   100,   101,   101,   101,
     102,   102,   102,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   104,   103,   103,   105,   105,   105,   105,   105,
     106,   107,   107,   107
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     1,     3,     5,     7,     0,     8,
       0,     3,     1,     0,     4,    10,     1,     2,     0,     3,
       4,     3,     2,     1,     2,     3,     2,     1,     5,     6,
       3,     1,     1,     1,     3,     1,     2,     1,     3,     1,
       1,     0,     5,     1,     3,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     2,     1,     3,     3,     3,     1,
       3,     3,     3,     2,     3,     3,     2,     1,     3,     2,
       2,     1,     2,     3,     4,     4,     7,     1,     3,     5,
       7,     7,     3,     2,     4,     2,     3,     1,     0,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     0,
       5,     3,     0,     2,     1,     1,     1,     1,     1,     2,
       3,     1,     0,     4,     1,     5,     3,     3,     1,     0,
       3,     3,     1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      10,    89,    91,    99,    93,    94,     0,   102,     0,     0,
       0,    90,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     3,     4,    40,    43,    47,    55,
      59,    67,    71,    77,    98,     0,    54,     0,     0,    69,
      70,    41,    88,    63,     0,     0,     0,     0,     0,     0,
       0,   106,   112,   108,     0,     0,   104,   105,   107,   111,
       0,   114,     0,    66,     0,     1,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,    90,    72,
      99,    95,     0,     0,    96,     0,    87,     0,    78,    83,
      85,     0,     0,     0,    37,     0,   119,     0,   103,     0,
     109,     0,    13,    82,     2,     5,    46,    45,    44,    48,
      49,    50,    51,    52,    53,    57,    58,    56,    64,    65,
      61,    62,    60,    68,    90,     0,    73,    97,   101,     0,
       0,     0,    75,     0,     0,     0,     0,     0,    36,     0,
     118,     0,   110,   123,   120,    12,     0,    74,     0,    42,
       0,    86,    84,    79,     0,     0,     6,     0,   119,   113,
     122,     0,     0,     8,   100,     0,     0,     0,     0,     0,
      16,     0,     0,   116,   117,   123,     0,    11,     0,     0,
       0,    23,    27,     0,    35,     0,    88,     0,     0,    76,
      81,    80,   119,   121,     7,     0,    39,     0,    19,    22,
      26,    24,     0,     0,     0,     0,    21,    18,   115,     9,
      25,     0,    34,     0,     0,     0,    20,    13,    14,    38,
       0,    31,    35,     0,    17,    32,    28,    33,     0,     0,
      29,    30,    88,     0,    18,    15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    23,    24,   188,   156,   179,   228,   180,   190,   191,
     192,   230,   238,   193,   105,   207,    96,    95,    26,    27,
      28,    29,    30,    31,    32,    46,    47,    97,    33,    35,
      38,   170,   106,   151,    61,   171
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -198
static const yytype_int16 yypact[] =
{
     181,  -198,  -198,   348,  -198,  -198,   480,    53,   506,   506,
      63,   -17,  -198,   480,   348,    82,    88,   348,   105,    20,
     110,   480,   348,   120,  -198,   121,    44,   145,    21,    69,
       1,   553,  -198,  -198,   133,   140,    21,   128,    18,     1,
       1,  -198,   348,    69,   108,   129,   348,   104,   112,   102,
     131,  -198,  -198,  -198,    20,   134,  -198,  -198,  -198,  -198,
     -15,  -198,   123,    69,   111,  -198,   181,   348,   430,   430,
     430,   480,   480,   480,   480,   480,   480,   480,   480,   480,
     506,   506,   506,   506,   506,   562,   401,   589,  -198,  -198,
     348,  -198,   348,   138,  -198,   146,  -198,    10,  -198,  -198,
     161,   348,   348,   348,  -198,    34,   -20,   -36,  -198,   126,
    -198,    20,   144,  -198,  -198,  -198,   145,   145,   145,    21,
      21,    21,    21,    21,    21,    69,    69,    69,     1,     1,
       1,     1,     1,   553,    -1,   157,  -198,  -198,  -198,   152,
     348,   237,  -198,    82,   132,   136,   139,   348,  -198,   167,
     174,   179,  -198,    20,    71,  -198,    11,  -198,   348,  -198,
       5,  -198,  -198,  -198,   348,   348,  -198,    20,   -20,  -198,
       2,   151,   172,  -198,  -198,   266,   173,   158,     5,    85,
    -198,   150,   153,    39,  -198,    20,   177,  -198,   185,   180,
     107,  -198,   208,   210,   122,   166,   348,    86,     9,  -198,
    -198,  -198,   -20,  -198,  -198,     5,  -198,    97,  -198,  -198,
    -198,  -198,   348,   209,   348,    14,  -198,   160,  -198,   175,
    -198,   189,  -198,   348,   197,   348,  -198,   144,  -198,  -198,
     118,  -198,   200,   196,   218,  -198,  -198,  -198,   319,   183,
    -198,  -198,   348,    15,   160,  -198
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -198,  -198,   164,  -198,     6,  -160,   -12,    37,  -198,    46,
    -198,  -198,  -198,  -197,  -198,  -198,     0,  -198,  -198,   -30,
      23,    -6,    -4,   168,   -29,  -198,   103,  -190,  -198,   155,
    -198,   -18,  -198,  -159,  -198,    66
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      25,    60,    89,    34,    39,    40,   215,    43,   149,   184,
     152,   185,   175,   109,    44,    63,   175,    49,   197,   141,
     172,    92,    64,   225,   225,    51,   231,    93,    42,    36,
     110,   176,   111,   177,    94,    52,   107,   177,   116,   117,
     118,   241,    85,   218,    42,   219,   100,   150,   202,    53,
     178,   110,   243,   111,   178,   147,   142,   173,   136,    77,
     226,   244,   148,    78,    79,    54,    25,   115,   110,    68,
     111,   125,   126,   127,    69,    70,   128,   129,   130,   131,
     132,    37,    55,    56,    57,    58,   135,    59,    80,    81,
      34,    41,   138,   154,   119,   120,   121,   122,   123,   124,
     220,   144,   145,   146,    89,   110,   221,   111,    82,    83,
      45,     1,     2,    84,     3,   208,    48,     4,     5,     6,
      65,   235,     7,    66,     8,     9,   236,   237,   198,   198,
      10,   199,   216,    50,   189,    11,    12,   110,    62,   111,
     159,   161,    90,    67,   212,    13,   213,   166,    91,   183,
      92,    99,    14,   101,    98,    15,   103,    16,   174,   104,
      17,   102,   108,   113,   181,   182,   139,   140,   112,    21,
     143,   153,   155,   157,   158,   194,    22,    71,    72,    73,
      74,    75,    76,   168,   163,     1,     2,   164,     3,   167,
     194,     4,     5,     6,   165,   169,     7,   186,     8,     9,
     187,   195,   200,   196,    10,   201,   204,   205,   206,    11,
      12,   210,   222,   211,   224,   214,   223,   229,   198,    13,
     227,   233,   212,   232,   239,   161,    14,   172,   242,    15,
     114,    16,   245,   234,    17,   217,   209,    18,   232,    19,
      20,     1,     2,    21,     3,   137,   162,     4,     5,     6,
      22,   203,     7,   133,     8,     9,     0,     0,     0,     0,
      10,     0,     0,     0,     0,    11,    12,     0,     0,     0,
       1,     2,     0,     3,     0,    13,     4,     5,     6,     0,
       0,     7,    14,     8,     9,    15,     0,    16,     0,    10,
      17,     0,     0,   189,    11,    12,   160,     0,     0,    21,
       0,     0,     0,     0,    13,     0,    22,     0,     0,     0,
       0,    14,     0,     0,    15,     0,    16,     0,     0,    17,
       0,     0,     0,     1,     2,     0,     3,   240,    21,     4,
       5,     6,     0,     0,     7,    22,     8,     9,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    11,    12,     0,
       0,     0,     1,     2,     0,     3,     0,    13,     4,     5,
       6,     0,     0,     7,    14,     8,     9,    15,     0,    16,
       0,    10,    17,     0,     0,     0,    11,    12,     0,     0,
       0,    21,     0,     0,     0,     0,    13,     0,    22,     0,
       0,     0,     0,    14,     0,     0,    15,     0,    16,     0,
       0,    17,     0,     0,     0,     1,     2,     0,     3,     0,
      21,     4,     5,     6,     0,     0,     7,    22,     8,     9,
       0,     0,     0,     0,    10,     0,     0,     0,     0,   134,
      12,     0,     0,     0,     1,     2,     0,     3,     0,    13,
       4,     5,     6,     0,     0,     7,    14,     8,     9,    15,
       0,    16,     0,     0,    17,     0,     0,     0,    11,    12,
       0,     0,     0,    21,     0,     0,     0,     0,    13,     0,
      22,     0,     0,     0,     0,    14,     0,     0,    15,     0,
      16,     0,     0,    17,     1,     2,     0,     3,     0,     0,
       4,     5,    21,     0,     0,     7,     0,     8,     9,    22,
       0,     0,     0,     0,     0,     0,     0,     0,    11,    12,
       1,     2,     0,     3,     0,     0,     4,     5,    13,     0,
       0,     7,     0,     8,     9,    14,     0,     0,    15,     0,
      16,     0,     0,    17,    11,    12,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,     0,    22,
       0,    14,     0,     0,    15,     0,    16,     1,     2,    17,
       3,     0,     0,     4,     5,     0,     1,     2,    86,     3,
       0,     0,     4,     5,    87,    22,     0,     7,     0,     0,
       0,    88,    12,     0,     0,     0,     0,     0,     0,     0,
      11,    12,     0,     1,     2,     0,     3,     0,    14,     4,
       5,    15,     0,    16,     7,     0,    17,    14,     0,     0,
      15,     0,    16,     0,     0,    17,     0,    88,    12,     0,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,    14,     0,     0,    15,     0,    16,
       0,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22
};

static const yytype_int16 yycheck[] =
{
       0,    19,    31,     3,     8,     9,   196,    13,    28,   168,
      46,     9,     7,    28,    14,    21,     7,    17,   178,     9,
       9,    22,    22,     9,     9,     5,   223,     9,    45,     6,
      66,    26,    68,    28,    16,    15,    54,    28,    68,    69,
      70,   238,    41,   202,    45,   205,    46,    67,     9,    29,
      45,    66,   242,    68,    45,    21,    46,    46,    87,    38,
      46,    46,    28,    42,    43,    45,    66,    67,    66,    25,
      68,    77,    78,    79,    30,    31,    80,    81,    82,    83,
      84,    28,    62,    63,    64,    65,    86,    67,    19,    20,
      90,    28,    92,   111,    71,    72,    73,    74,    75,    76,
       3,   101,   102,   103,   133,    66,     9,    68,    39,    40,
      28,     4,     5,    44,     7,     8,    28,    10,    11,    12,
       0,     3,    15,     3,    17,    18,     8,     9,    43,    43,
      23,    46,    46,    28,    27,    28,    29,    66,    28,    68,
     140,   141,     9,    22,    22,    38,    24,   147,     8,   167,
      22,    22,    45,    49,    46,    48,    54,    50,   158,    28,
      53,    49,    28,    52,   164,   165,    28,    21,    45,    62,
       9,    45,    28,    16,    22,   175,    69,    32,    33,    34,
      35,    36,    37,     9,    52,     4,     5,    51,     7,    22,
     190,    10,    11,    12,    55,    16,    15,    46,    17,    18,
      28,    28,    52,    45,    23,    52,    29,    22,    28,    28,
      29,     3,   212,     3,   214,    49,     7,    28,    43,    38,
      60,    24,    22,   223,    28,   225,    45,     9,    45,    48,
      66,    50,   244,   227,    53,   198,   190,    56,   238,    58,
      59,     4,     5,    62,     7,    90,   143,    10,    11,    12,
      69,   185,    15,    85,    17,    18,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
       4,     5,    -1,     7,    -1,    38,    10,    11,    12,    -1,
      -1,    15,    45,    17,    18,    48,    -1,    50,    -1,    23,
      53,    -1,    -1,    27,    28,    29,    59,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    38,    -1,    69,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,     4,     5,    -1,     7,     8,    62,    10,
      11,    12,    -1,    -1,    15,    69,    17,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,     4,     5,    -1,     7,    -1,    38,    10,    11,
      12,    -1,    -1,    15,    45,    17,    18,    48,    -1,    50,
      -1,    23,    53,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    -1,    38,    -1,    69,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,     4,     5,    -1,     7,    -1,
      62,    10,    11,    12,    -1,    -1,    15,    69,    17,    18,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,     4,     5,    -1,     7,    -1,    38,
      10,    11,    12,    -1,    -1,    15,    45,    17,    18,    48,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    38,    -1,
      69,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      50,    -1,    -1,    53,     4,     5,    -1,     7,    -1,    -1,
      10,    11,    62,    -1,    -1,    15,    -1,    17,    18,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
       4,     5,    -1,     7,    -1,    -1,    10,    11,    38,    -1,
      -1,    15,    -1,    17,    18,    45,    -1,    -1,    48,    -1,
      50,    -1,    -1,    53,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    45,    -1,    -1,    48,    -1,    50,     4,     5,    53,
       7,    -1,    -1,    10,    11,    -1,     4,     5,    15,     7,
      -1,    -1,    10,    11,    21,    69,    -1,    15,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,     4,     5,    -1,     7,    -1,    45,    10,
      11,    48,    -1,    50,    15,    -1,    53,    45,    -1,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    28,    29,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     7,    10,    11,    12,    15,    17,    18,
      23,    28,    29,    38,    45,    48,    50,    53,    56,    58,
      59,    62,    69,    73,    74,    88,    90,    91,    92,    93,
      94,    95,    96,   100,    88,   101,    92,    28,   102,    94,
      94,    28,    45,    93,    88,    28,    97,    98,    28,    88,
      28,     5,    15,    29,    45,    62,    63,    64,    65,    67,
     103,   106,    28,    93,    88,     0,     3,    22,    25,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    42,    43,
      19,    20,    39,    40,    44,    41,    15,    21,    28,    96,
       9,     8,    22,     9,    16,    89,    88,    99,    46,    22,
      88,    49,    49,    54,    28,    86,   104,   103,    28,    28,
      66,    68,    45,    52,    74,    88,    91,    91,    91,    92,
      92,    92,    92,    92,    92,    93,    93,    93,    94,    94,
      94,    94,    94,    95,    28,    88,    96,   101,    88,    28,
      21,     9,    46,     9,    88,    88,    88,    21,    28,    28,
      67,   105,    46,    45,   103,    28,    76,    16,    22,    88,
      59,    88,    98,    52,    51,    55,    88,    22,     9,    16,
     103,   107,     9,    46,    88,     7,    26,    28,    45,    77,
      79,    88,    88,   103,   105,     9,    46,    28,    75,    27,
      80,    81,    82,    85,    88,    28,    45,    77,    43,    46,
      52,    52,     9,   107,    29,    22,    28,    87,     8,    81,
       3,     3,    22,    24,    49,    99,    46,    79,   105,    77,
       3,     9,    88,     7,    88,     9,    46,    60,    78,    28,
      83,    85,    88,    24,    76,     3,     8,     9,    84,    28,
       8,    85,    45,    99,    46,    78
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 294 "../ccl-master/parser.y"
    {

      VERBOSE; 

      (yyvsp[(1) - (1)].exp)->type_check ( top_scope, main_env ); // will throw an error if there is a problem

      Value * ans = (yyvsp[(1) - (1)].exp)->eval ( top_scope );

      if ( ans->get_type() != Value::UNIT ) { ans->print(); NL; NL;} 

      delete ans;

      //delete $1;

    ;}
    break;

  case 5:
#line 311 "../ccl-master/parser.y"
    { 

      VERBOSE;

      Expr * lhs = (yyvsp[(1) - (3)].exp),
           * rhs = (yyvsp[(3) - (3)].exp);

      !rhs->type_check ( top_scope, main_env );

      if ( lhs->get_type() == Expr::VARIABLE && top_scope->get ( lhs->get_name() ) == NULL) {

        top_scope->add ( lhs->get_name(), rhs->eval ( top_scope ) );

      } else {

	std::list<TypeExpr *> garbage, ng;

	if ( ! TypeExpr::unify ( lhs->compute_type ( top_scope, main_env, &ng, &garbage ),
				 rhs->compute_type ( top_scope, main_env, &ng, &garbage ) ) ) {

          TYPE_ERROR ( "Type Error: The symbol <i>" << lhs->tostring()
                       << "</i> has a different type than does the expression <i>" << rhs->tostring() << ".<i>" );

	} else if ( ! lhs->is_lhs() ) {

          TYPE_ERROR ( "Type Error: illegal expression " << lhs->tostring() << " on left hand side of assignment." );

	} else {

	  top_scope->set ( lhs, rhs->eval ( top_scope ) );

	}

	TypeExpr::take_out ( &garbage );

      }

      delete (yyvsp[(1) - (3)].exp);
      delete (yyvsp[(3) - (3)].exp);

    ;}
    break;

  case 6:
#line 353 "../ccl-master/parser.y"
    {

      VERBOSE;

      fun_temp = new TypeExpr ( new TypeExpr(), new TypeExpr() );

      PUSH_WITH_TYPE ( (yyvsp[(2) - (5)].str), fun_temp ); 

      std::list<char *>::iterator i;
      Expr * e = (yyvsp[(5) - (5)].exp);
      SET_INFO(e);

      for ( i=(yyvsp[(3) - (5)].vlist)->begin(); i != (yyvsp[(3) - (5)].vlist)->end(); i++ ) {
	e = FUNC ( (*i), e );
	free (*i);
	SET_INFO(e);
      }

      delete (yyvsp[(3) - (5)].vlist);

      e->type_check ( top_scope, main_env );

      fun_temp->reset();
      fun_temp->free_children();

      POP; /* frees fun_temp */

      Value * answer = e->eval ( top_scope );
      top_scope->add ( (yyvsp[(2) - (5)].str), answer );
      top_scope->set_recursive ( (yyvsp[(2) - (5)].str) );
      delete e;

      free ( (yyvsp[(2) - (5)].str) );

    ;}
    break;

  case 7:
#line 391 "../ccl-master/parser.y"
    {

      VERBOSE;

      const char * error;

      EXTERNAL_CCLI_FUNCTION f = lookup_ccl_function ( (yyvsp[(7) - (7)].str) );

      if ( f == NULL )  {
        ERROR ( "Could not find symbol <i>" << (yyvsp[(7) - (7)].str) << "</i> in list of internally defined functions." );
      } 

      Value * efunc = new Value ( f, (yyvsp[(2) - (7)].type), (yyvsp[(5) - (7)].type_list) );
      top_scope->add ( (yyvsp[(3) - (7)].str), efunc );
      free ( (yyvsp[(3) - (7)].str) );

      while ( main_env != NULL )
	POP_SIMP;

      free ( (yyvsp[(7) - (7)].str) );

    ;}
    break;

  case 8:
#line 414 "../ccl-master/parser.y"
    {

      VERBOSE;

      std::list<char *>::iterator i;

      for ( i = (yyvsp[(4) - (5)].vlist)->begin(); i != (yyvsp[(4) - (5)].vlist)->end(); i++ ) {  // push parameter list onto main_env
	PUSH ( *i );
      }

    ;}
    break;

  case 9:
#line 424 "../ccl-master/parser.y"
    {

      std::list<char *>::iterator i;

      (yyvsp[(8) - (8)].prog)->set_args((yyvsp[(4) - (8)].vlist));
      (yyvsp[(8) - (8)].prog)->compute_arg_types ( main_env ); // doesn't do anything if $8 is atomic
      top_scope->add((yyvsp[(2) - (8)].str),(yyvsp[(8) - (8)].prog));
      (yyvsp[(8) - (8)].prog)->set_name ( (yyvsp[(2) - (8)].str) );

      for ( i = (yyvsp[(4) - (8)].vlist)->begin(); i != (yyvsp[(4) - (8)].vlist)->end(); i++ ) {  // pop parameter list
	POP;
      }

      TypeExpr::take_out ( &program_garbage );

      free ( (yyvsp[(2) - (8)].str) );

    ;}
    break;

  case 11:
#line 448 "../ccl-master/parser.y"
    { (yyval.vlist) = (yyvsp[(1) - (3)].vlist); (yyval.vlist)->push_back ( (yyvsp[(3) - (3)].str) ); ;}
    break;

  case 12:
#line 449 "../ccl-master/parser.y"
    { (yyval.vlist) = new std::list<char *>; (yyval.vlist)->push_back ( (yyvsp[(1) - (1)].str) ); ;}
    break;

  case 13:
#line 450 "../ccl-master/parser.y"
    { (yyval.vlist) = new std::list<char *>; ;}
    break;

  case 14:
#line 455 "../ccl-master/parser.y"
    { 

        (yyval.prog) = new CompositeProgram ( (yyvsp[(1) - (4)].prog), (yyvsp[(3) - (4)].prog), (yyvsp[(4) - (4)].vlist), current_file_name[include_stack_ptr], lineNum[include_stack_ptr] );
	FREE_STR_LIST ( (yyvsp[(4) - (4)].vlist) );

      ;}
    break;

  case 15:
#line 462 "../ccl-master/parser.y"
    {

      // 1) $4 evaluates to a list of constants, and that it isn't empty?

      if ( ! (yyvsp[(4) - (10)].exp)->type_check ( top_scope, NULL ) ) {  // note: using NULL instead of main_env here because the
                                                     // index set should not depend on parameters of some enclosing
                                                     // program definition
        TYPE_ERROR ( "Something wrong with </i>" << (yyvsp[(4) - (10)].exp)->tostring() << "<i> in program composition expression." );
      }

      Value * L = (yyvsp[(4) - (10)].exp)->eval ( top_scope );

      if ( L->get_type() != Value::LIST ) {
        TYPE_ERROR ( "Index set in program composition does not evaluate to a list." );
      }

      if ( L->list_value()->size() < 1 )  {
        ERROR ( "Index set in program composition is empty." );
      }

      // 2) $6 is a program?
      Program * p = top_scope->get_program ( (yyvsp[(6) - (10)].str) );

      if ( p == NULL ) {
         ERROR ( "The symbol <i>" << (yyvsp[(6) - (10)].str) << "</i> is undefined, or is not defined as a program." );
      }

      // 3) $6->args and $8 have same length?
      if ( p->get_args()->size() != (yyvsp[(8) - (10)].elist)->size() ) {
         ERROR ( "The program <i>" << (yyvsp[(6) - (10)].str) << "</i> is instantiated with the wrong number of arguments "
                << "(should have " << p->get_args()->size() << ")." );
      }

      // 4) Make the composition
      (yyval.prog) = Program::Composition ( p, (yyvsp[(8) - (10)].elist), (yyvsp[(2) - (10)].str), L->list_value(), (yyvsp[(10) - (10)].vlist), top_scope, main_env, 
				  current_file_name[include_stack_ptr], lineNum[include_stack_ptr] );

      // 5) Delete old stuff
      free((yyvsp[(2) - (10)].str));
      free((yyvsp[(6) - (10)].str));
      FREE_STR_LIST ( (yyvsp[(10) - (10)].vlist) );
      FREE_EXPR_LIST ( (yyvsp[(8) - (10)].elist) );
      delete (yyvsp[(4) - (10)].exp);
      delete L;

    ;}
    break;

  case 16:
#line 509 "../ccl-master/parser.y"
    { (yyval.prog) = (yyvsp[(1) - (1)].prog); ;}
    break;

  case 17:
#line 514 "../ccl-master/parser.y"
    { (yyval.vlist) = (yyvsp[(2) - (2)].vlist); ;}
    break;

  case 18:
#line 515 "../ccl-master/parser.y"
    { (yyval.vlist) = new std::list<char *>; ;}
    break;

  case 19:
#line 522 "../ccl-master/parser.y"
    {

      AtomicProgram * p = new AtomicProgram ( current_file_name[include_stack_ptr], lineNum[include_stack_ptr] );
      Clause * init = new Clause ( CONST ( true ) );
      std::list<Statement *>::iterator i;
      std::list<TypeExpr *> ng, needs_garbage;
      TypeExpr * T;
      int program_var_counter = 0;

      for ( i=(yyvsp[(2) - (3)].st_list)->begin(); i!=(yyvsp[(2) - (3)].st_list)->end(); i++ ) {

	if ( (*i)->com != NULL ) { // initializer //////////////////////////////////////////////////////////////////

	  init->addCommand ( (*i)->com );
	  T = (*i)->com->get_rhs()->compute_type ( top_scope, main_env, &ng, &program_garbage );

          if ( !T->is_consistent() ) TYPE_ERROR ( "Inconsistent type." );

          Expr * lhs = (*i)->com->get_lhs();

          if ( lhs != NULL 
	       && lhs->get_type() == Expr::VARIABLE 
	       && RETRIEVE(main_env,lhs->get_name()) == NULL ) {

	    //	    p->add_symbol ( lhs->get_name(), new Value ( Value::UNIT ) ); 
	    PUSH_WITH_TYPE ( lhs->get_name(), T );
	    program_var_counter++;

	  }

	  if ( lhs != NULL ) { // check types of lhs and rhs

	    TypeExpr * Tlhs = lhs->compute_type ( top_scope, main_env, &ng, &program_garbage );

            if ( !Tlhs->is_consistent() ) TYPE_ERROR ( "Inconsistent type." );

	    if ( ! TypeExpr::unify ( T, Tlhs ) ) {

              TYPE_ERROR ( "Incompatible types in assignment. "
                << "The left hand side has type <i>" << Tlhs->tostring()
                << "</i> while the right hand side has type <i>" << T->tostring() << ".</i>" );
	      
	    }

	  }

	} else if ( (*i)->cl != NULL ) { // clause /////////////////////////////////////////////////////////////////

	  T = (*i)->cl->get_guard()->compute_type ( top_scope, main_env, &ng, &program_garbage );
          if ( !T->is_consistent() ) TYPE_ERROR ( "Inconsistent type." );

          TypeExpr * B = new TypeExpr ( Value::BOOLEAN );
	  program_garbage.push_back ( B );

	  if ( ! TypeExpr::unify ( T, B ) ) {

            TYPE_ERROR ( "Clause does not have Boolean guard. In particular, "
              << "<i>" << (*i)->cl->get_guard()->tostring() << "</i> has type "
              << "<i>" << T->tostring() << ".</i>" );

	  }

	  std::list<Command *>::iterator j;

	  for ( j = (*i)->cl->get_commands()->begin(); j != (*i)->cl->get_commands()->end(); j++ ) {

	    if ( (*j)->get_lhs() != NULL ) {

	      TypeExpr * T1 = (*j)->get_lhs()->compute_type ( top_scope, main_env, &ng, &program_garbage ),
		* T2 = (*j)->get_rhs()->compute_type ( top_scope, main_env, &ng, &program_garbage );
	      
              if ( ! TypeExpr::unify ( T1, T2 ) ) {

                TYPE_ERROR ( "Incompatible types in assignment in command part of guarded command. "
                  << "<i>" << (*j)->get_lhs()->tostring() << "</i> has type <i>" << T1->tostring()
                  << "</i> while <i>" << (*j)->get_rhs()->tostring() << "</i> has type <i>" << T2->tostring() << ".</i>" );

	      }

	    } else {

	      TypeExpr * T = (*j)->get_rhs()->compute_type ( top_scope, main_env, &ng, &program_garbage );
	      if ( !T->is_consistent() )
		return -1;

	    }

	    //TypeExpr::take_out ( &program_garbage );

	  }

	  p->add_clause ( (*i)->cl );

	} else if ( (*i)->vars != NULL ) { // "needs" statement ////////////////////////////////////////////////////

          std::list<char *>::iterator j;

	  for ( j = (*i)->vars->begin(); j != (*i)->vars->end(); j++ ) {
	    PUSH ( *j );
	    needs_garbage.push_back ( main_env->get_type() );
            p->add_needs ( *j ); 
	    program_var_counter++;
	    free ( *j );
	  }

	  delete (*i)->vars;

        }

	delete (*i);

      } // done reading statements

      delete (yyvsp[(2) - (3)].st_list);
      p->set_init ( init );
      (yyval.prog) = p;

      // save parameter and local variable types
      {

	Environment * env = main_env;
	std::list<TypeExpr *> * vars = new std::list<TypeExpr *>, 
                              * ng_save = new std::list<TypeExpr *>;

	while ( env != NULL ) {
	  env->get_type()->compute_non_generics ( vars );
	  env = env->get_tail();
	}
	
	COPY_TYPEVAR_LIST ( vars, ng_save );

	p->set_non_generics ( ng_save );

	std::list<TypeExpr *>::iterator w;

	Environment * temp = main_env, * locals = NULL;

	for ( int j = 0; j < program_var_counter; j++ ) { // this copies variables in assignments and in needs statements
	  locals = new Environment ( temp->get_name(), temp->get_type()->copy_ex ( vars, ng_save ), locals );
	  temp = temp->get_tail();
	}

	p->set_locals ( locals );
	
	Environment * arg_types = NULL;
	if ( temp != NULL )
	  arg_types = temp->copy_ex ( vars, ng_save );

	p->set_arg_types ( arg_types );

	delete vars;

      } 

      // pop local program vars and add default values 
      for ( int j = 0; j < program_var_counter; j++ ) {
	p->add_symbol ( main_env->get_name(), main_env->get_type()->default_value() );
	POP_SIMP;

      }

      // free needs type exprs
      std::list<TypeExpr *>::iterator q;
      for ( q=needs_garbage.begin(); q != needs_garbage.end(); q++ )
	delete (*q);

    ;}
    break;

  case 20:
#line 692 "../ccl-master/parser.y"
    {

      Program * p = top_scope->get_program((yyvsp[(1) - (4)].str));

      if ( p == NULL ) {
         ERROR ( "Symbol <i>" << (yyvsp[(1) - (4)].str) << "</i> undefined." );
      }

      std::list<char *> * args = p->get_args();

      if ( args->size() != (yyvsp[(3) - (4)].elist)->size() ) {
         ERROR ( "The program <i>" << (yyvsp[(1) - (4)].str) << "</i> is instantiated with the wrong number of arguments "
             << "(should have " << args->size() << ")." );
      }

      p = p->copy();
      p->set_insts ( (yyvsp[(3) - (4)].elist), top_scope, main_env, (yyvsp[(1) - (4)].str) );
      (yyval.prog) = new InstProgram ( p, current_file_name[include_stack_ptr], lineNum[include_stack_ptr] ); 

      free ( (yyvsp[(1) - (4)].str) );

    ;}
    break;

  case 21:
#line 715 "../ccl-master/parser.y"
    { (yyval.prog) = (yyvsp[(2) - (3)].prog); ;}
    break;

  case 22:
#line 722 "../ccl-master/parser.y"
    {
      (yyval.st_list) = (yyvsp[(1) - (2)].st_list);
      (yyval.st_list)->push_back ( (yyvsp[(2) - (2)].st) );
    ;}
    break;

  case 23:
#line 727 "../ccl-master/parser.y"
    {
      (yyval.st_list) = new std::list<Statement *>;
      (yyval.st_list)->push_back ( (yyvsp[(1) - (1)].st) );
    ;}
    break;

  case 24:
#line 735 "../ccl-master/parser.y"
    { (yyval.st) = new Statement ( (yyvsp[(1) - (2)].command) );  ;}
    break;

  case 25:
#line 736 "../ccl-master/parser.y"
    { (yyval.st) = new Statement ( (yyvsp[(2) - (3)].vlist) ); ;}
    break;

  case 26:
#line 737 "../ccl-master/parser.y"
    { (yyval.st) = new Statement ( (yyvsp[(1) - (2)].cls) ); ;}
    break;

  case 27:
#line 738 "../ccl-master/parser.y"
    { (yyval.st) = new Statement ( (yyvsp[(1) - (1)].cls) ); ;}
    break;

  case 28:
#line 743 "../ccl-master/parser.y"
    { (yyval.cls) = new Clause ( (yyvsp[(1) - (5)].exp), (yyvsp[(4) - (5)].command_list) ); ;}
    break;

  case 29:
#line 744 "../ccl-master/parser.y"
    { (yyval.cls) = new Clause ( (yyvsp[(1) - (6)].exp), (yyvsp[(4) - (6)].command_list) ); ;}
    break;

  case 30:
#line 750 "../ccl-master/parser.y"
    {
      (yyval.command_list) = (yyvsp[(1) - (3)].command_list);
      (yyval.command_list)->push_back((yyvsp[(3) - (3)].command));
    ;}
    break;

  case 31:
#line 755 "../ccl-master/parser.y"
    {
      (yyval.command_list) = new std::list<Command *>;
      (yyval.command_list)->push_back((yyvsp[(1) - (1)].command));
    ;}
    break;

  case 34:
#line 768 "../ccl-master/parser.y"
    { (yyval.command) = new Command ( (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); ;}
    break;

  case 35:
#line 769 "../ccl-master/parser.y"
    { (yyval.command) = new Command ( NULL, (yyvsp[(1) - (1)].exp) ); ;}
    break;

  case 36:
#line 775 "../ccl-master/parser.y"
    {
      (yyvsp[(1) - (2)].vlist)->push_front ( (yyvsp[(2) - (2)].str) );
      (yyval.vlist) = (yyvsp[(1) - (2)].vlist);
    ;}
    break;

  case 37:
#line 780 "../ccl-master/parser.y"
    {
      (yyval.vlist) = new std::list<char *>; 
      (yyval.vlist)->push_back ( (yyvsp[(1) - (1)].str) );
    ;}
    break;

  case 38:
#line 789 "../ccl-master/parser.y"
    {
      (yyvsp[(1) - (3)].vlist)->push_front ( (yyvsp[(3) - (3)].str) );
      (yyval.vlist) = (yyvsp[(1) - (3)].vlist);
    ;}
    break;

  case 39:
#line 794 "../ccl-master/parser.y"
    {
      (yyval.vlist) = new std::list<char *>; 
      (yyval.vlist)->push_back ( (yyvsp[(1) - (1)].str) );
    ;}
    break;

  case 41:
#line 804 "../ccl-master/parser.y"
    { PUSH((yyvsp[(2) - (2)].str)); ;}
    break;

  case 42:
#line 804 "../ccl-master/parser.y"
    { 

      (yyval.exp) = FUNC ( (yyvsp[(2) - (5)].str), (yyvsp[(5) - (5)].exp) ); 
      SET_INFO((yyval.exp));
      free ( (yyvsp[(2) - (5)].str) ); 
      POP;

    ;}
    break;

  case 43:
#line 817 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 44:
#line 818 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::AND, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 45:
#line 819 "../ccl-master/parser.y"
    {  (yyval.exp) = BINEXP ( Expr::OR, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 46:
#line 820 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::STR_CAT, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 47:
#line 826 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 48:
#line 827 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::LT, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 49:
#line 828 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::LE, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 50:
#line 829 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::GT, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 51:
#line 830 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::GE, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 52:
#line 831 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::EQ, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 53:
#line 832 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::NE, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 54:
#line 833 "../ccl-master/parser.y"
    { (yyval.exp) = UNEXP ( Expr::NOT, (yyvsp[(2) - (2)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 55:
#line 838 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 56:
#line 839 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::PLUS, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 57:
#line 840 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::MINUS, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 58:
#line 841 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::RECORD_ADD, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 59:
#line 846 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 60:
#line 847 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::MULT, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 61:
#line 848 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::MOD, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 62:
#line 849 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::DIV, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 63:
#line 850 "../ccl-master/parser.y"
    { (yyval.exp) = UNEXP ( Expr::UMIN, (yyvsp[(2) - (2)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 64:
#line 851 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::CONCAT, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 65:
#line 852 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::CONS, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 66:
#line 854 "../ccl-master/parser.y"
    { 
	std::list<char *> locals;
	(yyval.exp) = (yyvsp[(2) - (2)].exp)->prevify ( &locals ); 
	delete (yyvsp[(2) - (2)].exp); 
    ;}
    break;

  case 67:
#line 863 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 68:
#line 864 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::POW, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 69:
#line 865 "../ccl-master/parser.y"
    { (yyval.exp) = UNEXP ( Expr::HEAD, (yyvsp[(2) - (2)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 70:
#line 866 "../ccl-master/parser.y"
    { (yyval.exp) = UNEXP ( Expr::TAIL, (yyvsp[(2) - (2)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 71:
#line 871 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 72:
#line 872 "../ccl-master/parser.y"
    { (yyval.exp) = APP ( (yyvsp[(1) - (2)].exp), (yyvsp[(2) - (2)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 73:
#line 873 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::DOT, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 74:
#line 874 "../ccl-master/parser.y"
    { (yyval.exp) = BINEXP ( Expr::LIST_EL, (yyvsp[(1) - (4)].exp), (yyvsp[(3) - (4)].exp) ); SET_INFO((yyval.exp)); ;}
    break;

  case 75:
#line 876 "../ccl-master/parser.y"
    { // this could be either a function application, an external function application
                                         // or possibly a program

      Value * f = top_scope->get ( (yyvsp[(1) - (4)].str) );

      if ( f && f->get_type() == Value::EX_FUNCTION ) { // external function 

        if ( !f ) {
          ERROR ( "Seriously odd error. The symbol <i>" << (yyvsp[(1) - (4)].str) << "</i> is undefined." );
        }

        (yyval.exp) = EAPP ( f->copy(), (yyvsp[(3) - (4)].elist) ); 
	SET_INFO((yyval.exp));

	free ( (yyvsp[(1) - (4)].str) );

      } else if ( top_scope->get_program ( (yyvsp[(1) - (4)].str) ) ) {

          TYPE_ERROR ( "The program <i>" << (yyvsp[(1) - (4)].str) << "</i> appears mixed with expressions that are not programs." );

      } else {

	if ( (yyvsp[(3) - (4)].elist)->size() == 1 ) {

          Expr * arg = VAR ( (yyvsp[(1) - (4)].str) );
	  SET_INFO ( arg );

	  (yyval.exp) = APP ( arg, *((yyvsp[(3) - (4)].elist)->begin()) );
	  SET_INFO((yyval.exp));

	  free ( (yyvsp[(1) - (4)].str) );
	  delete (yyvsp[(3) - (4)].elist);

	} else {

          ERROR ( "Parse error." );

	}

      }

    ;}
    break;

  case 76:
#line 919 "../ccl-master/parser.y"
    {

      Value * f = top_scope->get ( (yyvsp[(1) - (7)].str) );

      if ( f && f->get_type() == Value::EX_FUNCTION ) { // external function 

        if ( !f ) {
          ERROR ( "Seriously odd error. The symbol <i>" << (yyvsp[(1) - (7)].str) << "</i> is undefined." );
        }

        (yyvsp[(3) - (7)].elist)->push_back ( new Expr ( (yyvsp[(6) - (7)].prog) ) );
        (yyval.exp) = EAPP ( f->copy(), (yyvsp[(3) - (7)].elist) ); 
	SET_INFO((yyval.exp));

      } else {

        ERROR ( "<i>" << (yyvsp[(1) - (7)].str) << "</i> should refer to an external function since it has a program as an argument." );

      }

      free ( (yyvsp[(1) - (7)].str) );

    ;}
    break;

  case 77:
#line 947 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 78:
#line 949 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(2) - (3)].exp); SET_INFO((yyval.exp)); ;}
    break;

  case 79:
#line 951 "../ccl-master/parser.y"
    { 

      std::list<Statement *>::iterator i;
      Expr * result = NULL;

      for ( i = (yyvsp[(2) - (5)].st_list)->begin(); i != (yyvsp[(2) - (5)].st_list)->end(); i++ ) {

        Command * c = (*i)->com;

        if ( c == NULL || c->get_lhs()->get_type() != Expr::VARIABLE ) {
          ERROR ( "Error in let expression (not a variable assignment). " );
	} 

        if ( result == NULL ) {

          result = LETEXP ( c->get_lhs()->get_name(), c->get_rhs()->copy(), (yyvsp[(4) - (5)].exp) );

        } else {

          result = LETEXP ( c->get_lhs()->get_name(), c->get_rhs()->copy(), result );

	}

      }

      for ( i = (yyvsp[(2) - (5)].st_list)->begin(); i != (yyvsp[(2) - (5)].st_list)->end(); i++ ) {

        Command * c = (*i)->com;
	POP;
	delete c;

      }

      if ( result == NULL ) {
        ERROR ( "Empty assignment section in let statement." );
      }

      (yyval.exp) = result;
      SET_INFO((yyval.exp));

      delete (yyvsp[(2) - (5)].st_list);

    ;}
    break;

  case 80:
#line 997 "../ccl-master/parser.y"
    { 

      (yyval.exp) = IFEXP ( (yyvsp[(2) - (7)].exp), (yyvsp[(4) - (7)].exp), (yyvsp[(6) - (7)].exp) ); 
      SET_INFO((yyval.exp));

    ;}
    break;

  case 81:
#line 1004 "../ccl-master/parser.y"
    {

      Expr * elambda = FUNC ( (yyvsp[(2) - (7)].str), (yyvsp[(6) - (7)].exp) );

      char buf[80];
      sprintf ( buf, "fmanglewahwah%d", foreach_index++ );

      Expr * body = 
        IFEXP ( 
	 BINEXP ( Expr::EQ, VAR("w"), EMPTY ),
	 EMPTY,
	 BINEXP ( 
             Expr::CONS, 
	     APP ( elambda, UNEXP ( Expr::HEAD, VAR("w") ) ),
             APP ( VAR(buf), UNEXP ( Expr::TAIL, VAR("w") ) ) 
           )
        );

      top_scope->add ( buf, new Value ( "w", body ) );
      top_scope->set_recursive ( buf );

      (yyval.exp) = APP ( VAR(buf), (yyvsp[(4) - (7)].exp) );
      SET_INFO((yyval.exp));
      free ( (yyvsp[(2) - (7)].str) );

    ;}
    break;

  case 82:
#line 1031 "../ccl-master/parser.y"
    {

      EXTERNAL_CCLI_FUNCTION m2c = lookup_ccl_function ( "map_to_cells" );

      Expr * body = (yyvsp[(2) - (3)].exp);

      std::list<char *> * fv = new std::list<char *>;
      std::list<char *>::iterator i;
      (yyvsp[(2) - (3)].exp)->freevars ( fv );
      
      for ( i=fv->begin(); i!=fv->end(); i++ ) {
        body = FUNC ( *i, body );
      }

      TypeExpr * R = new TypeExpr();
      std::list<TypeExpr *> * Ta = new std::list<TypeExpr *>;
      Ta->push_back ( new TypeExpr () );

      std::list<Expr *> * args = new std::list<Expr *>;
      args->push_back ( body );

      (yyval.exp) = EAPP ( new Value ( m2c, R, Ta ), args );

    ;}
    break;

  case 83:
#line 1060 "../ccl-master/parser.y"
    { 
      PUSH((yyvsp[(1) - (2)].str)); 
      (yyval.str) = (yyvsp[(1) - (2)].str);
    ;}
    break;

  case 84:
#line 1068 "../ccl-master/parser.y"
    {

      (yyval.st_list) = (yyvsp[(4) - (4)].st_list);
      (yyval.st_list)->push_back ( new Statement ( new Command ( VAR ( (yyvsp[(1) - (4)].str) ), (yyvsp[(2) - (4)].exp) ) ) );

    ;}
    break;

  case 85:
#line 1075 "../ccl-master/parser.y"
    {
 
      (yyval.st_list) = new std::list<Statement *>;
      (yyval.st_list)->push_front ( new Statement ( new Command ( VAR ( (yyvsp[(1) - (2)].str) ), (yyvsp[(2) - (2)].exp) ) ) );

    ;}
    break;

  case 86:
#line 1087 "../ccl-master/parser.y"
    {

      (yyval.elist) = (yyvsp[(1) - (3)].elist);
      (yyval.elist)->push_back ( (yyvsp[(3) - (3)].exp) );

    ;}
    break;

  case 87:
#line 1094 "../ccl-master/parser.y"
    { 

      (yyval.elist) = new std::list<Expr *>; 
      (yyval.elist)->push_back ( (yyvsp[(1) - (1)].exp) );

    ;}
    break;

  case 88:
#line 1101 "../ccl-master/parser.y"
    { (yyval.elist) = new std::list<Expr *>; ;}
    break;

  case 89:
#line 1106 "../ccl-master/parser.y"
    { (yyval.exp) = CONST ( (yyvsp[(1) - (1)].i) ); SET_INFO((yyval.exp)); ;}
    break;

  case 90:
#line 1108 "../ccl-master/parser.y"
    { (yyval.exp) = VAR ( (yyvsp[(1) - (1)].str) ); free((yyvsp[(1) - (1)].str)); SET_INFO((yyval.exp)); ;}
    break;

  case 91:
#line 1109 "../ccl-master/parser.y"
    { (yyval.exp) = CONST ( (yyvsp[(1) - (1)].real) ); SET_INFO((yyval.exp)); ;}
    break;

  case 92:
#line 1110 "../ccl-master/parser.y"
    { (yyval.exp) = CONST ( (yyvsp[(1) - (1)].str) ); SET_INFO((yyval.exp)); free ( (yyvsp[(1) - (1)].str) ); ;}
    break;

  case 93:
#line 1111 "../ccl-master/parser.y"
    { (yyval.exp) = CONST ( true ); SET_INFO((yyval.exp)); ;}
    break;

  case 94:
#line 1112 "../ccl-master/parser.y"
    { (yyval.exp) = CONST ( false ); SET_INFO((yyval.exp)); ;}
    break;

  case 95:
#line 1113 "../ccl-master/parser.y"
    { (yyval.exp) = (yyvsp[(2) - (3)].exp); SET_INFO((yyval.exp)); ;}
    break;

  case 96:
#line 1117 "../ccl-master/parser.y"
    {

	std::list<Expr::FIELD *>::iterator i;

	(yyval.exp) = RECEXP;
	SET_INFO((yyval.exp));

	for ( i = (yyvsp[(2) - (3)].field_list)->begin(); i != (yyvsp[(2) - (3)].field_list)->end(); i++ ) {
	  (yyval.exp)->addField ( (*i)->name, (*i)->e );
	  free ( (*i)->name );
          delete *i;
	}

	delete (yyvsp[(2) - (3)].field_list);

      ;}
    break;

  case 97:
#line 1138 "../ccl-master/parser.y"
    {

      (yyval.exp) = BINEXP ( Expr::CONS, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp) );
      SET_INFO((yyval.exp));

    ;}
    break;

  case 98:
#line 1145 "../ccl-master/parser.y"
    { 

       (yyval.exp) = BINEXP ( Expr::CONS, (yyvsp[(1) - (1)].exp), EMPTY ); 
       SET_INFO((yyval.exp));

    ;}
    break;

  case 99:
#line 1152 "../ccl-master/parser.y"
    { (yyval.exp) =  EMPTY; ;}
    break;

  case 100:
#line 1158 "../ccl-master/parser.y"
    {
	(yyval.field_list) = (yyvsp[(1) - (5)].field_list);
        Expr::FIELD * f = new Expr::FIELD;
	f->name = (yyvsp[(3) - (5)].str);
	f->e = (yyvsp[(5) - (5)].exp);
	(yyval.field_list)->push_back ( f );
      ;}
    break;

  case 101:
#line 1167 "../ccl-master/parser.y"
    {
        (yyval.field_list) = new std::list<Expr::FIELD *>; 
	Expr::FIELD * f = new Expr::FIELD;
	f->name = (yyvsp[(1) - (3)].str);
	f->e = (yyvsp[(3) - (3)].exp);
        (yyval.field_list)->push_back ( f );
      ;}
    break;

  case 102:
#line 1175 "../ccl-master/parser.y"
    { (yyval.field_list) = new std::list<Expr::FIELD *>; ;}
    break;

  case 103:
#line 1182 "../ccl-master/parser.y"
    {

      TypeExpr * T = RETRIEVE ( main_env, (yyvsp[(2) - (2)].str) );

      if ( T == NULL ) {

	PUSH ( (yyvsp[(2) - (2)].str) );
	(yyval.type) = main_env->get_type();

      } else 

	(yyval.type) = T;

      free ( (yyvsp[(2) - (2)].str) );

    ;}
    break;

  case 104:
#line 1199 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( Value::UNIT ); ;}
    break;

  case 105:
#line 1200 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( Value::BOOLEAN ); ;}
    break;

  case 106:
#line 1201 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( Value::REAL ); ;}
    break;

  case 107:
#line 1202 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( Value::INTEGER ); ;}
    break;

  case 108:
#line 1203 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( Value::STRING ); ;}
    break;

  case 109:
#line 1204 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( (yyvsp[(1) - (2)].type) ); ;}
    break;

  case 110:
#line 1205 "../ccl-master/parser.y"
    { (yyval.type) = (yyvsp[(2) - (3)].type); ;}
    break;

  case 111:
#line 1206 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( true ); ;}
    break;

  case 112:
#line 1207 "../ccl-master/parser.y"
    { rec_extensible_flag = false; ;}
    break;

  case 113:
#line 1207 "../ccl-master/parser.y"
    { 
      (yyval.type) = new TypeExpr ( (yyvsp[(3) - (4)].env) ); 
      (yyval.type)->set_extensibility ( rec_extensible_flag );
    ;}
    break;

  case 114:
#line 1212 "../ccl-master/parser.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); ;}
    break;

  case 115:
#line 1220 "../ccl-master/parser.y"
    {

      (yyval.env) = new Environment ( (yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].type), (yyvsp[(5) - (5)].env) );
      free ( (yyvsp[(1) - (5)].str) );

    ;}
    break;

  case 116:
#line 1229 "../ccl-master/parser.y"
    {

      (yyval.env) = new Environment ( (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].type), NULL );
      free ( (yyvsp[(1) - (3)].str) );

    ;}
    break;

  case 117:
#line 1238 "../ccl-master/parser.y"
    {
      (yyval.env) = (yyvsp[(3) - (3)].env);
      rec_extensible_flag = true;
    ;}
    break;

  case 118:
#line 1245 "../ccl-master/parser.y"
    {
      (yyval.env) = NULL;
      rec_extensible_flag = true;
    ;}
    break;

  case 119:
#line 1250 "../ccl-master/parser.y"
    { (yyval.env) = NULL; ;}
    break;

  case 120:
#line 1256 "../ccl-master/parser.y"
    { (yyval.type) = new TypeExpr ( (yyvsp[(1) - (3)].type), (yyvsp[(3) - (3)].type) ); ;}
    break;

  case 121:
#line 1264 "../ccl-master/parser.y"
    {

      (yyval.type_list) = (yyvsp[(3) - (3)].type_list);
      (yyval.type_list)->push_front ( (yyvsp[(1) - (3)].type) );

    ;}
    break;

  case 122:
#line 1273 "../ccl-master/parser.y"
    {

      (yyval.type_list) = new std::list<TypeExpr *>;
      (yyval.type_list)->push_back ( (yyvsp[(1) - (1)].type) );

    ;}
    break;

  case 123:
#line 1280 "../ccl-master/parser.y"
    { (yyval.type_list) =  new std::list<TypeExpr *>; ;}
    break;


/* Line 1267 of yacc.c.  */
#line 3199 "../ccl-master/parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1284 "../ccl-master/parser.y"


int yyerror ( char * s ) { 

    std::stringstream strm;
    strm << s << ": near line " << lineNum[include_stack_ptr] << " of " << current_file_name[include_stack_ptr];

    throw ( strm.str() );

    return -1;

}

extern FILE * yyin;

extern void RegisterCCLInternals ( void );

int readOrganismProgram ( Scope * sc, const char * path ) {

  RegisterCCLInternals();

  int i;

  top_scope = sc;

  if ( path == NULL )
    path = strdup ( "(null)" );

  if ( ( yyin = fopen ( path, "r" ) ) == NULL ) {
    ERROR ( "gro is unable to open file named <i>" << path << "</i>" );
  }

  yyrestart ( yyin );

  include_stack_ptr = 0;

  for ( i=0; i<MAX_INCLUDE_DEPTH+1; i++ ) 
    lineNum[i] = 1;  

  current_file_name[0] = strdup ( path );

  try {
    i = yyparse();
  }

  catch ( std::string err ) {
     fclose ( yyin );
     throw err;
  }

  fclose ( yyin );

  while ( !ccl_function_list.empty() ) {
    delete ccl_function_list.front();
    ccl_function_list.pop_front();
  }

  return i;

}

void register_ccl_function ( std::string name, EXTERNAL_CCLI_FUNCTION f ) {

  InternalFunction * inf = new InternalFunction;

  inf->name = name;
  inf->f = f;

  ccl_function_list.push_back ( inf );

}

EXTERNAL_CCLI_FUNCTION lookup_ccl_function ( std::string name ) {

  std::list<InternalFunction *>::iterator i;

  for ( i = ccl_function_list.begin(); i != ccl_function_list.end(); i++ ) {

    if ( (*i)->name == name )
      return (*i)->f;
  }

  return NULL;

}

