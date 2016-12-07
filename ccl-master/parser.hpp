/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1529 of yacc.c.  */
#line 212 "../ccl-master/parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

