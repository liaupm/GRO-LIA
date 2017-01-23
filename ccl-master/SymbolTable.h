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

#ifndef _SYMBOLTABLE_HH
#define _SYMBOLTABLE_HH

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <sstream>
//#include "/usr/include/c++/4.2.1/sstream"

class Value;
class SymbolTable;
class Expr;
class Scope;
class Program;
class TypeExpr;
class Environment;

typedef Value * (* EXTERNAL_CCLI_FUNCTION) ( std::list<Value *> *, Scope * );

class Value { // also used to represent types

public:

  typedef enum { UNIT, BOOLEAN, INTEGER, REAL, STRING, LIST,
                 RECORD, FUNCTION, EX_FUNCTION, PROGRAM } TYPE;

  // constructors (one for each type)
  Value ( TYPE t ); 
  Value ( int n );
  Value ( double r );
  Value ( bool b );
  Value ( std::list<Value *> * L );  // for std::lists
  Value ( void ); // for the empty std::list
  Value ( char * a, Expr * e ); // for functions
  Value ( EXTERNAL_CCLI_FUNCTION f, TypeExpr * T, std::list<TypeExpr *> * args ); // for external f
  Value ( const char * str ); // for strings
  Value ( Program * p ); // for programs
  ~Value ( void );
  void free_data ( void );

  Value * copy ( void );  
  void print ( FILE * fp ); 
  inline void print ( void ) { print ( stdout ); }
  void print_to_string ( std::string * str );
  std::string tostring ( void );
  void tostring_aux ( std::stringstream * strm );
  static Value * string_to_value ( const char * str );
  inline void fprint ( void ) { print ( stderr ); }
  inline TYPE get_type ( void ) { return type; }

  // setting base values (one for each basic type)
  void set ( bool b );
  void set ( int n );
  void set ( double r );
  void set ( const char * str );
  
  // this resets everything
  void reset ( Value * v );  

  // accessors (one for each basic type)
  bool bool_value ( void );
  int int_value ( void );
  double real_value ( void );
  std::string string_value ( void );
  double num_value ( void );
  Program * program_value ( void );

  // for std::lists
  void set ( std::list<Value*> * L );
  std::list<Value*> * list_value ( void );

  // for records
  void addField ( const char * name,  Value * v );
  Value * getField ( const char * name );
  void set_fields ( SymbolTable * f );
  inline SymbolTable * get_fields ( void ) { return data.fields; }

  // for functions
  Expr * func_body ( void );
  char * func_arg_name ( void );
  void set_ret_type ( Value * T );

  // for external functions
  inline EXTERNAL_CCLI_FUNCTION efunc ( void ) { return data.efunc_val.f; }
  inline TypeExpr * return_type ( void ) { return data.efunc_val.return_type; }
  inline std::list<TypeExpr *> * arg_types ( void ) { return data.efunc_val.arg_types; }
  inline std::list<TypeExpr *> * non_generics ( void ) { return data.efunc_val.non_generics; }

  // type checking
  TypeExpr * compute_type ( Scope * s, Environment * env, 
			    std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage );

  // checking equality
  static bool equal ( Value * v1, Value * v2, Scope * s );

private:

  TYPE type;

  union {

    bool bool_val;
    int int_val;
    double real_val;
    std::string * str_val;

    SymbolTable * fields;

    struct { 
      Expr * function_body;
      char * arg_name;
    } func_val;

    struct {
      EXTERNAL_CCLI_FUNCTION f;
      TypeExpr * return_type;
      std::list<TypeExpr *> * arg_types,
	* non_generics;
    } efunc_val;

    struct {
      std::list <Value*> * vals; // this needs to be instantiated
                            // when the Value is used as a std::list
    } list_val;

    Program * program_val;

  } data;

};

Value * singleton ( Value * v, Value * t );

class Symbol {

public:

  Symbol ( const char * str, Value * val );
  Symbol ( const char * str, Program * prog );
  ~Symbol ( void );
  inline char * get_name ( void ) { return name; }
  inline Value * get_value ( void ) { return v; }
  void clear ( void );
  inline Value * get_previous ( void ) { return previous; }
  inline Program * get_prog_value ( void ) { return p; }
  inline void set_recursive ( void ) { rec_flag = true; }
  inline bool is_recursive ( void ) { return rec_flag; }
  inline bool is_program ( void ) { return ( p != NULL ); }

  friend class SymbolTable;
  friend class Scope;

private:

  char * name;
  Value * v, * previous;
  Program * p;
  Symbol * next;
  bool rec_flag; // whether the symbol stands for a recursive function
  int rec_count; 

};

class SymbolTable {

public:

  SymbolTable ( int n );
  ~SymbolTable ( void );

  int hash ( const char * str );
  void add ( const char * name, Value * v );
  void add ( const char * name, Program * p );
  void del ( const char * name );
  void replace ( SymbolTable * s );
  Value * get ( const char * name );
  Value * get_previous ( const char * name );
  Symbol * get_symbol ( const char * name );
  Program * get_program ( const char * name );
  void print ( FILE * fp );
  inline void print ( void ) { print ( stdout ); }
  void print_as_record ( FILE * fp );
  void print_as_record_to_string ( std::string * str );
  std::list<std::string> field_names_as_string_list ( void );
  void record_tostring_aux ( std::stringstream * strm );
  void print_as_record_type ( void );
  SymbolTable * copy ( void );
  bool assign ( const char * name, Value * v );
  void print_names ( FILE * fp, int indent );
  Environment * to_env ( Scope * s, Environment * env, 
			 std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage );
  friend class Scope;
  friend class Clause;

  void divide ( SymbolTable * dest, float frac );

private:

  int num_buckets, num_symbols;
  Symbol ** buckets;
  SymbolTable * next;

};

class Scope {

public:

  Scope ( void ) : head(NULL) {}
  ~Scope ( void );
  void push ( SymbolTable * s );
  void push_back ( SymbolTable * s );
  inline SymbolTable * get_top ( void ) { return head; }
  void pop ( void );
  void pop_all ( void );
  void add ( const char * str, Value * v );
  void add ( const char * str, Program * p );
  Value * get ( const char * name );
  Value * get_previous ( const char * name );
  Program * get_program ( const char * name );
  void print ( FILE * fp );
  inline void print ( void ) { print ( stdout ); }
  bool assign ( const char * name, Value * v );
  void set ( Expr * lhs, Value * v );
  Scope * copy ( void );
  std::list<SymbolTable *> * to_list ( void );
  void print_names ( FILE * fp );
  void add_by_symbols ( SymbolTable * sym );
  void set_recursive ( const char * name );
  bool is_recursive ( const char * name );

private:

  SymbolTable * head;

};

#define ASSERT(_cond_) {                                                          \
  if ( ! ( _cond_ ) ) {                                                           \
    fprintf (stderr, "Internal error: assertion '%s' failed at %s, line %d\n",    \
                   #_cond_ , __FILE__, __LINE__);                                 \
    exit ( -1 );                                                                  \
  }                                                                               \
}

#define ASSERT_MSG(_cond_,_msg_...) {                                             \
  if ( ! ( _cond_ ) ) {                                                           \
    fprintf ( stderr, "Internal error: assertion '%s' failed at %s, line %d\n",   \
                   #_cond_ , __FILE__, __LINE__);                                 \
    fprintf ( stderr, _msg_ );                                                    \
    exit ( -1 );                                                                  \
  }                                                                               \
}

#define COPY_STR_LIST(_from_,_to_) {                        \
  _to_ = new std::list<char *>;                             \
  std::list<char *>::iterator i;                            \
  for ( i=(_from_)->begin(); i != (_from_)->end(); i++ )    \
    (_to_)->push_back ( strdup(*i) );                       \
}

#define FREE_STR_LIST(_list_) {                           \
  std::list<char *>::iterator i;                          \
  for ( i=(_list_)->begin(); i != (_list_)->end(); i++ )  \
    free (*i);                                            \
  delete _list_;                                          \
}

#define PRINT_STR_LIST(_list_) {                          \
  std::list<char *>::iterator i;                          \
  for ( i=(_list_)->begin(); i != (_list_)->end(); i++ )  \
    printf ( "%s ", *i );                                 \
  printf ( "\n" );                                        \
}

bool member_charlist ( std::list<char *> * L, const char * str );

#define RUNTIME_ERROR(__file__,__line__,__msg__...) {                   \
                                                                        \
    std::stringstream strm;                                             \
                                                                        \
     strm << "Runtime error in "                                        \
          << ( __file__ != NULL ? __file__ : "no file information" )    \
          << " on line " << __line__                                    \
          << ".<br><blockquote>" << __msg__ << "</blockquote>";         \
                                                                        \
     throw strm.str();                                                  \
                                                                        \
}

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
    fflush ( stdout );                                    \
}

#ifdef MEMCHECK
#define INC_INST(_classname_) printf ( "new %s at %x\n", _classname_, this )
#define DEC_INST(_classname_) printf ( "  deleting a %s at %x\n", _classname_, this )
#else
#define INC_INST(_classname_)
#define DEC_INST(_classname_)
#endif

#endif
