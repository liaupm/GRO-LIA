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
#include <list>
#include "SymbolTable.h"
#include "Expr.h"

void Scope::push ( SymbolTable * s ) {

  s->next = head;
  head = s;

}

void Scope::push_back ( SymbolTable * s ) {

  if ( head == NULL ) {

    head = s;
    head->next = NULL; // should already be null, but hey

  } else {

    SymbolTable * temp = head;

    while ( temp->next != NULL )
      temp = temp->next;

    s->next = NULL;
    temp->next = s;

  }

}

void Scope::pop ( void ) {

  if ( head != NULL ) 
    head = head->next;

}

void Scope::pop_all ( void ) {

  while ( head != NULL )
    head = head->next;
 
}

Value * Scope::get ( const char * name ) {

  SymbolTable * temp = head;
  Value * v;

  while ( temp != NULL ) {
    v = temp->get ( name );
    if ( v != NULL )
      return v;
    else
      temp = temp->next;
  }

  return NULL;

}

Value * Scope::get_previous ( const char * name ) {

  SymbolTable * temp = head;
  Value * v;

  while ( temp != NULL ) {
    v = temp->get_previous ( name );
    if ( v != NULL )
      return v;
    else
      temp = temp->next;
  }

  return NULL;

}

void Scope::add ( const char * str, Value * v ) {

  head->add ( str, v );
  
}

bool Scope::assign (  const char * name, Value * v ) {

  SymbolTable * temp = head;
  Value * val;

  while ( temp != NULL ) {
    val = temp->get ( name );
    if ( val != NULL ) {
      temp->assign ( name, v );
      return true;
    } else
      temp = temp->next;
  }

  return false;

}

void Scope::set_recursive ( const char * name ) {

  SymbolTable * temp = head;
  Symbol * sym;

  while ( temp != NULL ) {
    sym = temp->get_symbol ( name );
    if ( sym != NULL ) {
      sym->set_recursive();
      return;
    } else
      temp = temp->next;
  }

}

bool Scope::is_recursive ( const char * name ) {


  SymbolTable * temp = head;
  Symbol * sym;

  while ( temp != NULL ) {
    sym = temp->get_symbol ( name );
    if ( sym != NULL ) {
      return sym->is_recursive();
    } else
      temp = temp->next;
  }

  return false;

}

void Scope::print ( FILE * fp ) {

  SymbolTable * temp = head;

  while ( temp != NULL ) {
    temp->print(fp);
    temp = temp->next;
  }

}

void Scope::print_names ( FILE * fp ) {

  SymbolTable * temp = head;
  int indent = 0;

  while ( temp != NULL ) {
    temp->print_names(fp,indent);
    temp = temp->next;
    if ( temp != NULL ) {
      indent += 2;
    }
  }

}

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
}

Scope * Scope::copy ( void ) {

  Scope * s = new Scope();
  SymbolTable * temp = head;

  while ( temp != NULL ) {
    s->push_back ( temp->copy() );
    temp = temp->next;
  }
  return s;

}

std::list<SymbolTable *> * Scope::to_list ( void ) {

  std::list<SymbolTable *> * L = new std::list<SymbolTable *>;

  SymbolTable * temp = head;

  while ( temp != NULL ) {

    L->push_back ( temp );
    temp = temp->next;

  }

  return L;

}

Scope::~Scope ( void ) {

  SymbolTable * temp = head, * next;

  while ( temp != NULL ) {
    next = temp->next;
    delete temp;
    temp = next;

  }

}

void Scope::set ( Expr * lhs, Value * v ) {

  if ( lhs->get_type() == Expr::VARIABLE ) {

    assign ( lhs->get_name(), v );

  } else {

    Value * lookup = lhs->get_lhs_value ( this );
    ASSERT ( lookup != NULL );
    lookup->reset ( v );
    delete v;

  }

}

void Scope::add ( const char * str, Program * p ) {

  head->add ( str, p );

}

Program * Scope::get_program ( const char * name ) {

  SymbolTable * temp = head;
  Program * p;

  while ( temp != NULL ) {
    p = temp->get_program ( name );
    if ( p != NULL )
      return p;
    else
      temp = temp->next;
  }

  return NULL;

}

void Scope::add_by_symbols ( SymbolTable * sym ) {

  int i;
  Symbol * temp;

  for ( i=0; i<sym->num_buckets; i++ ) 
    for ( temp=sym->buckets[i]; temp!=NULL; temp=temp->next ) 
      add ( temp->get_name(), temp->get_value()->copy() );

}
