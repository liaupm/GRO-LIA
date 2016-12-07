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

#ifndef _EXPR_HH
#define _EXPR_HH

#include <stdio.h>
#include <stdlib.h>


#include <string>
#include <sstream>
//#include "/usr/include/c++/4.2.1/sstream"


#include "SymbolTable.h"
#include "Type.h"
#include "Program.h"

int num_exprs ( void );

class Expr {

public:

  typedef enum { 
    CONSTANT,                  // constants
    VARIABLE,                  // variable references
    APPLICATION,               // f a
    EX_APPLICATION,            // f (a1,...,an)
    UNARY,                     // - !
    BINARY,                    // + * % / . ^ @ = == < > <= >=
    LET,                       // let x=0 in ...
    IF,                        // if x==0 then y else z
    RECORD,                    // [ x := e1, y := e2, ... ]
    PROGRAM,                   // for those odd instances where you might want to return a program
    THISS,                      // for reproduction expressions to refer to the expression itself
    RANDOM                     // returns a random number between -1 and 1, uniformly distributed
  } TYPE;

  typedef enum {

    // booleans
    AND, OR, NOT, 

    // integers 
    MOD,

    // reals
    POW,

    // integers and reals
    PLUS, MINUS, MULT, DIV, UMIN, LT, GT, LE, GE,

    // strings
    STR_CAT,

    // lists
    HEAD, TAIL, CONS, CONCAT, LIST_EL,

    // everything
    EQ, NE,

    // records
    DOT,

    // previous value
    PREV,

    // record add
    RECORD_ADD

  } OPERATOR;

  typedef struct { 
    char * name;
    Expr * e;
  } FIELD;

  Expr ( void ); // records
  Expr ( Value * v ); // constants
  Expr ( char * str ); // variables 
  Expr ( Expr * f, Expr * a ); // function applications
  Expr ( OPERATOR p, Expr * e ); // unary expressions
  Expr ( OPERATOR p, Expr * a, Expr * b ); // binary expressions
  Expr ( const char * str, Expr * v, Expr * e ); // let expressions
  Expr ( Expr * a, Expr * b, Expr * c ); // if then else expressions
  Expr ( Value * f, std::list<Expr *> * args ); // external function applications

  Expr ( Program * p ); // for programs

  // reproduction constructors
  Expr ( TYPE t ); // for making an expression by just setting the type (good with THISS and RANDOM)

  ~Expr ( void );

  Expr * copy ( void );

  void print ( FILE * fp );
  inline void print ( void ) { print ( stdout ); }
  inline void fprint ( void ) { print ( stderr ); }

  std::string tostring ( void );
  void tostring_aux ( std::stringstream * strm );

  Value * eval ( Scope * s );
  Value * eval_op ( Scope * s );
  Value * full_type ( Scope * s );
  void infer_variable_types ( Value * context, Scope * s );
  void infer_variable_types ( Scope * s );
  Value * get_lhs_value ( Scope * s );

  // Auilliary eval methods for lists
  Value * list_head ( Scope * s );
  Value * list_tail ( Scope * s );
  Value * list_cons ( Scope * s );
  Value * list_join ( Scope * s );
  static Value * list_equal ( Value * v1, Value * v2, Scope * s );

  // Auxilliary eval methods for functions
  void freevars ( std::list<char *> * fr );
  Expr * subst ( Expr * e, char * x );
  static char * new_var_name ( const char * v );
  inline Value * get_val ( void ) { return val; }

  // for records
  void addField ( const char * str, Expr * e );

  inline TYPE get_type ( void ) { return type; }
  inline OPERATOR get_op ( void ) { return op; }
  inline char * get_name ( void ) { return name; }
  inline Expr * get_left ( void ) { return e1; }
  inline Expr * get_right ( void ) { return e2; }

  // for previous value
  Expr * prevify ( std::list<char *> * locals );

  // type checking
  TypeExpr * compute_type ( Scope * s, Environment * env, 
			    std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage );
  TypeExpr * compute_type_aux ( Scope * s, Environment * env, 
				std::list<TypeExpr *> * non_generics, std::list<TypeExpr *> * garbage );
  Value::TYPE op_type ( OPERATOR op );
  Value::TYPE op_arg_type ( OPERATOR op );
  bool has_type ( Value::TYPE t, Scope * s, Environment * env );
  bool type_check ( Scope * s, Environment * env );
  inline void set_line_info ( int n, char * str ) { line = n; file = str; }
  inline int line_num ( void ) { return line; }
  inline char * file_name ( void ) { return file; }
  bool is_lhs ( void );


private:

  TYPE type;
  OPERATOR op;                // operator 
  Expr * e1, * e2, * e3;      // for arguments of operators, functions, and if-then-elses, function arguments
  Value * val;                // for constants and let rhs
  std::list<Expr *> * arglist;     // for external function calls
  char * name;                // for variable names, let lhs and record access
  std::list<FIELD *> * fields;     // fields
  Program * program;         // programs

  // for type check errors
  int line;
  char * file;

};

// Some MACROs to help with Expr Construction

#define APP(_e1_,_e2_) new Expr ( _e1_, _e2_ )
#define FUNC(_var_,_body_) new Expr ( new Value ( _var_, _body_ ) )
#define UNEXP(_op_,_e1_) new Expr ( _op_, _e1_ )
#define BINEXP(_op_,_e1_,_e2_) new Expr ( _op_, _e1_, _e2_ )
#define CONST(_c_) new Expr ( new Value ( _c_ ) )
#define VAR(_v_) new Expr ( _v_ )
#define IFEXP(_e1_,_e2_,_e3_) new Expr ( _e1_, _e2_, _e3_ )
#define LETEXP(_x_,_e1_,_e2_) new Expr ( _x_, _e1_, _e2_ )
#define EAPP(_f_,_args_) new Expr ( _f_, _args_ )
#define RECEXP new Expr()

#define BOOL_OP(__op__) \
    ( __op__ == AND    || __op__ == OR || __op__ == NOT ) 

#define COMP_OP(__op__) \
    ( __op__ == EQ  || __op__ == NE ||  \
      __op__ == LT  || __op__ == GT || __op__ == LE    || __op__ == GE  )

#define INT_OP(__op__) ( __op__ == MOD )

#define NUM_OP(__op__) \
    ( __op__ == PLUS  || __op__ == MINUS  || __op__ == MULT   || __op__ == DIV ||  \
      __op__ == UMIN  || __op__ == POW )

#define STRING_OP(__op__) \
    ( __op__ == STR_CAT )

#define LIST_OP(__op__) \
    ( __op__ == HEAD     || __op__ == TAIL    || __op__ == CONS        || __op__ == CONCAT  ||  \
      __op__ == LIST_EL )

#define RECORD_OP(__op__) ( __op__ == DOT )

#define COPY_EXPR_LIST(_from_,_to_) {                       \
  std::list<Expr *>::iterator i;                                 \
  for ( i=(_from_)->begin(); i != (_from_)->end(); i++ )    \
    (_to_)->push_back ( (*i)->copy() );                     \
}

#define FREE_EXPR_LIST(_elist_) {                           \
  std::list<Expr *>::iterator i;                                 \
  for ( i=(_elist_)->begin(); i != (_elist_)->end(); i++ )  \
    delete (*i);                                            \
  delete _elist_;                                           \
}

#endif


