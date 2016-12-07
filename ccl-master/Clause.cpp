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
#include "Program.h"

#define NL printf ( "\n" ); fflush ( stdout )

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
}

Command::Command ( Expr * l, Expr * r ) : lhs(l), rhs(r) {}

Command::~Command ( void ) {

  if ( lhs != NULL ) delete lhs;
  delete rhs;

}

void Command::apply ( Scope * s ) {

#ifdef DEBUB
  printf ( "evaluating " ); print(); 
  printf ( "\non line %d of %s\n", 
	   rhs->line_num(), 
	   rhs->file_name() == NULL ? "no file info" : rhs->file_name() );
#endif

  Value * answer = rhs->eval ( s );

  if ( lhs == NULL )
    delete answer;
  else
    s->set ( lhs, answer );

}

void Command::print ( void ) {

  if ( lhs == NULL ) 
    printf ( ". := " );
  else {
    lhs->print();
    printf ( " := " );
  }

  rhs->print();

}

Command * Command::copy ( void ) {

  if ( lhs != NULL ) 
    return new Command ( lhs->copy(), rhs->copy() );
  else
    return new Command ( NULL, rhs->copy() );

}

Command * Command::subst ( Expr * e, char * x ) {

  return ( new Command ( lhs->subst ( e, x ), rhs->subst ( e, x ) ) );

}

Clause::Clause ( Expr * g ) : guard ( g ) {

  commands = new std::list<Command*>;

}

Clause::Clause ( Expr * g, std::list<Command *> * coms ) : guard ( g ), commands ( coms ) {}

Clause::~Clause ( void ) {

  delete guard;

  for (  i = commands->begin(); i != commands->end(); i++ ) 
    delete (*i);

  delete commands;

}

void Clause::execute ( Scope * s ) {

#ifdef DEBOG
  printf ( "evaluating guard \"" ); guard->print(); 
  printf ( "\"\non line %d of %s\n", 
	   guard->line_num(), 
	   guard->file_name() == NULL ? "no file info" : guard->file_name() );
#endif

  Value * gval = guard->eval ( s );

  if ( gval == NULL ) {
    fprintf ( stderr, "Clause::execute: gval = NULL\n" );
    exit ( -1 );
  }

  if ( gval->bool_value() ) {
    std::list<Command *>::iterator i;
    for ( i = commands->begin(); i != commands->end(); i++ ) {
      (*i)->apply ( s );
    }
  }
  delete gval;

}

Clause * Clause::copy ( void ) {

  Clause * c = new Clause ( guard->copy() );

  for ( i = commands->begin(); i != commands->end(); i++ ) 
    c->addCommand ( (*i)->copy() );

  return c;

}

Clause * Clause::subst ( Expr * e, char * x ) {

  Clause * c = new Clause ( guard->subst(e,x) );

  for ( i = commands->begin(); i != commands->end(); i++ ) 
    c->addCommand ( (*i)->subst(e,x) );

  return c;

}

void Clause::print ( void ) {

  printf ( "    " ); guard->print();
  printf ( ": " );
  if ( commands->size() > 1 )
    printf ( "\n      " );

  for ( i = commands->begin(); i != commands->end(); i++ ) {
    if ( i != commands->begin() ) 
      printf ( "      " ); 
    (*i)->print(); NL;
  }

}

void Clause::print_as_init ( void ) {

  for ( i = commands->begin(); i != commands->end(); i++ ) {
    printf ( "    " ); 
    (*i)->print(); NL;
  }

}

void Clause::addCommand ( Command * c ) {

  commands->push_back ( c );

}

bool Command::is_assigned ( const char * var ) {

  if ( lhs->get_type() == Expr::VARIABLE && !strcmp ( lhs->get_name(), var ) )
    return true;
  else if ( lhs->get_type() == Expr::VARIABLE && lhs->get_op() == '.' &&
            lhs->get_left()->get_type() == Expr::VARIABLE && 
            !strcmp ( lhs->get_left()->get_name(), var ) )
    return true;
  else
    return false;

}

bool Clause::is_assigned ( const char * var ) {

  std::list<Command *>::iterator i;

  for ( i=commands->begin(); i!=commands->end(); i++ ) {
    if ( (*i)->is_assigned ( var ) )
      return true;
  }

  return false;

}

