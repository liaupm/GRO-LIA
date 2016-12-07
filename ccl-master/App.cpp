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
#include "Expr.h"

char * var_compare_val;

static void init_var_compare ( char * v ) {

  var_compare_val = v;

}

static bool var_compare ( char * v ) {

  return strcmp ( var_compare_val, v ) == 0;

}

static void remove_from_list ( char * v, std::list<char *> * L ) {

  init_var_compare ( v );
  L->remove_if ( var_compare );

}

void Expr::freevars ( std::list<char *> * fr ) {

  switch ( type ) {

  case CONSTANT:
    if ( val->get_type() == Value::FUNCTION ) {
      val->func_body()->freevars ( fr );
      remove_from_list ( val->func_arg_name(), fr );
    }
    break;

  case VARIABLE:
    fr->push_back ( name );
    break;

  case APPLICATION:
    e1->freevars ( fr );
    e2->freevars ( fr );
    break;

  case UNARY:
    e1->freevars ( fr );
    break;

  case BINARY:
    e1->freevars ( fr );
    if ( op != DOT )
      e2->freevars ( fr );
    break;

  case LET:
    e2->freevars ( fr );
    remove_from_list ( name, fr );
    e1->freevars ( fr );
    break;

  case IF:
    e1->freevars ( fr );
    e2->freevars ( fr );
    e3->freevars ( fr );
    break;

  case RECORD:
    {
      std::list <FIELD *>::iterator i;
      for ( i = fields->begin(); i != fields->end(); i++ )
	(*i)->e->freevars ( fr );
    }
    break;

   case EX_APPLICATION:
     {
       std::list<Expr *>::iterator i;

       for ( i = arglist->begin(); i != arglist->end(); i++ )
         (*i)->freevars ( fr );

     }
     break;

  }

  return;

}

bool in_free_vars ( char * v, Expr * e ) {

  std::list<char *> L;
  std::list<char *>::iterator i;
  bool found = false;
  e->freevars ( &L );

  for ( i = L.begin(); i != L.end(); i++ )
    if ( !strcmp ( *i, v ) ) {
      found = true;
      break;
    }

  return found;

}

static int new_var_name_count = 0;

char * Expr::new_var_name ( const char * v ) {

  char * buf = (char *) calloc ( 20 + strlen(v), sizeof ( char ) );
  sprintf ( buf, "%s:%d", v, new_var_name_count++ );
  //printf ( "               new var: '%s'\n", buf );

  return buf;

}

Expr * Expr::subst ( Expr * e, char * x ) {

  //printf ( "trying to substitute " ); e->print(); printf ( " for %s", x ); 
  //printf ( " in " ); print(); printf ( "\n" );

  Expr * f;

  switch ( type ) {

  case CONSTANT:

    if ( val->get_type() == Value::FUNCTION ) {

      char * arg = val->func_arg_name();
      Expr * body = val->func_body();

      if ( !strcmp ( x, arg ) )

	f = copy();

      else {

	if ( in_free_vars ( arg, e ) ) {

	  char * z = new_var_name ( arg );
	  Expr * z_exp = VAR ( z );
	  Expr * new_body = body->subst ( z_exp, arg );
	  f = FUNC ( z, new_body->subst ( e, x ) );
	  delete z_exp;
	  delete new_body;

	} else

	  f = FUNC ( arg, body->subst ( e, x ) );

      }

    } else 

      f = copy();

    break;

  case VARIABLE:
    if ( !strcmp ( x, name ) )
      f = e->copy();
    else
      f = copy();
    break;

  case APPLICATION:
    f = APP ( e1->subst ( e, x ), e2->subst ( e, x ) );
    break;

  case UNARY:
    f = UNEXP ( op, e1->subst ( e, x ) );
    break;

  case BINARY:
    if ( op != DOT )
      f = BINEXP ( op, e1->subst ( e, x ), e2->subst ( e, x ) );
    else
      f = BINEXP ( op, e1->subst ( e, x ), e2->copy() );
    break;

  case LET:

    if ( !strcmp ( x, name ) )

      f = LETEXP ( name, e1->subst ( e, x ), e2->copy() );

    else if ( in_free_vars ( name, e ) ) {

      char * z = new_var_name ( name );
      Expr * z_exp = VAR ( z );
      Expr * new_e2 = e2->subst ( z_exp, name );
      f = LETEXP ( z, e1->subst ( e, x ), new_e2->subst ( e, x ) );
      delete z_exp;
      delete new_e2;
      free ( z );

    } else

      f = LETEXP ( name, e1->subst ( e, x ), e2->subst ( e, x ) );

    break;

  case IF:

    f = IFEXP ( e1->subst ( e, x ), e2->subst ( e, x ), e3->subst ( e, x ) );
    break;

  case RECORD:

    {
      std::list<FIELD *>::iterator i;
      f = RECEXP;
      for ( i = fields->begin(); i != fields->end(); i++ )
	f->addField ( (*i)->name, (*i)->e->subst ( e, x ) );
    }
    break;

  case EX_APPLICATION:
    {

      std::list<Expr *> * elist = new std::list<Expr *>;
      std::list<Expr *>::iterator i;

      for ( i = arglist->begin(); i != arglist->end(); i++ )
        elist->push_back ( (*i)->subst ( e, x ) );

      f = EAPP ( val->copy(), elist );

    }
    break;

  case PROGRAM:
    f = copy();
    break;

  }

  // printf ( "  returning " ); f->print(); printf ( "\n" );

  return f;

}
