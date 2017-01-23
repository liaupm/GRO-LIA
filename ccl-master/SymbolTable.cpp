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
#include <math.h>
#include "SymbolTable.h"
#include "Program.h"

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
}

SymbolTable::SymbolTable ( int n ) : num_buckets(n), num_symbols(0) {

  int i;

  buckets = new Symbol * [n];
  for ( i=0; i<n; i++ ) 
    buckets[i] = NULL;

  next = NULL;

}

SymbolTable::~SymbolTable ( void ) {

  int i;
  Symbol * temp;

  for ( i=0; i<num_buckets; i++ ) 
    while ( buckets[i] != NULL ) {
      temp = buckets[i];
      buckets[i] = temp->next;
      delete temp; 

    }

  delete[] buckets;

}

int SymbolTable::hash ( const char * str ) {

  int i, sum=0;

  for ( i=0; i<strlen(str); i++ )
    sum += str[i];

  return ( sum % num_buckets );

}

void SymbolTable::add ( const char * name, Value * v ) {

  int index = hash ( name ); 
  Symbol * temp = new Symbol ( name, v );

  temp->next = buckets[index];
  buckets[index] = temp;
  num_symbols++;

}

void SymbolTable::del ( const char * name ) {

  if ( get ( name ) != NULL ) {

    Symbol * temp, * prev;

    int index = hash ( name );

    for ( temp = buckets[index], prev = temp; temp != NULL; temp = temp->next ) {

//    printf ( "name = %s, temp = %x, temp->get_name() = %s\n", name, temp, temp->get_name() );

      if ( !strcmp ( temp->get_name(), name ) ) {

	num_symbols--;

        if ( prev == temp ) {

	  buckets[index] = temp->next;
	  delete temp; 
	  return;

        } else {

	  prev->next = temp->next;
          delete next;

        }

      }

      prev = temp;

    }

  }

}

Value * SymbolTable::get ( const char * name ) {

  int index = hash ( name ); 
  Symbol * temp;

  for ( temp = buckets[index]; temp != NULL; temp = temp->next )
    if ( !strcmp ( temp->get_name(), name ) ) return temp->get_value();

  return NULL;

}

Value * SymbolTable::get_previous ( const char * name ) {

  int index = hash ( name ); 
  Symbol * temp;

  for ( temp = buckets[index]; temp != NULL; temp = temp->next )
    if ( !strcmp ( temp->get_name(), name ) ) return temp->get_previous();

  return NULL;

}

Symbol * SymbolTable::get_symbol ( const char * name ) {

  int index = hash ( name ); 
  Symbol * temp;

  for ( temp = buckets[index]; temp != NULL; temp = temp->next )
    if ( !strcmp ( temp->get_name(), name ) ) return temp;

  return NULL;

}

bool SymbolTable::assign ( const char * name, Value * v ) {

  int index = hash ( name ); 
  Symbol * temp = NULL;

  for ( temp = buckets[index]; temp != NULL; temp = temp->next ) 
    if ( !strcmp ( temp->get_name(), name ) ) 
      break;

  if ( temp == NULL ) 

    return false;

  else {

    if ( temp->v != temp->previous ) {

      delete temp->previous;
      temp->previous = temp->v;
      temp->v = v;

    } else if ( temp->previous->get_type() == Value::UNIT ) {

      delete temp->previous;
      temp->previous = v;
      temp->v = v;

    } else {

      temp->v = v;

    }

    return true;

  }

}

void SymbolTable::replace ( SymbolTable * s ) {

  int i;
  Symbol * temp;

  for ( i=0; i<s->num_buckets; i++ ) {

    for ( temp = s->buckets[i]; temp != NULL; temp=temp->next ) {

      if ( temp->get_value() != NULL ) {

	//del ( temp->get_name() );
	assign ( temp->get_name(), temp->get_value()->copy() );
	//temp->clear();

      } else {

	ASSERT ( false ); // this shouldn't happen

      }

    }

  } 

}

void SymbolTable::print ( FILE * fp ) {

  int i;
  Symbol * temp;

  fprintf ( fp, "    %d symbol(s) and %d bucket(s):\n", num_symbols, num_buckets );

  for ( i=0; i<num_buckets; i++ ) {
    //    if ( buckets[i] != NULL )
    //  fprintf ( fp, "  bucket %d\n", i );    
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
      fprintf ( fp, "      %x: %s: ", temp, temp->get_name() );
      if ( temp->get_value() != NULL ) {
        temp->get_value()->print(fp);
	fprintf( fp, "(" );
	temp->get_previous()->print(fp);
	fprintf ( fp, ")" );
      }
      else
        temp->get_prog_value()->small_print();
      fprintf ( fp, "\n" );
    }
  }

}

void SymbolTable::print_names ( FILE * fp, int indent ) {

  int i;
  Symbol * temp;

  for ( i=0; i<num_buckets; i++ )
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
      for ( int k = 0; k<indent; k++ ) 
	fprintf ( fp, " " );
      fprintf ( fp, "%s:", temp->get_name() );
      if ( temp->get_value() != NULL )
        temp->get_value()->print(fp);
      else
        fprintf ( fp, "<<prog>>" );
      fprintf ( fp, "\n" );
    }

}

void SymbolTable::print_as_record ( FILE * fp ) {

  int i, j=0;
  Symbol * temp;

  fprintf ( fp, "[" );

  for ( i=0; i<num_buckets; i++ ) {
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
      fprintf ( fp, " %s := ", temp->get_name() );
      temp->get_value()->print(fp);
      if ( ++j != num_symbols ) 
        fprintf ( fp, "," );
    }
  }

  fprintf ( fp, " ]" );


}

void SymbolTable::print_as_record_to_string ( std::string * str )  {

  int i, j=0;
  Symbol * temp;
  char buf[100];

  *str += "[";

  for ( i=0; i<num_buckets; i++ ) {
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
      sprintf ( buf, "%s:=", temp->get_name() );
      *str += buf;
      temp->get_value()->print_to_string ( str );
      if ( ++j != num_symbols ) 
	*str += ",";
    }
  }

  *str += "]";

}

std::list<std::string> SymbolTable::field_names_as_string_list (void)
{

  int i;
  Symbol * temp;

  std::list<std::string> fields;

  for ( i=0; i<num_buckets; i++ ) {
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
      fields.push_back(std::string(temp->get_name()));
    }
  }

  return(fields);

}

void SymbolTable::record_tostring_aux ( std::stringstream * strm )  {

  int i, j=0;
  Symbol * temp;

  *strm << "[";

  for ( i=0; i<num_buckets; i++ ) {
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
        *strm << temp->get_name() << ":=";
        temp->get_value()->tostring_aux ( strm );
        if ( ++j != num_symbols )
            *strm << ",";
    }
  }

  *strm << "]";

}

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
}

SymbolTable * SymbolTable::copy ( void ) {

  int i;
  Symbol * temp;
  SymbolTable * s = new SymbolTable ( num_buckets );

  for ( i=0; i<num_buckets; i++ ) {
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {
      if ( temp->is_program() ) 
        s->add ( temp->get_name(), temp->get_prog_value()->copy() );
      else
        s->add ( temp->get_name(), temp->get_value()->copy() );
    }
  }

  return s;

}

void SymbolTable::divide ( SymbolTable * dest, float frac ) {

  int i;
  Symbol * temp;

  for ( i=0; i<num_buckets; i++ ) {
    for ( temp=buckets[i]; temp!=NULL; temp=temp->next ) {

      if ( !temp->is_program() ) {

	Value * v = temp->get_value();

	if ( v->get_type() == Value::INTEGER ) {

	  int n = v->int_value();
	  v->set ( (int) floor (frac*n) );
          dest->assign ( temp->get_name(), new Value ( (int) ceil ( (1.0-frac)*n ) ) );

	} else if ( v->get_type() == Value::REAL ) {

	  double x = v->real_value();
	  v->set ( frac * x );
          dest->assign ( temp->get_name(), new Value ( ( 1.0 - frac ) * x ) );

	}

      }

    }
  }


}

void SymbolTable::add ( const char * name, Program * p ) {

  int index = hash ( name ); 
  Symbol * temp = new Symbol ( name, p );

  temp->next = buckets[index];
  buckets[index] = temp;
  num_symbols++;

}

Program * SymbolTable::get_program ( const char * name ) {

  int index = hash ( name ); 
  Symbol * temp;

  for ( temp = buckets[index]; temp != NULL; temp = temp->next )
    if ( !strcmp ( temp->get_name(), name ) ) return temp->get_prog_value();

  return NULL;

}

bool member_charlist ( std::list<char *> * L, const char * str ) {

  std::list<char *>::iterator i;                                
 
  for ( i = L->begin(); i != L->end(); i++ ) 
    if ( !strcmp ( *i, str ) )
      return true;

  return false;

}
