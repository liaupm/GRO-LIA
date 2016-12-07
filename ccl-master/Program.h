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

#ifndef _PROGRAM_HH
#define _PROGRAM_HH

#include <list>

//#ifdef WIN
#ifdef _WIN32
    #include <string.h>
#elif __linux__
    #include <string.h>
#endif

#include "SymbolTable.h"
#include "Expr.h"

#define NEW_SYMTAB_SIZE 50
#define NEW_PARAM_SYMTAB_SIZE 5

class Command {

public:

  Command ( Expr * l, Expr * r );
  ~Command ( void );
  void apply ( Scope * s );
  void debug_apply ( Scope * s );
  void print ( void );
  inline Expr * get_lhs ( void ) { return lhs; }
  inline Expr * get_rhs ( void ) { return rhs; }
  Command * copy ( void );
  Command * reproduce ( void );
  Command * subst ( Expr * e, char * x ); // C[e/x]
  bool is_assigned ( const char * var );

private:

  Expr * lhs;
  Expr * rhs;

};

class Clause {

public:

  Clause ( Expr * g );
  Clause ( Expr * g, std::list<Command *> * coms );
  ~Clause ( void );

  void addCommand ( Command * c );
  void execute ( Scope * s );
  void debug_execute ( Scope * s );
  void print ( void );
  void print_as_init ( void );
  Clause * copy ( void );
  Clause * reproduce ( void );
  Clause * subst ( Expr * e, char * x ); // C[e/x]
  bool is_assigned ( const char * var );
  inline Expr * get_guard ( void ) { return guard; }
  inline std::list<Command *> * get_commands ( void ) { return commands; }

  static void get_debug_command ( Scope * s );

private:

  Expr * guard;
  std::list<Command *> * commands;
  std::list<Command *>::iterator i;

};

typedef std::pair < Clause *, std::list<SymbolTable *> * > CLAUSE_LIST_EL;
typedef std::list<CLAUSE_LIST_EL *> CLAUSE_LIST;

class Program {

public:

  Program ( char * file, int line );
  Program ( bool alloc, char * file, int line );
  virtual ~Program ( void );

  inline void set_symtab ( SymbolTable * s ) { 
    symtab = s; 
  }
  inline SymbolTable * get_symtab ( void ) { return symtab; }
  inline SymbolTable * get_param_symtab ( void ) { return param_symtab; }
  void set_args ( std::list<char *> * a );
  inline std::list<char *> * get_args ( void ) { return args; }
  void set_insts_simp ( std::list<Expr *> * in );
  void set_insts ( std::list<Expr *> * in, Scope * s, Environment * env, char * name );
  inline std::list<Expr *> * get_insts ( void ) { return insts; }
  inline void set_parent ( Program * p ) { parent = p; }

  inline void set_non_generics ( std::list<TypeExpr *> * ng ) { 
    if ( non_generics != NULL ) {
      //  std::list<TypeExpr *>::iterator w;
      //for ( w=non_generics->begin(); w != non_generics->end(); w++ ) {
      //  printf ( "%x::set_non_generics: deleting %x\n", this, *w );
      //  delete (*w);
      //}
      delete non_generics; 
    }
    non_generics = ng; 
  }

  inline std::list<TypeExpr *> * get_non_generics ( void ) { return non_generics; }
  inline void set_locals ( Environment * env ) { locals = env; }
  inline void set_needs ( std::list<char *> * L ) { FREE_STR_LIST ( needs ); needs = L; }
  inline std::list<char *> * get_needs ( void ) { return needs; }
  inline void add_needs ( const char * v ) { needs->push_back ( strdup ( v ) ); }
  inline void set_arg_types ( Environment * env ) { arg_types = env; }
  inline Environment * get_locals ( void ) { return locals; }
  inline Environment * get_arg_types ( void ) { return arg_types; }

  void copy_param_symtab ( SymbolTable * ps );

  void print_args ( void );
  void small_print_base ( void );
  virtual void print ( void ) {}
  virtual void small_print ( void ) {}
  void set_base ( SymbolTable * s, std::list<char *> * alist, std::list<Expr *> * elist );
  virtual Program * copy ( void ) { return NULL; }
  void add_symbol ( const char * str, Value * v );

  void init_base ( Scope * );
  virtual void init ( Scope * ) {}
  virtual void init_params ( Scope * ) {}
  virtual void step ( Scope * ) {}
  virtual void random_step ( Scope *, float ) {}
  virtual void compute_arg_types ( Environment * ) {}
  virtual void make_clause_list ( CLAUSE_LIST *, Scope * ) {}

  void check_needs ( void );

  inline char * get_filename ( void ) { return filename; }
  inline char * get_name ( void ) { return name; }
  inline void set_name ( const char * n ) { 
    free ( name );
    name = strdup ( n );
  }
  inline int get_linenum ( void ) { return linenum; }

  inline std::list<TypeExpr *> * get_garbage() { return garbage; }

  static Program * Composition ( Program * p, std::list<Expr *> * exprlist, 
                                 char * binder, std::list<Value *> * vals, std::list<char *> * sharelist,
                                 Scope * scope, Environment * env, 
                                 char * filename, int linenum );

private:

  SymbolTable * symtab, * param_symtab;
  std::list<char *> * args, * needs;
  std::list<Expr *> * insts;
  Program * parent;
  int id;
  std::list<TypeExpr *> * non_generics, * garbage;
  Environment * locals;
  Environment * arg_types;
  char * filename;
  int linenum;
  char * name;

};

class AtomicProgram : public Program {

public:

  AtomicProgram ( char * file, int line );
  ~AtomicProgram ( void );

  void add_clause ( Clause * c );
  inline void set_init ( Clause * c ) { init_clause = c; }
  inline Clause * get_init ( void ) { return init_clause; }
  inline std::list<Clause *> * get_clauses ( void ) { return clauses; }
  void print ( void );
  Program * copy ( void );
  Program * reproduce ( void );
  void small_print ( void );
  void init ( Scope * s );
  void init_params ( Scope * s );
  void step ( Scope * s );
  void random_step ( Scope * s, float p );
  void compute_arg_types ( Environment * ) {}
  void make_clause_list ( CLAUSE_LIST * L, Scope * s );

private:

  std::list<Clause *> * clauses;
  Clause * init_clause;
  
};

class InstProgram : public Program {

public:

  InstProgram ( Program * c, char * file, int line );
  ~InstProgram ( void );
  void print ( void );
  Program * copy ( void );
  Program * reproduce ( void );
  void small_print ( void );
  void init ( Scope * s );
  void init_params ( Scope * s );
  void step ( Scope * s );
  void compute_arg_types ( Environment * env );
  void make_clause_list ( CLAUSE_LIST * L, Scope * s );

private:

  Program * child;

};

class CompositeProgram : public Program {

public:

  CompositeProgram ( Program * l, Program * r, std::list<char *> * shared, char * file, int line );
  ~CompositeProgram ( void );
  void print ( void );
  Program * copy ( void );
  Program * reproduce ( void );
  void small_print ( void );
  void init ( Scope * s );
  void init_params ( Scope * s );
  void step ( Scope * s );
  void compute_arg_types ( Environment * env );
  void make_clause_list ( CLAUSE_LIST * L, Scope * s );

private:

  Program * left, * right;

};

#endif
