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
#include "Expr.h"

#define NL printf ( "\n" ); fflush ( stdout );
#define FNL fprintf ( stderr, "\n" ); fflush ( stderr );

//#define VERBOSE 1

#if VERBOSE
static int indent = 0;
#define PIND { int i; for(i=0;i<indent;i++) printf(" "); }
#endif

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
    fflush ( stdout );                                    \
}

#define BOTH_INTS(__v1__,__v2__) ( __v1__->get_type() == Value::INTEGER && __v2__->get_type() == Value::INTEGER )

Value * Expr::eval ( Scope * s ) {

#if VERBOSE
  PIND; printf ( "evaluating " ); print(); NL;
  indent += 2;
#endif

  Value * answer = NULL;

  switch ( type ) {

  case CONSTANT:
    answer = val->copy();
    break;

  case VARIABLE:

    {
      Value * temp = s->get ( name );
      if ( temp == NULL ) {
          RUNTIME_ERROR(file,line,"Could not find definition of symbol <i>" << name << ".</i>" );
      }
      answer = temp->copy();
    }
    break;

  case APPLICATION:

    {

      //      printf ( "  trying to evaluate " ); e1->print(); printf ( " applied to " ); e2->print(); NL;

      Value * f = e1->eval ( s );

      Expr * e2val = new Expr ( e2->eval ( s ) );
      Expr * appexp = f->func_body()->subst ( e2val, f->func_arg_name() );
      answer = appexp->eval ( s );

      delete f;
      delete appexp;
      delete e2val;
    }
    break;

  case EX_APPLICATION:

    {

      std::list<Value *> vals;
      std::list<Value *>::iterator j;
      std::list<Expr *>::iterator i;

      for ( i = arglist->begin(); i != arglist->end(); i++ )
          vals.push_back ( (*i)->eval ( s ) );

      answer = val->efunc() ( &vals, s );

      for ( j = vals.begin(); j != vals.end(); j++ )
          delete (*j);

    }
    break;

  case UNARY:
  case BINARY:

    answer = eval_op(s);
    break;

  case LET:

    {
 
      SymbolTable * sym = new SymbolTable ( 1 );
      Value * letval = e1->eval ( s );
      sym->add ( name, letval );
      s->push ( sym );
      answer = e2->eval ( s );
      s->pop();
      delete sym;

    }

    break;

  case IF:

    {
      Value * cond = e1->eval(s);
      answer = cond->bool_value() ? e2->eval(s) : e3->eval(s);
      delete cond;
    }

    break;

  case RECORD:

    {

      std::list<FIELD *>::iterator i;

      answer = new Value ( Value::RECORD );

      for ( i = fields->begin(); i != fields->end(); i++ )
	answer->addField ( (*i)->name, (*i)->e->eval(s) );


    }
    break;

  case PROGRAM:
    answer = new Value ( program );
    break;

  default:
    fprintf ( stderr, "unimplemented expr->eval method on type %d!\n", type );
    exit ( -1 );
    break;

  }  

#if VERBOSE
  indent -= 2;
  PIND; printf ( "got " );
  answer->print(); NL;
#endif

  return answer;

}

Value * Expr::eval_op ( Scope * s ) {

  Value * answer = NULL;

  switch ( op ) {

    // booleans ----------------------------------------------------------------

    case AND:

      { 
        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );
        answer = new Value ( v1->bool_value() && v2->bool_value() );
        delete v1;
        delete v2;
      }
      break;

    case OR:

      {
        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );
        answer = new Value (  v1->bool_value() || v2->bool_value() );
        delete v1;
        delete v2;
      }
      break;

    case NOT:

      {
        Value * v1 = e1->eval ( s );
        answer = new Value (  !v1->bool_value() );
        delete v1;
      }
      break;

    // integers ----------------------------------------------------------------

    case MOD:

      {
        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );
        answer = new Value ( v1->int_value() % v2->int_value() );
        delete v1;
        delete v2;
      }
      break;

    // integers and reals ------------------------------------------------------

    case POW:

      {
        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

        if ( BOTH_INTS(v1,v2) )

           answer = new Value ( (int) pow ( v1->num_value(), v2->num_value() ) );

	else

	  answer = new Value ( pow ( v1->num_value(), v2->num_value() ) );

        delete v1;
        delete v2;
      }
      break;

    case PLUS:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value ( v1->int_value() + v2->int_value() );

        else
 
	  answer = new Value ( v1->num_value() + v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    case MINUS:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value (  v1->int_value() - v2->int_value() );

        else
 
	  answer = new Value (  v1->num_value() - v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    case MULT:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value ( v1->int_value() * v2->int_value() );

        else
 
	  answer = new Value ( v1->num_value() * v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    case DIV:

  {

      Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

      if ( BOTH_INTS(v1,v2) ) {

          if ( v2->int_value() == 0 ) {
              RUNTIME_ERROR(e2->file_name(),e2->line_num(),"Divide by zero!" );
          } else
              answer = new Value ( v1->int_value() / v2->int_value() );

      } else {

          if ( v2->num_value() == 0 ) {
              RUNTIME_ERROR(e2->file_name(),e2->line_num(),"Divide by zero!" );
          } else
              answer = new Value ( v1->num_value() / v2->num_value() );

      }

      delete v1;
      delete v2;

  }

      break;

    case UMIN:

      {

        Value * v1 = e1->eval ( s );

	if ( v1->get_type() == Value::INTEGER )

	  answer = new Value ( -v1->int_value() );

	else

	  answer = new Value ( -v1->real_value() );

        delete v1;

      }

      break;

    case LT:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value (  v1->int_value() < v2->int_value() );

        else
 
	  answer = new Value (  v1->num_value() < v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    case GT:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value (  v1->int_value() > v2->int_value() );

        else
 
	  answer = new Value (  v1->num_value() > v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    case LE:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value (  v1->int_value() <= v2->int_value() );

        else
 
	  answer = new Value (  v1->num_value() <= v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    case GE:

      {

        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );

	if ( BOTH_INTS(v1,v2) )

	  answer = new Value (  v1->int_value() >= v2->int_value() );

        else
 
	  answer = new Value (  v1->num_value() >= v2->num_value() );

        delete v1;
        delete v2;

      }

      break;

    // strings -----------------------------------------------------------------

    case STR_CAT:
      {
        Value * v1 = e1->eval ( s ), * v2 = e2->eval ( s );
        answer = new Value ( (v1->string_value()+v2->string_value()).c_str() );
        delete v1;
        delete v2;
      }
      break;

    // lists -------------------------------------------------------------------

    case HEAD:

      answer = list_head ( s );
      break;

    case TAIL:

      answer = list_tail ( s );
      break;

    case CONS:

      answer = list_cons ( s );
      break;

    case CONCAT:

      answer = list_join ( s );
      break;

    case LIST_EL:

      {

	Value * L;

        if ( e1->get_type() == Expr::VARIABLE )  // this might speed things up a tad
	  L = s->get ( e1->get_name() );
        else 
	  L = e1->eval(s);

	Value * i = e2->eval(s);

	int q = 0, j = i->int_value();
	std::list<Value *> * K = L->list_value();
	std::list<Value *>::iterator m = K->begin();

	if ( j < 0 || j >= K->size() ) {

	  fprintf ( stderr, "Runtime Error, line %d of '%s': array out of bounds error (index = %d)\n", e1->line_num(), 
		    e1->file_name() == NULL ? "no file info" : e1->file_name(), j );

      RUNTIME_ERROR(file,line,"Array out of bounds error (index = " << j << ")." );

	}

	while ( q < j ) { q++; m++; }

	answer = (*m)->copy();

	if ( e1->get_type() != Expr::VARIABLE )
	  delete L;

	delete i;
	
      }

      break;

    case EQ:

      {

	Value * v1 = e1->eval(s),
	  * v2 = e2->eval(s);

	switch ( v1->get_type() ) {

          case Value::UNIT:

	    answer = new Value ( true );
	    break;

          case Value::BOOLEAN:

	    answer = new Value ( v1->bool_value() == v2->bool_value() );
	    break;

          case Value::INTEGER:

	    if ( v2->get_type() == Value::INTEGER ) 
	      answer = new Value ( v1->int_value() == v2->int_value() );
	    else
	      answer = new Value ( v1->num_value() == v2->num_value() );

	    break;

          case Value::REAL: 

	    answer = new Value ( v1->num_value() == v2->num_value() );
	    break;

          case Value::STRING:

	    answer = new Value (  v1->string_value() == v2->string_value() );
	    break;

          case Value::LIST:

	    answer = list_equal ( v1, v2, s );
	    break;

          case Value::RECORD:
          case Value::FUNCTION:
   	  case Value::EX_FUNCTION:

	    fprintf ( stderr, "Error at line %d of '%s'\n", e1->line_num(), 
		      e1->file_name() == NULL ? "no file info" : e1->file_name() );
	    ASSERT_MSG ( false,
			 "equality not implemented for RECORD, FUNCTION and EX_FUNCTION types\n" );
	    break;

	}

	delete v1;
	delete v2;

      }

      break;

    case NE:

      {

	Value * v1 = e1->eval(s),
	  * v2 = e2->eval(s);

	switch ( v1->get_type() ) {

          case Value::UNIT:

	    answer = new Value ( true );
	    break;

          case Value::BOOLEAN:

	    answer = new Value ( v1->bool_value() != v2->bool_value() );
	    break;

          case Value::INTEGER:

	    if ( v2->get_type() == Value::INTEGER ) 
	      answer = new Value ( v1->int_value() != v2->int_value() );
	    else
	      answer = new Value ( v1->num_value() != v2->num_value() );

	    break;

          case Value::REAL: 

	    answer = new Value ( v1->num_value() != v2->num_value() );
	    break;

          case Value::STRING:

	    answer = new Value (  v1->string_value() != v2->string_value() );
	    break;

          case Value::LIST:

	    {

	      Value * temp = list_equal ( v1, v2, s );
	      answer = new Value ( ! temp->bool_value() );
	      delete temp;

	    }

	    break;

          case Value::RECORD:
          case Value::FUNCTION:
   	  case Value::EX_FUNCTION:

	    ASSERT_MSG ( false,
			 "equality not implemented for RECORD, FUNCTION and EX_FUNCTION types\n" );
	    break;

	}

	delete v1;
	delete v2;

      }

      break;

    // records -----------------------------------------------------------------

    case DOT:

      {

        Value * temp = e1->eval ( s ), * f = NULL;
        f = temp->getField ( e2->get_name() );

	if ( f == NULL ) { 
          temp->print ( stderr ); fprintf ( stderr, "." );
          e2->print ( stderr ); fprintf ( stderr, "\n" );
        }
	ASSERT_MSG ( f != NULL, "could not get field '%s' from record\n", e2->get_name() );

        answer = f->copy();
        delete temp;

      }

      break;

    case RECORD_ADD:

      {

	Value * left = e1->eval ( s ),
              * right = e2->eval ( s );

	left->get_fields()->replace ( right->get_fields() );
	answer = left;
	delete right;

      }

      break;

    case PREV:

      {

	ASSERT ( e1->get_type() == VARIABLE );

        #ifdef VERBOSE
	PIND; printf ( "prev: %s\n", e1->get_name() );
        #endif

	Value * temp = s->get_previous ( e1->get_name() );
	if ( temp == NULL ) {
	  fprintf ( stderr, "Expr::eval: could not find symbol '%s' in\n", name );
	  exit ( -1 );
	}
	answer = temp->copy();
      }

      break;

  }

  return answer;

}
