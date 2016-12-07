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

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
}

#include <stdio.h>
#include "Program.h"

#define NL printf ( "\n" ); fflush(stdout);
#define INDENT { int i; for(i=0;i<indent_count;i++) printf(" "); }
#define INDENT_SIZE 4
#define INC_INDENT indent_count+=INDENT_SIZE;
#define DEC_INDENT indent_count-=INDENT_SIZE;

static int indent_count = 0;

static int num_progs = 0;

// Program ----------------------------------------------

static int program_count = 0;

Program::Program ( char * file, int line ) : filename ( file ), linenum ( line ) {

  // INC_INST ( "Program" );

  name = strdup ( "no name" ); 

  symtab = new SymbolTable ( NEW_SYMTAB_SIZE );

  param_symtab = new SymbolTable ( NEW_PARAM_SYMTAB_SIZE );
  args = new std::list<char *>;
  needs = new std::list<char *>;
  insts = new std::list<Expr *>;
  id = program_count++;
  non_generics = NULL;
  locals = NULL;
  arg_types = NULL;

  garbage = new std::list<TypeExpr *>;

}

Program::Program ( bool alloc, char * file, int line ) : filename ( file ), linenum ( line ) {

  //INC_INST ( "Program" );

  if ( alloc ) {
    symtab = new SymbolTable ( NEW_SYMTAB_SIZE );
  } else {
    symtab = NULL;
  } 

  param_symtab = new SymbolTable ( NEW_PARAM_SYMTAB_SIZE );
  name = strdup ( "no name" );
  args = new std::list<char *>;
  needs = new std::list<char *>;
  insts = new std::list<Expr *>;
  id = program_count++;
  non_generics = NULL;
  locals = NULL;
  arg_types = NULL;
  garbage = new std::list<TypeExpr *>;

  //  printf ( "new: num_progs now %d\n", ++num_progs );

}

void Environment::delete_rest ( std::list<TypeExpr *> * exceptions ) {

  std::list<TypeExpr *>::iterator i; 
  bool f = false;

  if ( exceptions != NULL ) {
    for ( i=exceptions->begin(); i != exceptions->end(); i++ ) {
      if ( (*i) == T ) // need to also check that *i is not equal to any children of T /////////////
                       // since they might be re-freed when the non_generics are freed? ////////////
        f = true;
    }
  }

  if ( !f ) {
    if ( !freed ) {
      T->free_children(exceptions);
      delete T;
      freed = true;
    }
  }

  if ( tail != NULL ) {
    tail->delete_rest(exceptions);
    delete tail;
  }

}

Program::~Program ( void ) {

  DEC_INST ( "Program" );

  std::list<char*>::iterator j;

  for ( j=args->begin(); j!=args->end(); j++ )
    free (*j);

  for ( j=needs->begin(); j!=needs->end(); j++ )
    free (*j);

  std::list<Expr *>::iterator k;

  for ( k=insts->begin(); k!=insts->end(); k++ ) {
    delete (*k);
  }

  delete symtab;
  delete param_symtab;
  delete args;
  delete insts;

  {
    TypeExpr * T;

    if ( garbage != NULL ) {

      while ( garbage->size() > 0 ) {

        T = *garbage->begin();
        garbage->pop_front();
        if ( ! TypeExpr::member ( T, non_generics ) ) { 
          delete T;
        }
      }
    }
    delete garbage;
    garbage = NULL;

  }

  if ( locals != NULL ) {
    locals->delete_rest(non_generics); ////
    delete locals;
  }

  if ( arg_types != NULL ) {
    arg_types->delete_rest(non_generics);
    delete arg_types;
  }

  if ( non_generics != NULL ) {
    std::list<TypeExpr *>::iterator w;

    for ( w=non_generics->begin(); w != non_generics->end(); w++ ) {
      (*w)->free_children(); ////
      delete (*w);
    }
    delete non_generics;
  }

  delete needs;

  free ( name );

}

void Program::set_args ( std::list<char *> * a ) {

  std::list<char *>::iterator i;

  for ( i=args->begin(); i!=args->end(); i++ )
    free(*i);

  delete args;
  args = a;

}

void Program::set_insts_simp ( std::list<Expr *> * in ) {

  std::list<Expr *>::iterator k;

  for ( k=insts->begin(); k!=insts->end(); k++ )
    delete *k;

  delete insts;
  insts = in;

}

void Program::set_insts ( std::list<Expr *> * in, Scope * s, Environment * env, char * name ) {

  std::list<Expr *>::iterator k;

  for ( k=insts->begin(); k!=insts->end(); k++ )
    delete *k;

  delete insts;
  insts = in;

  // type checking and unification of parameters and local variables

  std::list<TypeExpr *> * temp_args = new std::list<TypeExpr *>;
  Environment * temp = arg_types;

  while ( temp != NULL ) {
    TypeExpr * t = temp->get_type();
    temp_args->push_front ( t );
    temp = temp->get_tail();
  }

  int arg_num;
  std::list<TypeExpr *> ng;
  std::list<Expr *>::iterator j;
  std::list<TypeExpr *>::iterator i;

  for ( i = temp_args->begin(), j = in->begin(), arg_num = 1;
	i != temp_args->end(), j != in->end(); 
	i++, j++, arg_num++ ) {

    TypeExpr * a = (*j)->compute_type ( s, env, &ng, garbage );

    if ( ! TypeExpr::unify ( a, *i ) ) {

      fprintf ( stderr, "Type Error: Near line %d of %s, program %s is called with an argument of type\n            \"", 
		(*j)->line_num(), (*j)->file_name() != NULL ? (*j)->file_name() : "no file information", name );
      a->print ( stderr ); 
      fprintf ( stderr,    "\" in for argument %d, but in this context, argument %d should have type\n            \"", arg_num, arg_num );
      (*i)->print ( stderr );
      fprintf ( stderr, "\".\n" );
      exit ( -1 );

    }

  }

  Environment * e = get_arg_types();

  while ( e != NULL ) {
    e->get_type()->compute_non_generics ( get_non_generics() );
    e = e->get_tail();
  }

  // Note: Possible memory leak here. Have to take out the garbage, except for variables in the
  // new non_generics
  // TypeExpr::take_out ( garbage, get_non_generics() );


  delete temp_args;
  

}

void Program::add_symbol ( const char * str, Value * v ) {

  symtab->add ( str, v );

}

void Program::small_print_base ( void ) {

  std::list<char *>::iterator i, j; 
  std::list<Expr *>::iterator k, l;

  INDENT;
  printf ( "program %d ( ", id );
  for ( i=args->begin(); i!=args->end(); i++ ) {
    j=i;
    j++;
    printf ( "%s", (*i) );
    if ( j!=args->end() )
      printf ( ", " );
  }

  printf ( " )" );

  if ( insts->size() > 0 ) {

    printf ( " => ( " );
    for ( k=insts->begin(); k!=insts->end(); k++ ) {
      l=k;
      l++;
      (*k)->print(); 
      if ( l!=insts->end() )
        printf ( ", " );
    }

    printf ( " ), " );

  } else printf ( ", " );

  printf ( "{ " );
  symtab->print_names(stdout,0);
  printf ( "}" );

}

void Program::print_args ( void ) {

  std::list<char *>::iterator i, j;

  printf ( "program ( " );
  for ( i=args->begin(); i!=args->end(); i++ ) {
    j=i;
    j++;
    printf ( "%s", (*i) );
    if ( j!=args->end() )
      printf ( ", " );
  }

  printf ( " ) => " );

  std::list<Expr *>::iterator k, l;

  if ( insts->size() > 0 ) {

    printf ( "    ( " );
    for ( k=insts->begin(); k!=insts->end(); k++ ) {
      l=k;
      l++;
      (*k)->print(); 
      if ( l!=insts->end() )
        printf ( ", " );
    }

    printf ( " )\n" );

  } else 

    printf ( "no argument instantiatons\n" );

  printf ( "  symtab:\n\n" );
  symtab->print();

}

void Program::set_base ( SymbolTable * s, std::list<char *> * alist, std::list<Expr *> * elist ) {

  delete symtab;
  set_symtab ( s->copy() );

  std::list<char *> * a = new std::list<char *>;
  std::list<char *>::iterator k;

  for ( k=alist->begin(); k!=alist->end(); k++ )
    a->push_back ( strdup ( (*k) ) );

  set_args ( a );

  std::list<Expr *> * new_insts = new std::list<Expr *>;
  std::list<Expr *>::iterator j;

  for ( j=elist->begin(); j!=elist->end(); j++ ) 
    new_insts->push_back ( (*j)->copy() ); 
 
  set_insts_simp ( new_insts );

}

// AtomicProgram ----------------------------------------

AtomicProgram::AtomicProgram ( char * file, int line ) : Program ( file, line ) {

  //INC_INST ( "AtomicProgram" );

  clauses = new std::list<Clause *>;
  init_clause = NULL;

}

AtomicProgram::~AtomicProgram ( void ) {

  // DEC_INST ( "AtomicProgram" );

  std::list<Clause *>::iterator i;

  for ( i = clauses->begin(); i != clauses->end(); i++ )
    delete ( *i );

  delete clauses;
  
  if ( init_clause != NULL )
    delete init_clause;

}

void AtomicProgram::add_clause ( Clause * c ) {

  clauses->push_back ( c );

}

void AtomicProgram::small_print ( void ) {

  small_print_base(); NL;

}

void AtomicProgram::print ( void ) {

  print_args();

  if ( init_clause != NULL ) {
    printf ( "  Initial:\n\n" ); 
    init_clause->print_as_init(); NL;
  }

  printf ( "  Clauses:\n\n" );

  std::list<Clause *>::iterator i;

  for ( i = clauses->begin(); i != clauses->end(); i++ ) {
    (*i)->print(); NL;
  }

}

void Program::copy_param_symtab ( SymbolTable * ps ) {

  delete param_symtab;
  param_symtab = ps->copy();

}

Program * AtomicProgram::copy ( void ) {

  std::list<TypeExpr *>::iterator i;

  AtomicProgram * p = new AtomicProgram ( get_filename(), get_linenum() );
  p->set_name ( get_name() );
  p->set_init ( init_clause->copy() );

  std::list<Clause *> * cl = clauses;
  std::list<Clause *>::iterator j;

  for ( j=cl->begin(); j!=cl->end(); j++ ) 
    p->add_clause( (*j)->copy() );

  p->set_base ( get_symtab(), get_args(), get_insts() );

  // copy type info
  std::list<TypeExpr *> * vars = new std::list<TypeExpr *>;

  COPY_TYPEVAR_LIST ( get_non_generics(), vars );

  p->set_non_generics ( vars );

  for ( i = get_non_generics()->begin(); i != get_non_generics()->end(); i++ )
    *i = (*i)->prune(*i);

  if ( get_locals() != NULL ) 
    p->set_locals ( get_locals()->copy_ex ( get_non_generics(), vars ) );

  if ( get_arg_types() != NULL )
    p->set_arg_types ( get_arg_types()->copy_ex ( get_non_generics(), vars ) );

  std::list<char *> * L;
  COPY_STR_LIST ( get_needs(), L );
  p->set_needs ( L );

  p->copy_param_symtab ( get_param_symtab() );

  return p;

}

// Instantiated Program ----------------------------------

InstProgram::InstProgram ( Program * c, char * file, int line ) : Program ( file, line ) {

  //INC_INST ( "InstProgram" );

  child = c;
  child->set_parent(this);
  delete get_symtab();
  set_symtab ( child->get_symtab() );
  child->set_symtab ( new SymbolTable ( NEW_SYMTAB_SIZE ) );

  // local variables
  std::list<TypeExpr *> * vars = new std::list<TypeExpr *>;
  std::list<TypeExpr *>::iterator i;

  COPY_TYPEVAR_LIST ( c->get_non_generics(), vars ); // copies the types in c->non_generics to vars

  set_non_generics ( vars ); // sets the non_generics of the current program to vars, these should be deleted in ~Program

  Environment * env = NULL;

  if ( c->get_locals() != NULL )  {
    env = c->get_locals()->copy_ex ( c->get_non_generics(), vars );
  }

  set_locals ( env );
  set_name ( c->get_name() );

  std::list<char *> * new_needs;
  COPY_STR_LIST ( c->get_needs(), new_needs );
  set_needs ( new_needs );

}

InstProgram::~InstProgram ( void ) {

  //DEC_INST ( "InstProgram" );

  delete child;

}

void InstProgram::print ( void ) {

  print_args(); printf ( "instantiation of " );
  child->print();

}

void InstProgram::small_print ( void ) {

  small_print_base();
  printf ( ", instantiation of\n" );

  INC_INDENT;
    child->small_print();
  DEC_INDENT;

}

Program * InstProgram::copy ( void ) {

  InstProgram * p = new InstProgram ( child->copy(), get_filename(), get_linenum() );
  p->set_name ( get_name() );
  p->set_base ( get_symtab(), get_args(), get_insts() );

  // copy type info
  std::list<TypeExpr *>::iterator i;
  std::list<TypeExpr *> * vars = new std::list<TypeExpr *>;
  for ( i = get_non_generics()->begin(); i!= get_non_generics()->end(); i++ )
    vars->push_back ( new TypeExpr() );

  p->set_non_generics ( vars );

  if ( get_locals() != NULL ) {
    Environment * temp = p->get_locals();
    std::list<TypeExpr *> * temp2 = new std::list<TypeExpr *>;
    p->set_locals ( get_locals()->copy_ex ( get_non_generics(), vars ) );
    temp->delete_rest(temp2);
    delete temp;
    delete temp2;
  }

  if ( get_arg_types() != NULL )
    p->set_arg_types ( get_arg_types()->copy_ex ( get_non_generics(), vars ) );

  return p;

}

void InstProgram::compute_arg_types ( Environment * env ) {

  // the arg types of this program should be the same as the type in env
  // except with an "isomorphic" set of program variables

  Environment * e = env;
  std::list<TypeExpr *> * vars = new std::list<TypeExpr *>, 
                        * ng_save = new std::list<TypeExpr *>;
  
  while ( e != NULL ) {
    e->get_type()->compute_non_generics ( vars );
    e = e->get_tail();
  }
	
  COPY_TYPEVAR_LIST ( vars, ng_save );

  e = NULL;

  if ( env != NULL ) 
    e = env->copy_ex ( vars, ng_save );

  set_arg_types ( e );

  // tack on new non_generics (this is worrying me: some of the non-generics
  // in ng_save might really be the same as those in the child program's non_generics)

  while ( ! ng_save->empty() ) {
    get_non_generics()->push_back ( ng_save->front() );
    ng_save->pop_front();
  }

  delete vars;
  delete ng_save;

}

// Composite Program -------------------------------------

CompositeProgram::CompositeProgram ( Program * l, Program * r, std::list<char *> * shared, char * file, int line ) 
  : Program ( false, file, line ) {

  left = l;
  right = r; 

  left->set_parent(this);
  right->set_parent(this);

  SymbolTable * st = new SymbolTable ( NEW_SYMTAB_SIZE );

  std::list<char *>::iterator j;
  SymbolTable * lsym = left->get_symtab(),
              * rsym = right->get_symtab();

  for ( j=shared->begin(); j!=shared->end(); j++ ) {

    Value * v1 = lsym->get ( *j ), 
          * v2 = rsym->get ( *j );

    if ( v2 != NULL ) { // use right's value if right defines it
                        // irrespective of whether left defines it
      v2 = v2->copy();
      lsym->del ( *j ); // deletes the symbol, including the string name and value, from lsym
      rsym->del ( *j ); // which is why v2 is copied before this
      st->add ( *j, v2 );

    } else if ( v1 != NULL ) { // right doesn't define it, use left's value

      v1 = v1->copy();
      lsym->del ( *j );
      st->add ( *j, v1 );

    } else { // neither define it: add something stupid and issue a warning

      fprintf ( stderr, "Warning: adding dummy value (UNIT) for shared variable %s\n", *j );
      st->add ( *j, new Value ( Value::UNIT ) );

    }

  }

  set_symtab ( st );

  // type check

  Environment * temp = NULL;

  std::list<TypeExpr *> * left_ng = new std::list<TypeExpr *>,
    * right_ng = new std::list<TypeExpr *>;

  //  PRINT_TYPEVAR_LIST ( left->get_non_generics() );
  //printf ( "left locals: \n" );
  //left->get_locals()->print();

  //PRINT_TYPEVAR_LIST ( right->get_non_generics() );
  //printf ( "right locals: \n" );
  //right->get_locals()->print();

  COPY_TYPEVAR_LIST ( left->get_non_generics(), left_ng );
  COPY_TYPEVAR_LIST ( right->get_non_generics(), right_ng );

  //PRINT_TYPEVAR_LIST ( left_ng );
  //PRINT_TYPEVAR_LIST ( right_ng );

  std::list<TypeExpr *>::iterator i;

  for ( j=shared->begin(); j!=shared->end(); j++ ) {

    TypeExpr * T_left = RETRIEVE ( left->get_locals(), *j );
    TypeExpr * T_right = RETRIEVE ( right->get_locals(), *j );

    if ( T_left == NULL ) {
      T_left = new TypeExpr();
      left_ng->push_back ( T_left ); // at the end: shouldn't affect subsequent copy_ex calls
    } else {
      T_left = T_left->copy_ex ( left->get_non_generics(), left_ng ); // in this case, T_left will have been allocated in COPY_TYPEVAR_LIST above
    }

    if ( T_right == NULL ) {
      T_right = new TypeExpr();
      right_ng->push_back ( T_right ); 
    } else {
      T_right = T_right->copy_ex ( right->get_non_generics(), right_ng ); // in this case, T_right will have been  allocated in COPY_TYPEVAR_LIST above
    }

    if ( ! TypeExpr::unify ( T_left, T_right ) ) {
      fprintf ( stderr, "Type Error: On line %d of %s: The shared variable %s has type \"", get_linenum(), get_filename(), *j ); 
      T_left->print(stderr); 
      fprintf ( stderr, "\"\n            in left program(%s) but type \"", left->get_name() ); T_right->print(stderr);
      fprintf ( stderr, "\"\n            in right program(%s)\n", right->get_name() );
      exit ( -1 );
    } // if one of these types is more specific than the other, then it will be set as an instance of the other 

    bool left_used = false;

    for ( i = right_ng->begin(); i != right_ng->end(); i++ )
      if ( (*i)->in_use ( T_left ) ) 
        left_used = true;

    if ( !left_used ) 
      delete T_left;
   
    temp = new Environment ( *j, T_right, temp );

  }

  for ( i = left_ng->begin(); i != left_ng->end(); i++ )
   *i = (*i)->prune(*i);

  for ( i = right_ng->begin(); i != right_ng->end(); i++ )
    *i = (*i)->prune(*i);

  for ( i = right_ng->begin(); i != right_ng->end(); i++ ) 
    left_ng->push_back ( *i ); // this may return an instance that points to T_left

  set_locals ( temp );
  set_non_generics ( left_ng ); // in some cases, right_ng will have T_left in it

  //printf ( "locals: " ); get_locals()->print();
  //PRINT_TYPEVAR_LIST ( get_non_generics() );

  delete right_ng; // Don't I need to delete members of this list first? No. The type exprs in right_ng
                   // are added to left_ng above

  // create new needs list
  for ( j = left->get_needs()->begin(); j != left->get_needs()->end(); j++ ) {
    TypeExpr * T = RETRIEVE ( right->get_locals(), *j );
    if ( T == NULL || ( T != NULL && !member_charlist ( shared, *j ) ) )
      add_needs ( *j );
  }

  for ( j = right->get_needs()->begin(); j != right->get_needs()->end(); j++ ) {
    TypeExpr * T = RETRIEVE ( left->get_locals(), *j );
    if ( T == NULL || ( T != NULL && !member_charlist ( shared, *j ) ) )
      add_needs ( *j );
  }

}

CompositeProgram::~CompositeProgram ( void ) {

  //DEC_INST ( "CompositeProgram" );

  delete left;
  delete right;

}

void CompositeProgram::print ( void ) {

  print_args();
  printf ( "composition of " );
  left->print();
  printf ( "\nand " );
  right->print();

}

void CompositeProgram::small_print ( void ) {

  small_print_base(); printf ( ", composition of\n" );

  INC_INDENT;
    left->small_print();
  INDENT; printf ( "and\n" );
    right->small_print();
  DEC_INDENT;

}

Program * CompositeProgram::copy ( void ) {

  std::list<TypeExpr *>::iterator i;

  std::list<char *> vlist; // uses a dummy shared list

  CompositeProgram * p = 
    new CompositeProgram ( left->copy(), right->copy(), 
			   &vlist, get_filename(), get_linenum() );

  p->set_name ( get_name() );
  p->set_base ( get_symtab(), get_args(), get_insts() );

  // copy type info
  std::list<TypeExpr *> * vars = new std::list<TypeExpr *>;

  ASSERT ( get_non_generics() != NULL );

  COPY_TYPEVAR_LIST ( get_non_generics(), vars );

  std::list<char *> * new_needs;
  COPY_STR_LIST ( get_needs(), new_needs );
  p->set_needs ( new_needs );
  
  for ( i = p->get_non_generics()->begin(); i != p->get_non_generics()->end(); i++ )
    delete (*i);
  p->set_non_generics ( vars );

  if ( get_locals() != NULL ) 
    p->set_locals ( get_locals()->copy_ex ( get_non_generics(), vars ) );

  if ( get_arg_types() != NULL )
    p->set_arg_types ( get_arg_types()->copy_ex ( get_non_generics(), vars ) );

  return p;

}

void CompositeProgram::compute_arg_types ( Environment * env ) {

  // the arg types of this program should be the same as the type in env
  // except with an "isomorphic" set of program variables
  
  Environment * e = env;
  std::list<TypeExpr *> * vars = new std::list<TypeExpr *>, * ng_save = new std::list<TypeExpr *>;
  
  while ( e != NULL ) {
    e->get_type()->compute_non_generics ( vars );
    e = e->get_tail();
  }
	
  COPY_TYPEVAR_LIST ( vars, ng_save );

  e = NULL;

  if ( env != NULL ) 
    e = env->copy_ex ( vars, ng_save );

  set_arg_types ( e );

  // tack on new non_generics (this is worrying me: some of the non-generics
  // in ng_save might really be the same as those in the child program's non_generics)

  while ( ! ng_save->empty() ) {
    get_non_generics()->push_back ( ng_save->front() );
    ng_save->pop_front();
  }

  delete vars;
  delete ng_save;

}

/////////////////////////////////////////////////////////////////////////////////////
//
//   init methods
//

void Program::init_base ( Scope * s ) {

  std::list<char *>::iterator i;
  std::list<Expr *>::iterator j;

  for ( i=args->begin(), j=insts->begin(); i!=args->end(), j!=insts->end(); i++, j++ ) {

    Expr * e = new Expr ( (*j)->eval(s) ), * f, * g;
    std::list<char *> U, V;
    std::list<char *>::iterator k;
    
    e->freevars ( &U );

    // make a copy since freevars does not, and we may want to delete e
    // before we're done processing freevars
    for ( k = U.begin(); k != U.end(); k++ )
      V.push_back ( strdup ( *k ) );

    f = e;

    for ( k = V.begin(); k != V.end(); k++ ) {

      g = new Expr ( s->get(*k)->copy() );
      f = e->subst ( g, *k );
      delete e;
      delete g;
      e = f;

    }

    param_symtab->add ( *i, f->eval(s) ); 
    delete f;

  }

}

void AtomicProgram::init_params ( Scope * s ) {

  init_base(s);

}

void InstProgram::init_params ( Scope * s ) {

  init_base ( s );
  s->push ( get_param_symtab() );
  child->init_params ( s );
  s->pop();

}

void CompositeProgram::init_params ( Scope * s ) {

  init_base(s);

  s->push ( get_param_symtab() );
  left->init_params ( s );
  right->init_params ( s );
  s->pop();

}

void AtomicProgram::init ( Scope * s ) {

  s->push ( get_param_symtab() );
  s->push ( get_symtab() );
  init_clause->execute(s);
  s->pop();
  s->pop();

}

void InstProgram::init ( Scope * s ) {

  s->push ( get_symtab() );
  child->init ( s );
  s->pop();

}

void CompositeProgram::init ( Scope * s ) {

  s->push ( get_symtab() );
  left->init ( s );
  right->init ( s );
  s->pop();

}

/////////////////////////////////////////////////////////////////////////////////////
//   execute methods
//
//

void CompositeProgram::step ( Scope * s ) {

  s->push(get_symtab());
  left->step(s);
  right->step(s);
  s->pop();

}

void InstProgram::step ( Scope * s ) {

  s->push(get_symtab());
  child->step(s);
  s->pop();

}

void AtomicProgram::step ( Scope * s ) {

  s->push ( get_param_symtab() );
  s->push ( get_symtab() );

  std::list<Clause *>::iterator i;

  for ( i=clauses->begin(); i!=clauses->end(); i++ ) {
    (*i)->execute(s);
  }

  s->pop();
  s->pop();

}

void AtomicProgram::random_step ( Scope * s, float p ) {

  s->push (get_param_symtab());
  s->push (get_symtab());

  std::list<Clause *>::iterator i;

  float p_per_clause = p / clauses->size();

  for ( i=clauses->begin(); i!=clauses->end(); i++ ) {
    if ( 0.0001*(rand()%10000) < p )
      (*i)->execute(s);
  }

  s->pop();
  s->pop();

}

/////////////////////////////////////////////////////////////////////////////////////
//   make clause list methods
//
//

void CompositeProgram::make_clause_list ( CLAUSE_LIST * L, Scope * s ) {

  s->push(get_symtab());
  left->make_clause_list ( L, s );
  right->make_clause_list ( L, s );
  s->pop();

}

void InstProgram::make_clause_list ( CLAUSE_LIST * L, Scope * s ) {

  s->push(get_symtab());
  child->make_clause_list(L,s);
  s->pop();

}

void AtomicProgram::make_clause_list ( CLAUSE_LIST * L, Scope * s ) {

  s->push ( get_param_symtab() );
  s->push ( get_symtab() );

  std::list<Clause *>::iterator i;

  for ( i=clauses->begin(); i!=clauses->end(); i++ ) 
    L->push_back ( new CLAUSE_LIST_EL ( *i, s->to_list() ) );
   
  s->pop();
  s->pop();

}

///////////////////////////////////////////////////////////////////////////////////////
//
// n-fold Composition
//

std::list<Expr *> * subst_in_list ( std::list<Expr *> * L, char * b, Value * v ) {

  std::list<Expr *>::iterator i;
  std::list<Expr *> * M = new std::list<Expr *>;
  Expr * e = new Expr ( v->copy() );

  for ( i=L->begin(); i != L->end(); i++ ) {
    Expr * f = (*i)->subst ( e, b );
    //    printf ( "substituted " ); e->print(); printf ( " in for %s in ", b ); (*i)->print();
    //    printf ( " to get " ); f->print(); NL;
    M->push_back ( (*i)->subst ( e, b ) );
  }

  delete e;
  return M;

}

Program * Program::Composition ( Program * p, std::list<Expr *> * exprlist, 
                                 char * binder, std::list<Value *> * vals, std::list<char *> * sharelist,
                                 Scope * scope, Environment * env, 
                                 char * filename, int linenum ) {

  std::list<Value *>::iterator i;
  ASSERT ( vals->size() > 0 );
  i = vals->begin();

  std::list<Expr *> * new_exprlist = subst_in_list ( exprlist, binder, *i );
  Program * r, * q = p->copy();
  q->set_insts ( new_exprlist, scope, env, "<<composed program>>" );
  q = new InstProgram ( q, filename, linenum );

  i++;

  while ( i != vals->end() ) {

    new_exprlist = subst_in_list ( exprlist, binder, *i );
    r = p->copy();
    r->set_insts ( new_exprlist, scope, env, "<<composed program>>" );
    q = new CompositeProgram ( q, new InstProgram ( r, filename, linenum ),
                               sharelist, filename, linenum );

    i++;

  }

  return q;

}

void Program::check_needs ( void ) {

    std::list<char *> * n = get_needs();

    if ( n->size() > 0 ) {

      fprintf ( stderr, "Error: program 'main' needs variable(s) " );
      std::list<char *>::iterator i;
      for ( i = n->begin(); i != n->end(); i++ )
	fprintf ( stderr, "%s ", *i );
      fprintf ( stderr, "\n" );
      exit ( -1 );

    } 

}
