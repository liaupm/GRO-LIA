%x incl
%x string_const

%{

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

#include <string.h>
#include <string>
#include "Expr.h"
#include "Program.h"
#include "parser.h"
#include "parser.hpp"

int lineNum[MAX_INCLUDE_DEPTH+1];   /* keeps track of the line number */

YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

char * current_file_name[MAX_INCLUDE_DEPTH+1];

std::string lex_str_const;

int str_line_start;

void foo ( void ) { return; }


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

%}

VAR [a-zA-Z_][a-zA-Z0-9_]*
INT [0-9]+
REAL ([0-9]+)|([0-9]*"."[0-9]+(E[+-][0-9]{1,2}){0,1})
STRING \"[^\"]*\"

%%

  /*
   * keywords
   */

include        BEGIN(incl);

  /*
   * punctuation
   */

":"            { return t_COLON; }
";"            { return t_SEMICOLON; }
","            { return t_COMMA; }
"("            { return t_LPAREN; }
")"            { return t_RPAREN; }
"{"            { return t_LCURLY; }
"}"            { return t_RCURLY; }
"[|"           { return t_LBBRACKET; }
"|]"           { return t_RBBRACKET; }
"["            { return t_LBRACKET; }
"]"            { return t_RBRACKET; }
"*"            { return t_MULT; }
"/"            { return t_DIV; }
"+"            { return t_PLUS; }
"-"            { return t_MINUS; }
"%"            { return t_MOD; }
"^"            { return t_POWER; }
"|"            { return t_OR; }
"&"            { return t_AND; }
"!"            { return t_NOT; }
"<<"           { return t_RECORD_ADD; }
"<"            { return t_LESS_THAN; }
">"            { return t_GREATER_THAN; }
">="           { return t_GREATER_THAN_OR_EQUAL; }
"<="           { return t_LESS_THAN_OR_EQUAL; }
"="            { return t_EQUAL; }
"!="           { return t_NOT_EQUAL; }
":="           { return t_ASSIGN; }
"."            { return t_DOT; }
"@"            { return t_CONS; }
"#"            { return t_CONCAT; }
"\\"           { return t_LAMBDA; }
"<>"           { return t_STRCAT; }
"lambda"       { return t_LAMBDA; }
"true"         { return t_TRUE; }
"false"        { return t_FALSE; }
"let"          {  return t_LET; }
"in"           { return t_IN; }
"foreach"      { return t_FOREACH; }
"do"           { return t_DO; }
"end"          { return t_END; }
"maptocells"   { return t_MAP_TO_CELLS; }
"if"           { return t_IF; }
"then"         { return t_THEN; }
"else"         { return t_ELSE; }
"fun"          { return t_FUN; }
"head"         { return t_HEAD; }
"tail"         { return t_TAIL; }
"external"     { return t_EXTERNAL; }
"internal"     { return t_INTERNAL; }
"program"      { return t_PROGRAM; }
"sharing"      { return t_SHARING; }
"compose"      { return t_COMPOSE; }
"needs"        { return t_NEEDS; }
"exec"         { return t_EXEC; }

"'"            { return t_BACKQUOTE; }
"unit"         { return t_UNIT; }
"bool"         { return t_BOOL; }
"int"          { return t_INT; }
"real"         { return t_REAL; }
"string"       { return t_STRING; }
"list"         { return t_LIST; }
"..."          { return t_VARARGS; }
"->"           { return t_INTO; }

  /*
   * identifiers
   */

{VAR}         {
                 yylval.str = strdup ( yytext );
                 return t_VAR; 
              }
  /*
{STRING}  {

            int i;

            yylval.str = (char *) calloc ( strlen ( yytext ) - 1, sizeof ( char ) );

            for ( i = 0; i < strlen ( yytext ) -  2; i++ )
              yylval.str[i] = yytext[i+1];

            yylval.str[i] = 0;

            //printf ( "read string: '%s'\n", yylval.str );
            
            return t_STRING;

          }
  */

{INT} { 
        yylval.i = atoi ( yytext );
        return t_INTEGER; 
      }

{REAL} { yylval.real = atof ( yytext ); return t_REAL; }

  /*
   * strings
   */

\"  { 
      BEGIN(string_const); 
      lex_str_const = "";
      str_line_start = lineNum[include_stack_ptr];
      foo();
    } 

<string_const>\\a { lex_str_const += '\a'; }
<string_const>\\b { lex_str_const += '\b'; }
<string_const>\\f { lex_str_const += '\f'; }
<string_const>\\n { lex_str_const += '\n'; }
<string_const>\\r { lex_str_const += '\r'; }
<string_const>\\t { lex_str_const += '\t'; }
<string_const>\\v { lex_str_const += '\v'; }
<string_const>\\\\ { lex_str_const += '\\'; }
<string_const>\\\n { }
<string_const>"\n" { lex_str_const += '\n'; lineNum[include_stack_ptr]++; }
<string_const><<EOF>> { 

      std::stringstream strm;
      strm << "Parse error.<blockquote>"
           << "Unterminated string constant in file " << current_file_name[include_stack_ptr] << " starting near line " << str_line_start
           << ".</blockquote>";
      throw strm.str();

}
<string_const>[^\"] { lex_str_const += yytext; }

<string_const>\" {

  yylval.str = strdup ( lex_str_const.c_str() );
  BEGIN(INITIAL);
  return t_STRING;

}


  /*
   * numbersyylval.str
   */

  /*
   * one line comments ( increase the line counter )
   */

"//"[^\n]*"\n" { lineNum[include_stack_ptr]++; }

"//"[^\r]*"\r" { lineNum[include_stack_ptr]++; }

  /*
   * C style comments
   */

"/*" {

    register int c;

    for ( ; ; ) {

      while ( (c = yyinput()) != '*' && c != EOF ) {
        if ( c == '\n' ) lineNum[include_stack_ptr]++;
      }

      if ( c == '*' ) {

        while ( (c = yyinput()) == '*' );

        if ( c == '/' )
          break;    /* found the end */

      }

      if ( c == EOF ) {
        ERROR ( "End of file encountered before the end of a C-style comment." );
      }

    }

  }


  /*
   * newlines increase the line counter
   */

"\n"           { lineNum[include_stack_ptr]++; }
"\r"           { lineNum[include_stack_ptr]++; }

[ \t]+         /* Ignore White Space */

.              {
                 fprintf ( stderr, "line %d of '%s': unrecognized token (%s)\n",
                           lineNum[include_stack_ptr], current_file_name[include_stack_ptr], yytext );
               }

<incl>[ \t]*     /* Ignore whitespace */

<incl>[^ \t\n\r]+  {

                   if ( include_stack_ptr >= MAX_INCLUDE_DEPTH ) {
                     ERROR ( "Includes nested too deeply, redefine MAX_INCLUDE_DEPTH." );
                   }

                   include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;

                   std::string name;

                   if ( yytext != NULL ) {
                     name = yytext;
                   } else {
                     ERROR ( "yytext is NULL." );
                   }

                   name = name + ".gro";

                   current_file_name[include_stack_ptr] = strdup ( name.c_str() );

                   yyin = fopen( name.c_str(), "r" );

                   if ( ! yyin ) {

                     std::string path = "include/" + name;
                     //printf ( "opening %s\n", path.c_str() ); fflush(stdout);
                     yyin = fopen( path.c_str(), "r" );

                   }


                   if ( ! yyin ) {

                       std::stringstream strm;

                       strm << "Error. <blockquote>Could not find include file <i>" << name.c_str()
                            << "</i>, on line " << lineNum[include_stack_ptr]
                            << " of <i>" << current_file_name[include_stack_ptr-1] << "</i>.</blockquote>";
                       BEGIN(INITIAL);
                       include_stack_ptr = 0;
                       throw strm.str();

                   }

                   yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) ); /* changes YY_CURRENT_BUFFER */
                   lineNum[include_stack_ptr] = 1;

                   BEGIN(INITIAL);

                 }

<<EOF>>          {

                   if ( --include_stack_ptr < 0 ) {

                     include_stack_ptr = 0;
                     yyterminate();

                   } else {

                      yy_delete_buffer( YY_CURRENT_BUFFER );
                      yy_switch_to_buffer( include_stack[include_stack_ptr] );
                      // free ( current_file_name[include_stack_ptr+1] );
                       
                   }

                 }



%%

#ifndef yywrap
  int yywrap() { return( 1 ); }
#endif 






