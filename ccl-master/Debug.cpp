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

#include <stdio.h>
#include <stdlib.h>
#ifndef WIN
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include "Program.h"

void Command::debug_apply ( Scope * s ) {

  printf ( "\n\n%s, %d:\n", 
	   rhs->file_name() == NULL ? "no file info" : rhs->file_name(), 
	   rhs->line_num() );
  print();
  printf ( "\n" );

  Clause::get_debug_command(s);

  Value * answer = rhs->eval ( s );

  if ( lhs == NULL )
    delete answer;
  else
    s->set ( lhs, answer );

}

void Clause::debug_execute ( Scope * s ) {

  printf ( "\n%s, %d:\n", 
	   guard->file_name() == NULL ? "no file info" : guard->file_name(),
	   guard->line_num() );
  guard->print(); 
  printf ( " <<guard>>\n" );

  get_debug_command(s);

  Value * gval = guard->eval ( s );

  if ( gval == NULL ) {
    fprintf ( stderr, "Clause::execute: gval = NULL\n" );
    exit ( -1 );
  }

  if ( gval->bool_value() ) { 

    printf ( "guard evaluated to true, executing clauses" );

    for ( i = commands->begin(); i != commands->end(); i++ ) 
      (*i)->debug_apply ( s );

  } else {

    printf ( "\nguard evaluated to false, continuing\n" );

  }

  delete gval;

}

void Clause::get_debug_command ( Scope * s ) {

  char * com;

  while ( 1 ) {

#ifndef WIN
    com = readline ( "> " );
    if ( com && *com )
      add_history ( com );
#endif

    switch ( com[0] ) {

    case 's': // step
    case ' ':
    case 0:
      free ( com );
      return;

    case 'p': // print a symbol

      if ( strlen ( com ) > 2 ) {

	Value * v = s->get ( com+2 );
	if ( v == NULL ) 
	  printf ( "symbol '%s' not found\n", com+2 );
	else {
	  printf ( "%s: " );
	  v->print();
	  printf ( "\n" );
	}
	
      } else {
	printf ( "command 'p' requires an argument\n" );
      }
      break;

    case 't': // print top level symbol tables
      {
	int n = atoi ( com + 2 ), i = 0;
	SymbolTable * temp = s->get_top();

	while ( i < n && temp != NULL ) {
	  temp->print_names ( stdout, 2*i );
	  temp = temp->next;
	  i++;
	}
      }

      break;

    case 'T': // print scope
      s->print_names ( stdout );
      break;

    case 'q': // quit
      exit ( 0 );
      break;

    case 'l': // clear screen
      system ( "clear" );
      break;

    case 'h': // help

      printf ( "\n" );
      printf ( "        debugger commands\n" );
      printf ( "        ---------------------------------------------------\n" );
      printf ( "        s         step ( also ' ' and \\n )\n" );
      printf ( "        p var     print the value of the variable var\n" );
      printf ( "        t n       print symbols in top n scopes\n" );
      printf ( "        T         print symbols in all scopes\n" );
      printf ( "        q         quit\n" );
      printf ( "        l         clear the screen\n" );
      printf ( "        h         this help message\n\n" );

      break;         

    default:
      printf ( "unrecongnized command: '%s'\n", com );
      break;

    }

    free ( com );

  }

}
