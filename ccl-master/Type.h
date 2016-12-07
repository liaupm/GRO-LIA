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

#ifndef _TYPE_HH
#define _TYPE_HH

#include <list>
#include "SymbolTable.h"

class TypeExpr {

public:

  typedef enum { VARIABLE, ATOMIC, LIST, FUNCTION, RECORD, VARARGS } TYPE;

  TypeExpr ( void );                         // variable types
  TypeExpr ( Value::TYPE t );                // atomic
  TypeExpr ( TypeExpr * t );                 // lists
  TypeExpr ( TypeExpr * t1, TypeExpr * t2 ); // functions
  TypeExpr ( bool b );                       // varargs if true, inconsistent type if false
  TypeExpr ( Environment * f );              // for records

  ~TypeExpr ( void );
  void free_children ( void );
  void free_children ( std::list<TypeExpr *> * ng );

  // accessors
  Value::TYPE get_atomic_type ( void );
  TypeExpr * get_list_base ( void );
  TypeExpr * get_domain ( void );
  TypeExpr * get_codomain ( void );
  Environment * get_fields ( void );
  TypeExpr * get_instance ( void ) { return instance; }
  TYPE get_type ( void );
  inline bool is_extensible ( void ) { return extensible; }
  Value * default_value ( void );

  // checking consistency
  bool is_consistent ( void ) { return consistent; }

  bool in_use ( TypeExpr * t );

  // setting
  void set_instance ( TypeExpr * t );
  void add_field ( char * name, TypeExpr * T );
  inline void set_extensibility ( bool val ) { extensible = val; }
  void reset ( void );

  // printing
  void print ( FILE * fp );
  inline void print ( void ) { print ( stdout ); }
  void print_aux ( FILE * fp );
  void clear_var_names ( void );
  std::string tostring ( void );
  void tostring_aux ( std::stringstream * strm );

  // unification
  static TypeExpr * prune ( TypeExpr * T );
  static bool occurs_in ( TypeExpr * v, TypeExpr * T );
  static bool unify ( TypeExpr * T1, TypeExpr * T2 );

  // clean up
  static void take_out ( std::list<TypeExpr *> * garbage );
  static void take_out ( std::list<TypeExpr *> * garbage,  std::list<TypeExpr *> * exceptions );

  // copy w.r.t non-generics
  TypeExpr * copy ( std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage );
  TypeExpr * copy_ex ( std::list<TypeExpr *> * old_vars, std::list<TypeExpr *> * new_vars );
  static bool member ( TypeExpr * t, std::list<TypeExpr *> * L );
  void compute_non_generics ( std::list<TypeExpr *> * non_generics );

private:

  TYPE type;

  Value::TYPE atomic;   // constants ( not including lambda expressions )

  TypeExpr * list_base; // lists

  TypeExpr * domain,    // functions
           * codomain;

  Environment * fields; // for records
  bool extensible;

  TypeExpr * instance;  // for variables

  int var_name;         // for printing

  friend class Environment;

  bool consistent;

};

class Environment {

public:

  Environment ( char * str, TypeExpr * t, Environment * e );
  ~Environment ( void );
  TypeExpr * retrieve ( char * str );
  inline char * get_name ( void ) { return name; }
  inline TypeExpr * get_type ( void ) { return T; }
  inline Environment * get_tail ( void ) { return tail; }
  void reset ( void ); // sets all variable instance fields to NULL
  void print ( void );
  Environment * copy_ex ( std::list<TypeExpr *> * old_vars, std::list<TypeExpr *> * new_vars );
  void delete_rest ( std::list<TypeExpr *> * exceptions );
  void set_freed ( bool v ) { freed = v; }
  bool is_freed ( void ) { return freed; }

private:

  char * name;
  TypeExpr * T;
  Environment * tail;
  bool freed;

  friend class TypeExpr;

};

#define RETRIEVE(_env_,_str_) ( _env_ == NULL ? NULL : _env_->retrieve ( _str_ ) )

#define RESET(_env_) if ( _env_ != NULL ) _env_->reset()

#define COPY_TYPEVAR_LIST(_from_,_to_) {                    \
  std::list<TypeExpr *>::iterator i;                        \
  for ( i=(_from_)->begin(); i != (_from_)->end(); i++ )    \
    (_to_)->push_back ( new TypeExpr() );                   \
}

#define PRINT_TYPEVAR_LIST(_tvl_) {                      \
  std::list<TypeExpr *>::iterator i;                      \
  printf ( "%s at %x contains: \n", #_tvl_, _tvl_ );      \
  for ( i=(_tvl_)->begin(); i != (_tvl_)->end(); i++ )    \
    (*i)->print(); printf ( "\n" );			  \
}

#endif
