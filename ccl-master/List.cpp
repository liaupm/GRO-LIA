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
#include "SymbolTable.h"
#include "Expr.h"

#define LISTVAL data.list_val.vals

void Value::set ( std::list<Value*> * L ) {

  if ( type != LIST ) {
    fprintf ( stderr, "Value::set ( list ): attempted to get list type of non-LIST symbol\n" );
    exit ( -1 );
  } else {
    delete LISTVAL;
    LISTVAL = L;
  }

}

std::list<Value*> * Value::list_value ( void ) {

  if ( type != LIST ) {
    fprintf ( stderr, "Value::list_value ( void ): attempted to access LIST value of non-LIST symbol '" );
    print ( stderr ); fprintf ( stderr, "'\n" );
    exit ( -1 );
  }

  return LISTVAL;

}

Value * Expr::list_head ( Scope * s ) {

  Value * temp = e1->eval ( s ), * h;

  if ( temp->get_type() != Value::LIST ) {
    fprintf ( stderr, "Expr::list_head: attempted to take head of non-list\n" );
    exit ( -1 );
  }

  if ( temp->list_value()->size() == 0 ) {
      RUNTIME_ERROR(file,line,"Attempted to take head of an empty list.")
  }

  h = temp->list_value()->front()->copy();
  delete temp;

  return h;

}

Value * Expr::list_tail ( Scope * s ) {

  Value * temp = e1->eval ( s );

  if ( temp->get_type() != Value::LIST ) {
    fprintf ( stderr, "Expr::list_head: attempted to take head of non-list\n" );
    exit ( -1 );
  }

  if ( temp->list_value()->size() == 0 ) {
       RUNTIME_ERROR(file,line,"Attempted to take tail of an empty list.")
  }

  delete temp->list_value()->front();
  temp->list_value()->pop_front();

  return temp;

}

Value * Expr::list_cons ( Scope * s ) {

  Value * temp1 = e1->eval ( s ),
        * temp2 = e2->eval ( s );

  if ( temp2->get_type() != Value::LIST ) {
    fprintf ( stderr, "Expr::list_cons: attempted to cons to a non-list '" );
    temp1->print ( stderr ); printf ( "' and\n'" );
    temp2->print ( stderr ); printf ( "'\n" );
    exit ( -1 );
  }

  temp2->list_value()->push_front ( temp1 );

  return temp2;

}

Value * Expr::list_join ( Scope * s ) {

  Value * temp1 = e1->eval ( s ),
        * temp2 = e2->eval ( s );

  std::list<Value *>::iterator i;

  if ( temp1->get_type() != Value::LIST || temp2->get_type() != Value::LIST ) {
    fprintf ( stderr, "Expr::list_join: attempted to join non-lists\n" );
    exit ( -1 );
  }

  for ( i = temp2->list_value()->begin(); i != temp2->list_value()->end(); i++ )
    temp1->list_value()->push_back(*i);

  while ( temp2->list_value()->size() > 0 )
    temp2->list_value()->pop_front();

  delete temp2;

  return temp1;

}

Value * Expr::list_equal ( Value * v1, Value * v2, Scope * s ) {

  bool rval = true;
  std::list<Value *>::iterator i, j;

  if ( v1->list_value()->size() != v2->list_value()->size() ) 

    rval = false; 

  else for ( i=v1->list_value()->begin(), j=v2->list_value()->begin();
	i != v1->list_value()->end() && j != v2->list_value()->end();
	i++, j++ ) {

    rval = Value::equal ( *i, *j, s );

    if ( !rval )
      break;

  }

  return new Value ( rval );

}

bool Value::equal ( Value * v1, Value * v2, Scope * s ) {

  bool answer;

  if ( v1->type != v2->type ) 

    answer = false;

  else {

    switch ( v1->type ) {

      case UNIT:

	answer = true;
	break;

      case BOOLEAN:

	answer = ( v1->bool_value() == v2->bool_value() );
	break;

      case INTEGER:

	answer = ( v1->int_value() == v2->int_value() );
	break;

      case REAL:

	answer = ( v1->real_value() == v2->real_value() );
	break;

      case STRING:

	answer = ( v1->string_value() == v2->string_value() );
	break;

      case LIST:

	answer = Expr::list_equal ( v1, v2, s );
	break;

      case RECORD:
      case FUNCTION:
      case EX_FUNCTION:

	ASSERT ( false );
	break;

    }

  }

  return answer;

}
