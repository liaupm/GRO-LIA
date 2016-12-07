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

#include "Type.h"
#include "Expr.h"

#define NL printf ( "\n" ); fflush ( stdout );
#define FNL fprintf ( stderr, "\n" ); fflush ( stderr );

static int indent_count = 0;
static int type_expr_instances = 0;

#define INC_TYPE { type_expr_instances++; /*INC_INST ( "TypeExpr" );*/ }

TypeExpr::TypeExpr ( void ) : consistent(true), type(VARIABLE) {

  instance = NULL;
  INC_TYPE;

}

void TypeExpr::set_instance ( TypeExpr * t ) { 

  ASSERT ( type == VARIABLE );
  instance = t;

}

TypeExpr::TypeExpr ( Value::TYPE t ) : consistent(true), type(ATOMIC), atomic(t) {

  INC_TYPE;

}

TypeExpr::TypeExpr ( TypeExpr * t ) : consistent(true), type(LIST), list_base(t) {

  INC_TYPE;

}

TypeExpr::TypeExpr ( TypeExpr * t1, TypeExpr * t2 ) : consistent(true), type(FUNCTION), domain(t1), codomain(t2) {

  INC_TYPE;

}

TypeExpr::TypeExpr ( bool b ) : type(VARARGS) {

  if ( b ) {
    consistent = true;
  } else
    consistent = false;

  INC_TYPE;

}

TypeExpr::TypeExpr ( Environment * f ) : consistent(true), type(RECORD), fields(f) {

  INC_TYPE;
  extensible = true;

}

void TypeExpr::add_field ( char * name, TypeExpr * T ) {

  fields = new Environment ( name, T, fields );

}

TypeExpr::~TypeExpr ( void ) {

  // note: this does not delete chlidren, because chidren may have multiple parents

  //DEC_INST ( "TypeExpr" );

  type_expr_instances--;

#ifdef MEMCHECK
  if ( type_expr_instances == 0 ) 
    printf ( "  no type exprs left\n" );
#endif
  if ( type_expr_instances < 0 ) 
    fprintf ( stderr, "  Warning::deleted too many type exprs!\n" );

  if ( type == RECORD && fields != NULL ) {

    Environment * e = fields, * temp;

    while ( e != NULL ) {

      temp = e;
      e = e->tail;
      delete temp;

    }

  }

}


void TypeExpr::free_children ( void ) {

  switch ( type ) {

    case VARIABLE:
    case ATOMIC:

      break;

    case LIST:

      list_base->free_children();
      delete list_base;
      break;

    case FUNCTION:

      domain->free_children();
      codomain->free_children();
      delete domain;
      delete codomain;
      break;

    case RECORD:

      {

	Environment * e = fields;

	while ( e != NULL ) {

	  e->T->free_children();
	  if ( !e->freed ) {
#ifdef MEMCHECK
	    printf ( "freeing children (1): %x\n", e->T );
#endif
   	    delete e->T;
	    e->freed = true;
	  }
	  e = e->tail;

	}

      }

      break;

  }

}

void TypeExpr::free_children ( std::list<TypeExpr *> * ng ) {

  switch ( type ) {

    case VARIABLE:
    case ATOMIC:

      break;

    case LIST:

      if ( ! member ( list_base, ng ) ) {
        list_base->free_children();
        delete list_base;
      }

      break;

    case FUNCTION:

      if ( ! member ( domain, ng ) ) {
	domain->free_children();
	delete domain;
      }

      if ( ! member ( codomain, ng ) ) {
	codomain->free_children();
	delete codomain;
      }

      break;

    case RECORD:

      {

	Environment * e = fields;

	while ( e != NULL ) {

	  if ( ! member ( e->T, ng ) ) {
	    e->T->free_children();

            if ( !e->freed ) {
#ifdef MEMCHECK
	      printf ( "freeing children (2): %x\n", e->T );
#endif
              delete e->T;
	      e->freed = true;
            }

	  }

	  e = e->tail;

	}

      }

      break;

  }

}

TypeExpr * TypeExpr::get_list_base ( void ) {

  ASSERT ( type == LIST || type == VARIABLE );

  if ( type == VARIABLE ) {

    ASSERT ( instance != NULL );
    return instance->get_list_base();

  } else {

    return list_base;

  }

}

Value::TYPE TypeExpr::get_atomic_type ( void ) { 

  if ( type == VARIABLE ) {

    if ( instance != NULL )
      return instance->get_atomic_type();
    else
      ASSERT_MSG ( false, "Tried to get atomic type of non atomic\n" );

  } else if ( type == ATOMIC ) { 

    return atomic;

  } else ASSERT_MSG ( false, "Tried to get atomic type of non atomic\n" );

}

TypeExpr::TYPE TypeExpr::get_type ( void ) { 

  if ( type == VARIABLE ) {

    if ( instance != NULL )
      return instance->get_type();
    else
      return VARIABLE;

  } else 

    return type;

}

TypeExpr * TypeExpr::get_domain ( void ) {

  ASSERT ( type == FUNCTION || type == VARIABLE );

  if ( type == VARIABLE ) {

    ASSERT ( instance != NULL );
    return instance->get_domain();

  } else {

    return domain;

  }

}

TypeExpr * TypeExpr::get_codomain ( void ) {

  ASSERT ( type == FUNCTION || type == VARIABLE );

  if ( type == VARIABLE ) {

    ASSERT ( instance != NULL );
    return instance->get_codomain();

  } else {

    return codomain;

  }

}

Environment * TypeExpr::get_fields ( void ) {

  ASSERT ( type == RECORD || type == VARIABLE );

  if ( type == VARIABLE ) {

    ASSERT ( instance != NULL );
    return instance->get_fields();

  } else {

    return fields;

  }

}

static int type_var_count;
static char type_var_names[] = "abcdefghijklmnopqrstuvwxyz";

void TypeExpr::print ( FILE * fp ) {

  clear_var_names();
  print_aux ( fp );

}

void TypeExpr::clear_var_names ( void ) {

  type_var_count = 0;

  switch ( type ) {

    case VARIABLE:

      if ( instance != NULL )
	instance->clear_var_names();

      var_name = -1;
      break;

    case ATOMIC:

      break;

    case LIST:

      list_base->clear_var_names();
      break;

    case FUNCTION:

      domain->clear_var_names();
      codomain->clear_var_names();
      break;

    case RECORD:

      {

	Environment * e = fields;

	while ( e != NULL ) {

	  e->T->clear_var_names();
	  e = e->tail;

	}

      }

      break;

  }

}

void TypeExpr::print_aux ( FILE * fp ) {

  switch ( type ) {

    case VARIABLE:

      if ( instance != NULL ) {

         instance->print_aux ( fp );

      } else {

        if ( var_name == -1 ) {
	  var_name = type_var_count++;
	}

        if ( var_name < 26 )
          fprintf ( fp, "'%c", type_var_names[var_name], this );
        else
          fprintf ( fp, "'a%d(%x)", var_name, this );

      }

      break;

    case ATOMIC:

      switch ( atomic ) {

        case Value::UNIT:

	  fprintf ( fp, "." );
	  break;

        case Value::BOOLEAN:

	  fprintf ( fp, "boolean" );
	  break;

        case Value::INTEGER:

	  fprintf ( fp, "integer" );
	  break;

        case Value::REAL:

	  fprintf ( fp, "real" );
	  break;

        case Value::STRING:

	  fprintf ( fp, "string" );
	  break;

        case Value::RECORD:

	  fprintf ( fp, "record" );
	  break;

        case Value::LIST:
        case Value::FUNCTION:
        case Value::EX_FUNCTION:

	  fprintf ( fp, "???" );
	  break;

      }

      break;

    case LIST:

      list_base->print_aux(fp);
      fprintf ( fp, " list" );
      break;

    case FUNCTION:

      domain->print_aux(fp);
      fprintf ( fp, " -> " );
      codomain->print_aux(fp);
      break;

    case VARARGS:

      if ( consistent )
        fprintf ( fp, "..." );
      else 
	fprintf ( fp, "<<inconsistent type>>" );
      break;

    case RECORD:

      {

	Environment * e = fields;

	fprintf ( fp, "[ " );

	while ( e != NULL ) {

	  fprintf ( fp, "%s := ", e->name );
	  e->T->print(fp);
	  e = e->tail;
	  if ( e != NULL )
	    fprintf ( fp, ", " );

	}

	if ( extensible ) 
	  fprintf ( fp, ", ... " );

	fprintf ( fp, " ]" );

      }

      break;

    default: 

      fprintf ( fp, "<<UNKOWN TYPE (this is a bug)>>" );

  }

}

std::string TypeExpr::tostring ( void ) {

    clear_var_names();
    std::stringstream strm;
    tostring_aux ( & strm );
    return strm.str();

}

void TypeExpr::tostring_aux ( std::stringstream * strm ) {

    switch ( type ) {

    case VARIABLE:

        if ( instance != NULL ) {

            instance->tostring_aux ( strm );

        } else {

            if ( var_name == -1 ) {
                var_name = type_var_count++;
            }

            if ( var_name < 26 )
                *strm << type_var_names[var_name];
            else
                *strm << "'qqq";

        }

        break;

    case ATOMIC:

        switch ( atomic ) {

        case Value::UNIT:

            *strm <<  ".";
            break;

        case Value::BOOLEAN:

            *strm << "boolean";
            break;

        case Value::INTEGER:

            *strm << "integer";
            break;

        case Value::REAL:

            *strm << "real";
            break;

        case Value::STRING:

            *strm << "string";
            break;

        case Value::RECORD:

            *strm << "record";
            break;

        case Value::LIST:
        case Value::FUNCTION:
        case Value::EX_FUNCTION:

            *strm << "???";
            break;

        }

        break;

    case LIST:

        list_base->tostring_aux ( strm ) ;
        *strm << " list";
        break;

    case FUNCTION:

        domain->tostring_aux ( strm );
        *strm << " -> ";
        codomain->tostring_aux ( strm );
        break;

    case VARARGS:

        if ( consistent )
            *strm << "...";
        else
            *strm << "<<inconsistent type>>";
        break;

    case RECORD:

    {

        Environment * e = fields;

        *strm << "[ ";

        while ( e != NULL ) {

            *strm << e->name << " :=";
            e->T->tostring_aux(strm);
            e = e->tail;
            if ( e != NULL )
                *strm << ", ";

        }

        if ( extensible )
            *strm << ", ... ";

        *strm << " ]";

    }

        break;

    default:

        throw ( std::string ( "<<UNKOWN TYPE (this is a bug)>>" ) );

    }

}


bool TypeExpr::occurs_in ( TypeExpr * v, TypeExpr * T ) {

  ASSERT ( v->type == VARIABLE );

  bool answer = false;

  T = prune ( T );

  switch ( T->type ) {

   case VARIABLE:

     answer = (  v == T );
     break;

    case ATOMIC:

      answer = false;
      break;

    case LIST:

      answer = occurs_in ( v,T->list_base );
      break;

    case FUNCTION:

      answer = occurs_in ( v, T->domain ) || occurs_in ( v, T->codomain );
      break;

    case RECORD:

      {

	Environment * e = T->fields;

	answer = false;

	while ( e != NULL ) {

	  answer = answer || occurs_in ( v, e->T );
	  e = e->tail;

	}

      }

      break;

  }

  return answer;

}

TypeExpr * TypeExpr::prune ( TypeExpr * T ) {

  TypeExpr * answer = NULL;

  if ( T->type == VARIABLE ) {

    if ( T->instance == NULL ) 
      answer = T;
    else {
      T->instance = prune ( T->instance );
      answer = T->instance;
    }

  } else {

    answer = T;

  }

  return answer;

}

bool TypeExpr::unify ( TypeExpr * T1, TypeExpr * T2 ) {

  T1 = prune ( T1 );
  T2 = prune ( T2 );

  bool answer = false;

  switch ( T1->type ) {

    case VARIABLE:

      if ( occurs_in ( T1, T2 ) ) 
	answer = ( T1 == T2 );
      else {
	T1->set_instance ( T2 );
	answer = true;
      }
      break;

    case ATOMIC:

      switch ( T2->type ) {

        case VARIABLE:

	  answer = unify ( T2, T1 );
	  break;

        case ATOMIC:

	  if ( ( T1->atomic == Value::REAL && T2->atomic == Value::INTEGER ) || 
	       ( T1->atomic == Value::INTEGER && T2->atomic == Value::REAL ) )

	    answer = true;

	  else

	    answer = ( T1->atomic == T2->atomic );

	  break;

        default:

	  answer = false;
	  break;

      }

      break;

    case LIST:

      switch ( T2->type ){

        case VARIABLE:

	  answer = unify ( T2, T1 );
	  break;

        case LIST:

	  answer = unify ( T1->list_base, T2->list_base );
	  break;

        default:

	  answer = false;
	  break;

      }

      break;

    case FUNCTION:

      switch ( T2->type ) {

        case VARIABLE:

	  answer = unify ( T2, T1 );
	  break;

        case FUNCTION:

	  answer = unify ( T1->domain, T2->domain ) && unify ( T1->codomain, T2->codomain );
	  break;

        default:

	  answer = false;
	  break;

      }

      break;

    case RECORD:

      switch ( T2->type ) {

        case VARIABLE:

	  answer = unify ( T2, T1 );
	  break;

        case RECORD:

	  {

	    answer = true;
	    Environment * f1 = T1->fields,
	      * f2 = T2->fields;

	    while ( answer && f1 != NULL ) {

	      TypeExpr * T = RETRIEVE ( f2, f1->name );

	      if ( T != NULL ) 
		answer = answer && unify ( T, f1->T );
	      else if ( T2->is_extensible() ) {
		f2 = new Environment ( f1->name, f1->T, f2 );
		T2->fields = f2;
	      } else {
		answer = false;
                return answer;
              }

	      f1 = f1->tail;

	    }

	    f1 = T1->fields;
            f2 = T2->fields;

	    while ( answer && f2 != NULL ) {

	      TypeExpr * T = RETRIEVE ( f1, f2->name );

	      if ( T != NULL ) 
		answer = answer && unify ( T, f2->T );
	      else  if ( T1->is_extensible() ) {
		f1 = new Environment ( f2->name, f2->T, f1 );
		T1->fields = f1;
	      } else {
		answer = false;
		return answer;
              }

	      f2 = f2->tail;

	    }

	  }

	  break;

        default:

	  answer = false;
	  break;

      }

      break;

  }

  return answer;

}

static int env_count = 0;

Environment::Environment ( char * str, TypeExpr * t, Environment * e ) : T(t), tail(e) {

  env_count++; 
  name = strdup ( str );
  freed = false;

  //INC_INST ( "Environment" );

}

Environment::~Environment ( void ) {

  //DEC_INST ( "Environment" );

  env_count--; 

  //  if ( env_count == 0 )
  //  printf ( "  no environments left\n" );
  
  if ( env_count < 0 )
    fprintf ( stderr, "  Warning::deleted too many type environments!\n" );

  free ( name );

}

void Environment::print ( void ) {

  Environment * e = this;

  while ( e != NULL ) {

    printf ( "     %s(", e->name ); e->T->print(); printf ( ", %x)\n", e->T );
    e = e->tail;

  }

}

TypeExpr * Environment::retrieve ( char * str ) {

  if ( ! strcmp ( str, name ) )
    return T;
  else if ( tail != NULL )
    return tail->retrieve ( str );
  else
    return NULL;

}

void TypeExpr::reset ( void ) {

  switch ( type ) {
      
    case VARIABLE:

	instance = NULL;
	break;
	
    case LIST:
	
      list_base->reset();
      break;
	
    case FUNCTION:

      domain->reset();
      codomain->reset();
      break;

    case RECORD:

      fields->reset();
      break;
	
    default:

      break;

  }

}

void Environment::reset ( void ) {

  Environment * e = this;

  while ( e ) {

    e->T->reset();
    e = e->tail;
      
  }

}

Environment * Environment::copy_ex ( std::list<TypeExpr *> * old_vars, std::list<TypeExpr *> * new_vars ) {

  Environment * env = NULL, * temp = this;
  std::list<char *> names;
  std::list<TypeExpr *> types;
  std::list<char *>::iterator i;
  std::list<TypeExpr *>::iterator j;

  while ( temp != NULL ) {
    names.push_front ( temp->get_name() );
    types.push_front ( temp->get_type() );
    temp = temp->get_tail();
  }

  for ( i = names.begin(), j = types.begin(); i != names.end(); i++, j++ ) {
    env = new Environment ( *i, (*j)->copy_ex ( old_vars, new_vars ), env );
  }

  return env;

}

bool TypeExpr::member ( TypeExpr * t, std::list<TypeExpr *> * L ) {

  std::list<TypeExpr *>::iterator i;

  for ( i=L->begin(); i != L->end(); i++ ) 
    if ( (*i) == t )
      return true;

  return false;

}

TypeExpr * TypeExpr::copy ( std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage ) {

  TypeExpr * result;

  switch ( type ) {

   case VARIABLE:

     if ( member ( this, non_generics ) ) 
       result = this;
     else if ( instance == NULL ) {
       result = new TypeExpr();
       garbage->push_back ( result );
     } else {
       result = instance->copy ( non_generics, garbage );
     }
     break;

    case ATOMIC:
    case VARARGS:

      result = this;
      break;

    case LIST:

      result = new TypeExpr ( list_base->copy ( non_generics, garbage ) );
      garbage->push_back ( result );
      break;

    case FUNCTION:

      result = new TypeExpr ( domain->copy ( non_generics, garbage ), 
			      codomain->copy ( non_generics, garbage ) );
      garbage->push_back ( result );
      break;

    case RECORD:

      {

	Environment * new_env = NULL, * e = fields;

	while ( e != NULL ) {

	  new_env = new Environment ( e->name, e->T->copy ( non_generics, garbage ), new_env );
	  e = e->tail;

	}

	result = new TypeExpr ( new_env );
	result->set_extensibility ( is_extensible() );

      }

      break;

  }

  return result;

}

TypeExpr * TypeExpr::copy_ex ( std::list<TypeExpr *> * old_vars, std::list<TypeExpr *> * new_vars ) {

  TypeExpr * result;
  std::list<TypeExpr *>::iterator i;

  switch ( type ) {

   case VARIABLE:

     {
       if ( instance == NULL ) {

	 ASSERT ( old_vars->size() <= new_vars->size() );

	 std::list<TypeExpr *>::iterator i, j;
	 i = old_vars->begin();
	 j = new_vars->begin();
	 while ( (*i) != this && i != old_vars->end() ) { 
           i++; j++; 
         }

	 ASSERT ( i != old_vars->end() );
	 result = *j; // why is this not a new type expr? as in result = new TypeExpr();?

       } else {

	 result = instance->copy_ex ( old_vars, new_vars );

       }

     }

     break;

    case ATOMIC:

      result = new TypeExpr ( atomic );
      break;

    case VARARGS:

      result = new TypeExpr ( type == VARARGS );
      break;

    case LIST:

      result = new TypeExpr ( list_base->copy_ex ( old_vars, new_vars ) );
      break;

    case FUNCTION:

      result = new TypeExpr ( domain->copy_ex ( old_vars, new_vars ),
			      codomain->copy_ex ( old_vars, new_vars ) );
      break;

    case RECORD:

      {

	Environment * new_env = NULL, * e = fields;

	while ( e != NULL ) {

	  new_env = new Environment ( e->name, e->T->copy_ex ( old_vars, new_vars ), new_env );
	  e = e->tail;

	}

	result = new TypeExpr ( new_env );
	result->set_extensibility ( is_extensible() );

      }

      break;

  }

  return result;

}

void TypeExpr::compute_non_generics ( std::list<TypeExpr *> * non_generics ) {

  switch ( type ) {

   case VARIABLE:

     if ( instance == NULL ) {
       if ( ! member ( this, non_generics ) ) {
	 non_generics->push_back ( this );
       }
     } else 
       instance->compute_non_generics ( non_generics );

     break;

    case ATOMIC:
    case VARARGS:

      break;

    case LIST:

      list_base->compute_non_generics ( non_generics );
      break;

    case FUNCTION:

      domain->compute_non_generics ( non_generics );
      codomain->compute_non_generics ( non_generics );
      break;

    case RECORD:

      {

	Environment * e = fields;

	while ( e != NULL ) {

	  e->T->compute_non_generics ( non_generics );
	  e = e->tail;

	}

      }

      break;

  }

}

Value * TypeExpr::default_value ( void ) {

  Value * answer = NULL;

  switch ( type ) {

   case VARIABLE:

     if ( instance == NULL )
       answer = new Value ( Value::UNIT );
     else
       answer = instance->default_value();

     break;

    case ATOMIC:

      switch ( atomic ) {

        case Value::UNIT:

	  answer = new Value ( Value::UNIT );
	  break;

        case Value::BOOLEAN:

	  answer = new Value ( false );
	  break;

        case Value::INTEGER:

	  answer = new Value ( 0 );
	  break;

        case Value::REAL:

	  answer = new Value ( 0.0 );
	  break;

        case Value::STRING:

	  answer = new Value ( "" );
	  break;

        case Value::RECORD:
        case Value::LIST:
        case Value::FUNCTION:
        case Value::EX_FUNCTION:

	  ASSERT ( false );
	  break;

      }
   
      break;

    case LIST:

      answer = new Value ( new std::list<Value *> );
      break;

    case FUNCTION:

      answer = new Value ( "x", new Expr ( codomain->default_value() ) );
      break;

    case RECORD:

      {

	Environment * e = fields;
	answer = new Value ( Value::RECORD );

	while ( e != NULL ) {

	  answer->addField ( e->get_name(), e->get_type()->default_value() );
	  e = e->tail;

	}

      }

      break;

  }

  return answer;

}


bool TypeExpr::in_use ( TypeExpr * t ) { // returns true if the TypeExpr pointed to by t is also pointed to
                                         // by some sub expressionof this

  std::list<TypeExpr *>::iterator i;
  bool answer;

  switch ( type ) {

   case VARIABLE:

     {
       if ( instance == NULL ) {

	 answer = ( this == t );

       } else {

	 answer = instance->in_use(t);

       }

     }

     break;

    case ATOMIC:
    case VARARGS:

      answer = ( this == t );
      break;

    case LIST:

      answer = new TypeExpr ( list_base->in_use(t) );
      break;

    case FUNCTION:

      answer = domain->in_use(t) || codomain->in_use(t);
      break;

    case RECORD:

      {

	Environment * new_env = NULL, * e = fields;

	while ( e != NULL ) {

	  answer = answer || e->get_type()->in_use(t);
	  e = e->tail;

	}

      }

      break;

  }

  return answer;


}
