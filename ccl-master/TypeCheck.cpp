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

#include <sstream>
//#include "/usr/include/c++/4.2.1/sstream"
#include <string>
#include "Expr.h"

#define NL printf ( "\n" ); fflush ( stdout );
#define INDENT(__n__) { int i; for (i=0; i<__n__; i++) printf (" "); }

#define TYPE_ERROR(__msg__...) {                                        \
                                                                        \
    std::stringstream strm;                                             \
                                                                        \
     strm << "Type error in "                                           \
          << ( file_name() != NULL ? file_name() : "no file information" ) \
          << " on line " << line_num()                                  \
          << "<br><blockquote>" << __msg__ << "</blockquote>";             \
                                                                        \
     throw strm.str();                                                  \
                                                                        \
}

#define TYPE_ERROR1(__e__,__t__,__msg__...) {                                         \
                                                                                      \
    std::stringstream strm;                                                           \
                                                                                      \
    strm << "Type error in "                                                          \
         << ( file_name() != NULL ? file_name() : "no file information" )             \
         << " on line " << line_num()                                                 \
         << "<br><blockquote>" << __msg__ << " Here, "                                \
         << "<i>" << __e__->tostring() << "</i> has type <i>" << __t__->tostring()    \
         << ".</i></blockquote>";                                                     \
                                                                                      \
         throw strm.str();                                                            \
                                                                                      \
}

#define TYPE_ERROR2(__e1__,__t1__,__e2__,__t2__,__msg__...) {                         \
                                                                                      \
    std::stringstream strm;                                                           \
                                                                                      \
    strm << "Type error in "                                                          \
         << ( file_name() != NULL ? file_name() : "no file information" )             \
         << " on line " << line_num()                                                 \
         << "<br><blockquote>" << __msg__ << " In particular, "                       \
         << "<i>" << __e1__->tostring() << "</i> has type <i>" << __t1__->tostring() << "</i>, while " \
         << "<i>" << __e2__->tostring() << "</i> has type <i>" << __t2__->tostring() << ".</i>"        \
         << "</blockquote>";                                                          \
                                                                                      \
         throw strm.str();                                                            \
                                                                                      \
}

//#define VERBOSE

static TypeExpr * add_garbage ( TypeExpr * T, std::list<TypeExpr *> * g ) {

  g->push_back ( T );
  return T;

}

#define VAR_TYPE add_garbage ( new TypeExpr(), garbage )
#define ATOMIC_TYPE(_T_) add_garbage ( new TypeExpr ( _T_ ), garbage )
#define LIST_TYPE(_T_) add_garbage ( new TypeExpr ( _T_ ), garbage )
#define FUNCTION_TYPE(_D_,_C_) add_garbage ( new TypeExpr ( _D_, _C_ ), garbage )
#define RECORD_TYPE(_E_) add_garbage ( new TypeExpr ( _E_ ), garbage )

Value::TYPE Expr::op_type ( OPERATOR op ) {

  if ( op == AND    || op == OR     || op == NOT     || op == EQ || op == NE ||
       op == LT     || op == GT     || op == LE      || op == GE )

    return Value::BOOLEAN;

  else return op_arg_type ( op );

}

Value::TYPE Expr::op_arg_type ( OPERATOR op ) {

  if ( BOOL_OP(op) ) return Value::BOOLEAN;
  if ( COMP_OP(op) ) return Value::INTEGER;
  else if ( INT_OP(op) ) return Value::INTEGER;
  else if ( NUM_OP(op) ) return Value::INTEGER;
  else if ( STRING_OP(op) ) return Value::STRING;
  else return Value::RECORD;

}

TypeExpr * Value::compute_type ( Scope * s, Environment * env, 
				 std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage ) {

  TypeExpr * answer = NULL;

  switch ( type ) {

    case UNIT:
    case BOOLEAN:
    case INTEGER:
    case REAL:
    case STRING:

      answer = ATOMIC_TYPE ( type );
      break;

    case LIST:

      if ( list_value()->size() == 0 )
	answer = LIST_TYPE ( VAR_TYPE );
      else 
	answer = LIST_TYPE ( ( *list_value()->begin() )->compute_type ( s, env, non_generics, garbage ) );
      break;

    case RECORD:

      answer = RECORD_TYPE ( get_fields()->to_env ( s, env, non_generics, garbage ) );
      answer->set_extensibility ( false );
      break;

    case FUNCTION:

      {

	TypeExpr * binder = VAR_TYPE;
	Environment * body_env = new Environment ( func_arg_name(), binder, env );
	non_generics->push_front ( binder );
	TypeExpr * body = func_body()->compute_type_aux ( s, body_env, non_generics, garbage );

        answer = FUNCTION_TYPE ( binder, body );
	non_generics->pop_front();
	delete body_env;

      }

      break;

    case EX_FUNCTION:

      ASSERT_MSG ( false, "error: attempted to compute type of external function\n" );
      break;

  }

  return answer;

}

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
    fflush ( stdout );                                    \
}

bool Expr::type_check ( Scope * s, Environment * env ) {

  RESET(env);

  std::list<TypeExpr *> ng, garbage;

  TypeExpr * T = compute_type ( s, env, &ng, &garbage );
  //printf ( "type is " ); T->print(); printf ( "\n" );

  bool ans = T->is_consistent();

  TypeExpr::take_out ( & garbage );

  return ans;

}

static std::list<char *> * rec_vars;

TypeExpr * Expr::compute_type ( Scope * s, Environment * env, 
				std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage ) {

  //printf ( "checking " ); print(); printf ( "\n" );
  rec_vars = new std::list<char *>;
  TypeExpr * answer = compute_type_aux ( s, env, non_generics, garbage );
  delete rec_vars;
  return answer;

}

static bool in_list ( const char * name, std::list<char *> * L ) {

  std::list<char *>::iterator i;

  for ( i = L->begin(); i != L->end(); i++ ) {
    if ( ! strcmp ( *i, name ) )
      return true;
  }

  return false;

}

static int ctcount = 0;

TypeExpr * Expr::compute_type_aux ( Scope * s, Environment * env, 
				    std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage ) {

#ifdef VERBOSE
  INDENT(ctcount);
  printf ( "%d. checking ", ctcount++ ); print(); NL;
#endif

  TypeExpr * answer;

  switch ( type ) {

    case CONSTANT:

      answer = val->compute_type ( s, env, non_generics, garbage );
      break;

    case VARIABLE:

      { 

#ifdef VERBOSE
	INDENT(ctcount-1); printf ( "%d. looking for %s in {", ctcount-1, name );
	env->print(); printf ( "}\n" );
#endif

	TypeExpr * T = RETRIEVE ( env, name );

	if ( T != NULL ) {

	  answer = T;

	} else if ( s->is_recursive ( name ) ) {

	  if ( in_list ( name, rec_vars ) ) {

	    answer = FUNCTION_TYPE ( VAR_TYPE, VAR_TYPE );

	  } else {

	    Value * v = s->get(name);
	    rec_vars->push_back ( name );
	    answer = v->compute_type ( s, env, non_generics, garbage );

	  }

	} else {

	  Value * v = s->get(name);

	  if ( v == NULL ) {

          TYPE_ERROR ( "The identifier <i>" << name << "</i> does not refer to a value (it may be undefined, or it may refer to something like a program, which is not okay in the present context)." );

	  } else if ( v->get_type() == Value::EX_FUNCTION ) {

          TYPE_ERROR ( "Attempted to use the external function <i>" << name << "</i> in an incorrect context." );

	  } else

	    answer = v->compute_type ( s, env, non_generics, garbage );

	}

      }

      break;

    case APPLICATION:

      {

	TypeExpr * ftype = e1->compute_type_aux ( s, env, non_generics, garbage ),
	  * argtype = e2->compute_type_aux ( s, env, non_generics, garbage ),
	  * restype = VAR_TYPE;

	if ( TypeExpr::unify ( ftype, FUNCTION_TYPE ( argtype, restype ) ) )

	  answer = restype;

	else

      TYPE_ERROR2 ( e1, ftype, e2, argtype, "Could not apply function to argument because of incompatible types." );

      }

      break;

    case EX_APPLICATION:

      {

	// copy old variables
	std::list<TypeExpr *> * new_vars = new std::list<TypeExpr *>,
	  * old_vars = val->non_generics();

	std::list<TypeExpr *>::iterator i;

	for ( i = old_vars->begin(); i != old_vars->end(); i++ ) { 
	  TypeExpr * t = new TypeExpr();
	  garbage->push_back ( t );
	  new_vars->push_back ( t );
	}

	std::list<TypeExpr *> * temp_args = new std::list<TypeExpr *>;

	TypeExpr * R = val->return_type()->copy_ex(old_vars, new_vars);
	if ( ! TypeExpr::member ( R, garbage ) )
	  garbage->push_back ( R );

	for ( i = val->arg_types()->begin(); i != val->arg_types()->end(); i++ ) {
	  TypeExpr * t = (*i)->copy_ex ( old_vars, new_vars );
	  temp_args->push_back ( t );
	  if ( ! TypeExpr::member ( t, garbage ) )
	    garbage->push_back ( t );
	}

	std::list<Expr *>::iterator j;

    if ( temp_args->size() > arglist->size() ) {

        TYPE_ERROR ( "wrong number of arguments in call to external function\n" );

    } else {

        answer= R;
        int arg_num ;

        for ( i = temp_args->begin(), j = arglist->begin(), arg_num = 0;
              i != temp_args->end() && (*i)->get_type() != TypeExpr::VARARGS;
              i++, j++, arg_num++ ) {

            if ( (*j)->get_type() != PROGRAM ) {

                TypeExpr * a = (*j)->compute_type_aux ( s, env, non_generics, garbage );

                if ( ! TypeExpr::unify ( a, *i ) ) {

                    std::stringstream strm;

                    strm << "Type error in "
                         << ( file_name() != NULL ? file_name() : "no file information" )
                         << " on line " << line_num()
                         << "<br><blockquote>" << " Argument " << arg_num+1 << " to external function has wrong type. In particular, "
                         << "<i>" << (*j)->tostring() << "</i> has type <i>" << a->tostring() << "</i> but the argument type should in fact be "
                         << "<i>" << (*i)->tostring() << "</i>.</blockquote>";

                    throw strm.str();

                }


            }

        }

        if ( i != temp_args->end() && (*i)->get_type() == TypeExpr::VARARGS ) {

        } else if ( temp_args->size() < arglist->size() ) {

            // not enough args
            TYPE_ERROR ( "Not enough arguments in call to external function." );

        }

    }

	// type check any remaining args

	while ( j != arglist->end() ) {

	  (*j)->compute_type_aux ( s, env, non_generics, garbage );
	  j++;

	}
  
	delete new_vars;
        delete temp_args;

      }

      break;

    case UNARY:

      if ( op == PREV ) {

	answer = e1->compute_type_aux ( s, env, non_generics, garbage );

      } else if ( op == HEAD ) {

	TypeExpr * T = e1->compute_type_aux ( s, env, non_generics, garbage );

	if ( ! TypeExpr::unify ( T, LIST_TYPE ( VAR_TYPE ) ) ) {

      TYPE_ERROR1 ( e1, T, "Cannot take head of non-list." );

	} else {

	  answer = T->get_list_base(); 

	}

      } else if ( op == TAIL ) {

	TypeExpr * T = e1->compute_type_aux ( s, env, non_generics, garbage );

	if ( ! TypeExpr::unify ( T, LIST_TYPE ( VAR_TYPE ) ) ) {

      TYPE_ERROR1 ( e1, T, "can not take tail of non-list." );

	} else {

	  answer = T;

	}

      } else  {

	TypeExpr * T = e1->compute_type_aux ( s, env, non_generics, garbage );

	if ( TypeExpr::unify ( T, ATOMIC_TYPE ( op_arg_type ( op ) ) ) )

	  answer = ATOMIC_TYPE ( op_type(op) );

        else {

	  std::list<TypeExpr *>::iterator i;

	  for ( i = garbage->begin(); i != garbage->end(); i++ ) {
	    (*i)->print ( stderr ); fprintf ( stderr, "\n" );
	  }

      TYPE_ERROR1 ( e1, T, "Could not apply unary operator (! or -) to non Boolean." );

	}

      }

      break;

    case BINARY:

      if ( op == EQ || op == NE ) {

	TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
	  * T2 = e2->compute_type_aux ( s, env, non_generics, garbage );
	
	if ( TypeExpr::unify ( T1, T2 ) )
	  
	  answer = ATOMIC_TYPE(Value::BOOLEAN);
	
	else 
	  
      TYPE_ERROR2 ( e1, T1, e2, T2, "Could not compare (using = or !=) arguments with different types." );
	
      } else if ( LIST_OP(op) ) {

	switch ( op ) {

  	  case CONS:

	    {

              TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
                       * T2 = e2->compute_type_aux ( s, env, non_generics, garbage );

	      if ( TypeExpr::unify ( T2, LIST_TYPE ( VAR_TYPE ) ) &&
		   TypeExpr::unify ( T1, T2->get_list_base() ) )

		answer = T2;

	      else 

        TYPE_ERROR2 ( e1, T1, e2, T2, "Cannot insert element into list because of incompatible types." );

	    }

	    break;

	  case CONCAT:

	    {

              TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
                       * T2 = e2->compute_type_aux ( s, env, non_generics, garbage );

	      if ( TypeExpr::unify ( T1, LIST_TYPE ( VAR_TYPE ) ) &&
		   TypeExpr::unify ( T1, T2 ) )

		answer = T2;

	      else 

        TYPE_ERROR2 ( e1, T1, e2, T2, "Cannot concatenate lists of different types." );

	    }

	    break;

	  case LIST_EL:

	    {

              TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
                       * T2 = e2->compute_type_aux ( s, env, non_generics, garbage );

	      if ( TypeExpr::unify ( T1, LIST_TYPE ( VAR_TYPE ) ) &&
		   TypeExpr::unify ( T2, ATOMIC_TYPE ( Value::INTEGER ) ) )

		answer = T1->get_list_base();

	      else 

        TYPE_ERROR2 ( e1, T1, e2, T2, "Cannot take list element because either the argument is not a list, or the index is not an integer." );

	    }

	    break;

	}

      } else if ( op == DOT ) {

	TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage );

	if ( e2->get_type() == VARIABLE &&
	     TypeExpr::unify ( T1, RECORD_TYPE ( new Environment ( e2->get_name(), VAR_TYPE, NULL ) ) ) )

	  answer = RETRIEVE ( T1->get_fields(), e2->get_name() );

	else {

      TYPE_ERROR1 ( e1, T1, "Cannot take (possibly non-existent) field "
                    <<  ( e2->get_type() == VARIABLE ? e2->get_name() : "?" )
                    << " of expression (that may not even be a record)." );

	}

      } else if ( op == RECORD_ADD ) {

	TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
                 * T2 = e2->compute_type_aux ( s, env, non_generics, garbage );

	T1->set_extensibility ( true );
	T2->set_extensibility ( true );

	if ( ! TypeExpr::unify ( T1, RECORD_TYPE ( (Environment *) NULL ) ) ) {

      TYPE_ERROR1 ( e1, T1, "Left argument to &lt;&lt; operator must be a record." );

	} else if ( ! TypeExpr::unify ( T2, RECORD_TYPE ( (Environment *) NULL ) ) ) {

      TYPE_ERROR1 ( e1, T1, "Left argument to &lt;&lt; operator must be a record." );

	} else {

	  if ( ! TypeExpr::unify ( T1, T2 ) ) {

	    TYPE_ERROR2 ( e1, T1, e2, T2, 
          "Cannot apply &lt;&lt; operator because the records have at least one field with the same name but different types." );

	  } else {

	    answer = T1;

          }

	}

      } else {

	TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
                 * T2 = e2->compute_type_aux ( s, env, non_generics, garbage );

        if ( !T1->is_consistent() || !T2->is_consistent() ) {

	  answer = new TypeExpr ( false );

	} else if ( NUM_OP(op) ) {

	  if ( TypeExpr::unify ( T1, ATOMIC_TYPE ( Value::INTEGER ) ) &&
	       TypeExpr::unify ( T2, ATOMIC_TYPE ( Value::INTEGER ) ) )

	    answer = ATOMIC_TYPE ( Value::INTEGER );

	  else if ( TypeExpr::unify ( T1, ATOMIC_TYPE ( Value::INTEGER ) ) &&
		    TypeExpr::unify ( T2, ATOMIC_TYPE ( Value::REAL ) ) )

	    answer = ATOMIC_TYPE ( Value::REAL );


	  else if ( TypeExpr::unify ( T1, ATOMIC_TYPE ( Value::REAL ) ) &&
		    TypeExpr::unify ( T2, ATOMIC_TYPE ( Value::INTEGER ) ) )

	    answer = ATOMIC_TYPE ( Value::REAL );

	  else

        TYPE_ERROR2 ( e1, T1, e2, T2, "Could not apply numerical operator (+,-,*,/,%,^) to arguments of incompatible types." );

	} else if ( TypeExpr::unify ( T1, ATOMIC_TYPE ( op_arg_type ( op ) ) ) &&
		    TypeExpr::unify ( T2, ATOMIC_TYPE ( op_arg_type ( op ) ) ) ) {

	  answer = ATOMIC_TYPE ( op_type ( op ) );

	} else 

      TYPE_ERROR2 ( e1, T1, e2, T2, "Could not apply operator to arguments because of a type error." );

      }

      break;

    case LET:

      {

        TypeExpr * binder = e1->compute_type_aux ( s, env, non_generics, garbage );
	Environment * body_env = new Environment ( name, binder, env );

	answer = e2->compute_type_aux ( s, body_env, non_generics, garbage );

	delete body_env;

      }

      break;

    case IF:

      {

	TypeExpr * T1 = e1->compute_type_aux ( s, env, non_generics, garbage ),
	         * T2 = e2->compute_type_aux ( s, env, non_generics, garbage ),
	         * T3 = e3->compute_type_aux ( s, env, non_generics, garbage );

        if ( TypeExpr::unify ( T1, ATOMIC_TYPE ( Value::BOOLEAN ) ) &&
	     TypeExpr::unify ( T2, T3 ) )

                answer = T2;

	      else 

        TYPE_ERROR1 ( e1, T1, "If statement either has a non-Boolean guard or body parts with incompatible body parts." );

      }

      break;

    case RECORD:      

      {

	Environment * e = NULL;
	std::list<FIELD *>::iterator i;

	for ( i = fields->begin(); i != fields->end(); i++ )
	  e = new Environment ( (*i)->name, (*i)->e->compute_type_aux ( s, env, non_generics, garbage ), e );

	answer = RECORD_TYPE ( e );
	answer->set_extensibility ( false );

      }

      break;

    default:
      throw ( std::string ( "Ugh: compute_type_aux got and undefined expression type." ) );


  }

#ifdef VERBOSE
  INDENT(ctcount-1);
  printf ( "%d. got ", --ctcount ); answer->print(); NL;
#endif 

  return answer;

}

void TypeExpr::take_out ( std::list<TypeExpr *> * garbage, std::list<TypeExpr *> * exceptions ) {

  TypeExpr * T;

  while ( garbage->size() > 0 ) {
    T = *garbage->begin();
    garbage->pop_front();
    if ( !member ( T, exceptions ) )
      delete T;
  }

}

void TypeExpr::take_out ( std::list<TypeExpr *> * garbage ) {

  TypeExpr * T;

  while ( garbage->size() > 0 ) {
    T = *garbage->begin();
    garbage->pop_front();
    delete T;
  }

}

Environment * SymbolTable::to_env ( Scope * s, Environment * env, 
				   std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage ) {

  Environment * e = NULL;
  int i;
  Symbol * temp;

  for ( i=0; i<num_buckets; i++ )

    for ( temp=buckets[i]; temp!=NULL; temp=temp->next )

      e = new Environment ( temp->get_name(), 
			    temp->get_value()->compute_type ( s, env, non_generics, garbage ),
			    e );

  return e;

}

bool Expr::has_type ( Value::TYPE t, Scope * s, Environment * env ) {

  std::list<TypeExpr *> * garbage = new std::list<TypeExpr *>;
  std::list<TypeExpr *> ng;

  RESET(env);

  //printf ( "computing type of " ); print(); NL;

  TypeExpr * T = compute_type ( s, env, &ng, garbage ),
    * U = ATOMIC_TYPE ( t );

  //  printf ( "trying to unify " ); T->print(); printf ( " with " ); U->print(); NL;

  bool answer = TypeExpr::unify ( T, U );

  // printf ( "got " ); T->print(); printf ( " and " ); U->print(); NL;

  TypeExpr::take_out ( garbage ); 
  delete garbage;

  return answer;

}
