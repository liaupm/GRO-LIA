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
#include <string>
#include "SymbolTable.h"
#include "Expr.h"
#include "Program.h"

#define LISTVAL data.list_val.vals

static int instance_counter = 0;

int get_count ( void ) {

  return instance_counter;

}

Value::Value ( int n ) : type(INTEGER) { 

  data.int_val = n;
  instance_counter++; 

}

Value::Value ( double r ) : type(REAL) { 

  data.real_val = r;
  instance_counter++; 

}

Value::Value ( bool b ) : type(BOOLEAN) {

  data.bool_val = b;
  instance_counter++; 

}

Value::Value ( const char * str ) : type(STRING) {

  data.str_val = new std::string;
  (*data.str_val) = str;
  instance_counter++;

}

Value::Value ( Value::TYPE t ) : type(t) {

  if ( type == LIST ) 
    LISTVAL = new std::list<Value*>;

  if ( type == RECORD )
    data.fields = new SymbolTable ( 10 );

  instance_counter++;

}

Value::Value (  std::list<Value *> * L ) : type(LIST) {

  type = LIST;
  LISTVAL = L;
  instance_counter++;

}

Value::Value ( char * a, Expr * e ) : type(FUNCTION) {

  data.func_val.arg_name = strdup ( a );
  data.func_val.function_body = e;

  instance_counter++;

}

Value::Value ( EXTERNAL_CCLI_FUNCTION f, TypeExpr * T, std::list<TypeExpr *> * args ) : type(EX_FUNCTION) {

  data.efunc_val.return_type = T;
  data.efunc_val.arg_types = args;
  data.efunc_val.f = f;

  // compute non_generics
  data.efunc_val.non_generics = new std::list<TypeExpr *>;

  T->compute_non_generics ( data.efunc_val.non_generics );

  std::list<TypeExpr *>::iterator i;

  for ( i = args->begin(); i != args->end(); i++ )
    (*i)->compute_non_generics ( data.efunc_val.non_generics );

#if 0
    printf ( "made new external function\n" );
    printf ( "  return type (%x) = ", return_type() ); return_type()->print(); printf ( "\n" );

    printf ( "  arg types:\n" );

    for ( i = arg_types()->begin(); i != arg_types()->end(); i++ ) {
      printf ( "    %x = ", *i ); (*i)->print(); printf ( "\n" );
    }
  
    printf ( "  non generics:\n" );

    for ( i = data.efunc_val.non_generics->begin(); i != data.efunc_val.non_generics->end(); i++ ) {
      printf ( "    %x = ", *i ); (*i)->print(); printf ( "\n" );
    }
#endif

  instance_counter++;

}

Value::Value ( void ) : type ( LIST ) {

  LISTVAL = new std::list<Value *>;
  instance_counter++;

}

Value::Value ( Program * p ) : type ( PROGRAM ) {

  data.program_val = p;
  instance_counter++;

}

void Value::free_data ( void ) {

  switch ( type ) {

  case LIST:
    {
      std::list<Value *>::iterator i;
      for ( i = LISTVAL->begin();  i != LISTVAL->end(); i++ )
        delete *i;
      delete LISTVAL;
    }
    break;

  case RECORD:
    delete data.fields;
    break;

  case FUNCTION:
    delete data.func_val.function_body;
    free ( data.func_val.arg_name );
    break;

  case STRING:
    delete data.str_val;
    break;

  case EX_FUNCTION:

    {

      if ( data.efunc_val.return_type != NULL ) {

	if ( ! TypeExpr::member ( return_type(), non_generics() ) ) {
	  //printf ( "deleting %x\n", return_type() );
	  return_type()->free_children(non_generics());
	  delete return_type();
        }
	
	while ( arg_types()->size() > 0 ) {
	  
	  TypeExpr * T = *(arg_types()->begin());
	  arg_types()->pop_front();
	  if ( ! TypeExpr::member ( T, non_generics() ) ) {
	    //printf ( "deleting %x\n", T );
	    T->free_children(non_generics());
	    delete T;
	  }
	  
	}
	
	std::list<TypeExpr *>::iterator i;
	
	for ( i = non_generics()->begin(); i != non_generics()->end(); i++ ) {
	  //printf ( "deleting %x\n", *i );
	  delete *i;
	}
	
	delete arg_types();
	delete non_generics();
	
      }

    }

    break;

  case PROGRAM:
    //    delete data.program_val;
    break;

  default:

    break;

  }

}

Value::~Value ( void ) {

  free_data();

  instance_counter--;

#ifdef MEMCHECK
  if ( instance_counter == 0 )
    printf ( "  no values left\n" );
#endif

  if ( instance_counter < 0 )
    printf ( "WARNING: instance count in ~Value is negative!\n" );

}

void Value::reset ( Value * v ) {

  free_data();

  type = v->type;

  switch ( v->type ) {

  case UNIT:

    break;

  case BOOLEAN:

    data.bool_val = v->data.bool_val;
    break;

  case INTEGER:

    data.int_val = v->data.int_val;
    break;

  case REAL:

    data.real_val = v->data.real_val;
    break;

  case STRING:
    
    data.str_val = v->data.str_val;
    break;

  case LIST:

    data.list_val.vals = v->data.list_val.vals;
    break;

  case RECORD:

    data.fields = v->data.fields;
    break;

  case FUNCTION:

    data.func_val.function_body = v->data.func_val.function_body;
    data.func_val.arg_name = v->data.func_val.arg_name;
    break;

  case EX_FUNCTION:

    data.efunc_val.f = v->data.efunc_val.f;
    data.efunc_val.return_type = v->data.efunc_val.return_type;
    data.efunc_val.arg_types = v->data.efunc_val.arg_types;
    data.efunc_val.non_generics = v->data.efunc_val.non_generics;
    break;

  default:

    ASSERT ( false );

  }

  v->type = UNIT; // this is so when you deconstruct it
                  // it doesn't delete anything

}

void Value::set ( bool b ) {

  if ( type == BOOLEAN )
    data.bool_val = b;
  else {
    fprintf ( stderr, "Value::set: attempted to set BOOLEAN value of non-BOOLEAN value\n" );
    exit ( -1 );
  }

}

void Value::set ( int n ) {

  if ( type == INTEGER )
    data.int_val = n;
  else {
    fprintf ( stderr, "Value::set: attempted to set INTEGER value of non-INTEGER value\n" );
    exit ( -1 );
  }

}

void Value::set ( double r ) {

  if ( type == REAL )
    data.real_val = r;
  else {
    fprintf ( stderr, "Value::set: attempted to set REAL value of non-REAL value\n" );
    exit ( -1 );
  }

}

void Value::set ( const char * str ) {

  if ( type == STRING )
    (*data.str_val) = str;
  else {
    fprintf ( stderr, "Value::set: attempted to set STRING value of non-STRING value\n" );
    print ( stderr ); fprintf ( stderr, "\n" ); fflush ( stderr );
    exit ( -1 );
  }

}

bool Value::bool_value ( void ) {

  if ( type != BOOLEAN ) {
    fprintf ( stderr, "Value::bool_value: attempted to access BOOLEAN value of non-BOOLEAN value\n" );
    exit ( -1 );
  }

  return data.bool_val;

}

Program * Value::program_value ( void ) {

  if ( type != PROGRAM ) {
    fprintf ( stderr, "Value::bool_value: attempted to access PROGRAM value of non-PROGRAM value\n" );
    exit ( -1 );
  }

  return data.program_val;

}

int Value::int_value ( void ) {

  if ( type != INTEGER ) {
    fprintf ( stderr, "Value::int_value: attempted to access INTEGER value of non-INTEGER value\n" );
    exit ( -1 );
  }

  return data.int_val;

}

double Value::real_value ( void ) {

  if ( type != REAL && type != INTEGER ) {
    fprintf ( stderr, "Value::real_value: attempted to access REAL value of non-REAL value\n" );
    exit ( -1 );
  }

  return type == REAL ? data.real_val : data.int_val;

}

std::string Value::string_value ( void ) {

  if ( type != STRING ) {
    fprintf ( stderr, "Value::string_value: attempted to access STRING value of non-STRING value\n" );
    print ( stderr ); fprintf ( stderr, "\n" ); fflush ( stderr );
    exit ( -1 );
  }

  return *data.str_val;

}


double Value::num_value ( void ) {

  if ( type == REAL ) 
    return data.real_val;
  if ( type == INTEGER )
    return (double) data.int_val;
  else {
    fprintf ( stderr, "Value::num_value: attempted to access numerical value of non-numerical value\n" );
    print(stderr); fprintf ( stderr, "\n" );
    exit ( -1 );
  }

}

Expr * Value::func_body ( void ) {

  if ( type != FUNCTION ) {
    fprintf ( stderr, "Value::func_body: attempted to get function body from non-function\n" );
    exit ( -1 );
  }

  return data.func_val.function_body;

}

char * Value::func_arg_name ( void ) {

  if ( type != FUNCTION ) {
    fprintf ( stderr, "Value::func_body: attempted to get argument names from non-function\n" );
    exit ( -1 );
  }

  return data.func_val.arg_name;

}

void Value::addField ( const char * name, Value * v ) {

  if ( type != RECORD ) {
    fprintf ( stderr, "Value::addField: attempted to add a field to a non-RECORD value\n" );
    exit ( -1 );
  }

  data.fields->add ( name, v );  

}

Value * Value::getField ( const char * name ) {

  if ( type != RECORD ) {
    fprintf ( stderr, "Value::getField: attempted to get a field from a non-RECORD value\n    " );
    print ( stderr ); fprintf ( stderr, "\n" );
    exit ( -1 );
  }

  return data.fields->get ( name );

}

void Value::set_fields ( SymbolTable * f ) {

  if ( type != RECORD ) {
    fprintf ( stderr, "Value::getField: attempted to set fields of a non-RECORD value\n" );
    exit ( -1 );
  }

  delete data.fields;
  data.fields = f;

}

void Value::print ( FILE * fp ) {

  switch ( type ) {

  case UNIT:
    fprintf ( fp, "." );
    break;
  case BOOLEAN:
    fprintf ( fp, "%s", data.bool_val ? "true" : "false" );
    break;
  case INTEGER:
    fprintf ( fp, "%d", data.int_val );
    break;
  case REAL:
    fprintf ( fp, "%f", data.real_val );
    break;
  case STRING:
    fprintf ( fp, "\"%s\"", data.str_val->c_str() );
    break;
  case LIST:

    {
      std::list<Value*>::iterator i, j;
      fprintf ( fp, "{ " );
      for ( i=LISTVAL->begin(); i != LISTVAL->end(); i++ ) {
        (*i)->print(fp);
	j = i;
	j++;
	if ( j++ != LISTVAL->end() )
          fprintf ( fp, ", " );
      }
      fprintf ( fp, " }" );
    }
    break;

  case RECORD:
    data.fields->print_as_record(fp);
    break;

  case FUNCTION:

    {

      fprintf ( fp, "( lambda %s . ",  data.func_val.arg_name );
      data.func_val.function_body->print(fp);
      fprintf ( fp, " )" );

    }
    break;

  case EX_FUNCTION:

    {

      std::list<TypeExpr *>::iterator i, j;

      return_type()->clear_var_names();

      for ( i=arg_types()->begin(); i != arg_types()->end(); i++ )
	(*i)->clear_var_names();

      return_type()->print_aux ( fp );

      fprintf ( fp, " <<external>> ( " );

      for ( i=arg_types()->begin(); i != arg_types()->end(); i++ ) {
	j = i;
	j++;
	(*i)->print_aux(fp);
	if ( j != arg_types()->end() )
	  fprintf ( fp, ", " );
      }

      fprintf ( fp, " )" );


    }

    break;

  case PROGRAM:
    data.program_val->print();
    break;

  default:
    fprintf ( fp, "!!unimplemented print method (type = %d)!!!\n", type );

  }

}

Value * Value::copy ( void ) {

  Value * v = NULL;

  switch ( type ) {

  case UNIT:

    v = new Value ( UNIT );
    break;

  case BOOLEAN:

    v = new Value ( BOOLEAN );
    v->set ( data.bool_val );
    break;

  case INTEGER:

    v = new Value ( INTEGER );
    v->set ( data.int_val );
    break;

  case REAL:

    v = new Value ( REAL );
    v->set ( data.real_val );
    break;

  case STRING:

    v = new Value ( data.str_val->c_str() );
    break;

  case LIST:

    {
      std::list<Value *> * L = new std::list<Value *>;
      std::list<Value *>::iterator i;

      for ( i=LISTVAL->begin(); i!=LISTVAL->end(); i++ )
	L->push_back ( (*i)->copy() );

      v = new Value ( L );

    }
    break;

  case RECORD:

    v = new Value ( RECORD );
    v->set_fields ( data.fields->copy() );
    break;

  case FUNCTION:

    v = new Value ( data.func_val.arg_name, data.func_val.function_body->copy() );
    break;

  case EX_FUNCTION:

    {

      // really shouldn't need to copy all of this during evaluation

      std::list<TypeExpr *> 
        * non_generics = new std::list<TypeExpr *>,
	* new_args = new std::list<TypeExpr *>,
        * new_vars = new std::list<TypeExpr *>,
	* old_vars = data.efunc_val.non_generics;

      std::list<TypeExpr *>::iterator i;

      for ( i = old_vars->begin(); i != old_vars->end(); i++ ) 
	new_vars->push_back ( new TypeExpr() );

      TypeExpr * R = return_type()->copy_ex ( old_vars, new_vars );

      for ( i = arg_types()->begin(); i != arg_types()->end(); i++ ) 
	new_args->push_back ( (*i)->copy_ex ( old_vars, new_vars ) );

      v = new Value ( data.efunc_val.f, R, new_args );

      delete new_vars;
      delete non_generics;

    }

    break;

  default:

    fprintf ( stderr, "unimplemented copy method (%d): ", type ); print(stderr); 
    fprintf ( stderr, "\n" );
    exit ( -1 );

  }

  return v;

}

Symbol::Symbol ( const char * str, Value * val ) : v(val), previous(val), p(NULL) {

  name = strdup ( str );
  rec_flag = false;

}

Symbol::Symbol ( const char * str, Program * prog ) : v(NULL), p(prog) {

  name = strdup ( str );
  rec_flag = false;

}

void Symbol::clear ( void ) {

  if ( previous != v )
    delete previous;

  v = NULL;

}

Symbol::~Symbol ( void ) {

  free ( name );
 
  if ( v != NULL ) {

    if ( v != previous ) {

      delete v;
      delete previous;

    } else 

      delete v;

  } else if ( p != NULL )

      delete p;

}

void Value::print_to_string ( std::string * str ) {

  char buf[20];

  switch ( type ) {

  case UNIT:
    sprintf ( buf, "." );
    *str += buf;
    break;
  case BOOLEAN:
    sprintf ( buf, "%s", data.bool_val ? "true" : "false" );
    *str += buf;
    break;
  case INTEGER:
    sprintf ( buf, "%d", data.int_val );
    *str += buf;
    break;
  case REAL:
    sprintf ( buf, "%f", data.real_val );
    *str += buf;
    break;
  case STRING:
    sprintf ( buf, "\"%s\"", data.str_val->c_str() );
    *str += buf;
    break;
  case LIST:

    {
      std::list<Value*>::iterator i, j;
      *str += "{";
      for ( i=LISTVAL->begin(); i != LISTVAL->end(); i++ ) {
	(*i)->print_to_string(str);
	j = i;
	j++;
	if ( j++ != LISTVAL->end() )
	  *str += ",";
      }
      *str += "}";
    }
    break;

  case RECORD:
    data.fields->print_as_record_to_string ( str );
    break;

  case FUNCTION:

    *str += "<func>";
    break;

  case EX_FUNCTION:

    *str += "<ex>";
    break;

  default:
    fprintf ( stderr, "!!unimplemented print to string method (type = %d)!!!\n", type );

  }

}

std::string Value::tostring ( void ) {

    std::stringstream strm;
    tostring_aux ( & strm );
    return strm.str();

}

void Value::tostring_aux ( std::stringstream * strm ) {

    switch ( type ) {

    case UNIT:
      *strm << ".";
      break;
    case BOOLEAN:
      *strm << ( data.bool_val ? "true" : "false" );
      break;
    case INTEGER:
      *strm << data.int_val;
      break;
    case REAL:
      *strm << data.real_val;
      break;
    case STRING:
      *strm << "\"" << data.str_val->c_str() << "\"";
      break;

    case LIST:

      {
          std::list<Value*>::iterator i, j;
          *strm << "{";
            for ( i=LISTVAL->begin(); i != LISTVAL->end(); i++ ) {
              (*i)->tostring_aux(strm);
              j = i;
              j++;
              if ( j++ != LISTVAL->end() )
              *strm << ",";
          }
          *strm << "}";
      }
      break;

    case RECORD:
      data.fields->record_tostring_aux ( strm );
      break;

    case FUNCTION:
      *strm << "&lt;func&gt;";
      break;

    case EX_FUNCTION:
      *strm << "&lt;ex&gt;";
      break;

    default:
      throw ( std::string ( "!!unimplemented tostring_aux method!!" ) );

    }

}


