%{

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

#include <unistd.h>
//#include <dlfcn.h>
#include <stdio.h>
#include <sstream>
//#include "/usr/include/c++/4.2.1/sstream"
#include <string.h>
#include <list>
#include <string>
#include "Expr.h"
#include "Program.h"
#include "parser.h"
#include "Type.h"

#define NL printf ( "\n" )

// for g++ compatibility 
extern int yyparse ( void );
extern int yylex ( void );
extern int yyerror ( char * s );
extern void yyrestart ( FILE * new_file );

extern int lineNum[MAX_INCLUDE_DEPTH+1];
extern int include_stack_ptr;
extern char * current_file_name[MAX_INCLUDE_DEPTH+1];

// for lists
Value int_list_el = Value ( 0 );
Value empty ( &int_list_el );

#define EMPTY new Expr ( new Value ( new std::list<Value *> ) )
#define UC { printf ( "under construction\n" ); $$ = NULL; }

// for lambdas and recursions
Environment * main_env = NULL;

#define PUSH(__str__) {                                               \
    main_env = new Environment ( __str__, new TypeExpr(), main_env );  \
} 

#define PUSH_WITH_TYPE(__str__,__type__) {                            \
  main_env = new Environment ( __str__, __type__, main_env );         \
} 

#define POP {                                                         \
  if ( !main_env->is_freed() ) {                                      \
    delete main_env->get_type();                                      \
    main_env->set_freed ( true );				      \
  } \
  Environment * temp = main_env;                                      \
  main_env = main_env->get_tail();                                    \
  delete temp;                                                        \
}

#define POP_SIMP {                                                    \
  Environment * temp = main_env;                                      \
  main_env = main_env->get_tail();                                    \
  delete temp;                                                        \
}

#define ERROR(__msg__...) {                                             \
                                                                        \
    std::stringstream strm;                                             \
                                                                        \
     strm << "gro encountered an error in "                             \
       << current_file_name[include_stack_ptr]                          \
       << " on line " << lineNum[include_stack_ptr]                     \
       << ".<blockquote>" << __msg__ << "</blockquote>";                \
                                                                        \
     throw strm.str();                                                  \
                                                                        \
}

#define TYPE_ERROR(__msg__...) {                                        \
                                                                        \
    std::stringstream strm;                                             \
                                                                        \
     strm << "Type error in "                                           \
       << current_file_name[include_stack_ptr]                          \
       << " on line " << lineNum[include_stack_ptr]                     \
       << ".<blockquote>" << __msg__ << "</blockquote>";                \
                                                                        \
     throw strm.str();                                                  \
                                                                        \
}

#define TYPE_ERROR1(__e__,__t__,__msg__...) {                                         \
                                                                                      \
    std::stringstream strm;                                                           \
                                                                                      \
    strm << "Type error in "                                                          \
         << current_file_name[include_stack_ptr]              \
         << " on line " << lineNum[include_stack_ptr]                                                \
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
         <<  current_file_name[include_stack_ptr]              \
         << " on line " << lineNum[include_stack_ptr]                                                \
         << "<br><blockquote>" << __msg__ << " In particular, "                       \
         << "<i>" << __e1__->tostring() << "</i> has type <i>" << __t1__->tostring() << "</i>, while " \
         << "<i>" << __e2__->tostring() << "</i> has type <i>" << __t2__->tostring() << ".</i>"        \
         << "</blockquote>";                                                          \
                                                                                      \
         throw strm.str();                                                            \
                                                                                      \
}

#define SET_INFO(__e__) { __e__->set_line_info ( lineNum[include_stack_ptr], current_file_name[include_stack_ptr] ); }

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
    fflush ( stdout );                                    \
}

#define VERBOOSE printf ( ">> line %d of %s\n", lineNum[include_stack_ptr], current_file_name[include_stack_ptr] );
#define VERBOSE

// main scope 
Scope * top_scope;

// for computing types of locals in programs
std::list<TypeExpr *> program_garbage;

TypeExpr * fun_temp;

// auxilliary flags for external fuction type construction
bool rec_extensible_flag, rec_ret_ex_flag;

// list of internally defined ccl functions
std::list<InternalFunction *> ccl_function_list;

 int foreach_index = 0;

%}

%union {

  Expr * exp;
  char * str;        /* for variable names */
  int i;
  double real;
  std::list<Expr *> * elist;
  std::list<Expr::FIELD *> * field_list;
  std::list<char *> * vlist;
  Command * command;
  std::list<Command *> * command_list;
  Clause * cls;
  Statement * st;
  std::list<Statement *> * st_list;
  Program * prog;  
  TypeExpr * type;
  std::list<TypeExpr *> * type_list;
  Environment * env;

}

%token t_SEMICOLON
%token <i> t_INTEGER
%token <real> t_REAL
%token t_EQUALS
%token t_LCURLY
%token t_RCURLY
%token t_COMMA

%token t_TRUE
%token t_FALSE
%token t_NOT
%token t_LBBRACKET
%token t_RBBRACKET
%token t_LBRACKET
%token t_RBRACKET
%token t_HEAD
%token t_TAIL
%token t_CONCAT
%token t_CONS
%token t_DOT
%token t_ASSIGN
%token t_LAMBDA
%token t_COLON
%token t_STRCAT

%token t_COMPOSE
%token t_NEEDS

%token <str> t_VAR
%token <str> t_STRING

%token t_OR t_AND
%token t_LESS_THAN t_LESS_THAN_OR_EQUAL t_GREATER_THAN t_GREATER_THAN_OR_EQUAL
%token t_EQUAL t_NOT_EQUAL
%token t_MINUS t_MOD 
%token t_DIV 
%token t_POWER
%token t_RECORD_ADD

%token t_PLUS
%token t_MULT
%nonassoc t_LAMBDA
%token t_LPAREN
%token t_RPAREN
%token t_APP
%token t_LET
%token t_IN
%token t_FOREACH
%token t_DO
%token t_END
%token t_IF
%token t_THEN
%token t_ELSE
%token t_FUN
%token t_EXTERNAL
%token t_INTERNAL
%token t_PROGRAM
%token t_SHARING
%token t_EXEC

%token t_BACKQUOTE
%token t_UNIT
%token t_BOOL
%token t_INT
%token t_LIST
%token t_VARARGS
%token t_INTO
%token t_MAP_TO_CELLS

%type <exp> expr expr1 expr12 expr2 expr3 expr4 expr5 expr6 term list_body
%type <field_list> rec_el_list
%type <vlist> varlist arglist share varlist_comma
%type <elist> exprlist
%type <command> assignment
%type <command_list> alist
%type <cls> clause
%type <prog> progexpr atomic
%type <st> stat
%type <st_list> stat_list let_list
%type <str> let_sub
%type <type> type_expr type_expr2
%type <type_list> type_expr_list
%type <env> type_assign_list

%nonassoc UMINUS UNOT

%expect 16

%start stmts

%%

stmts

    : stmts t_SEMICOLON stmt 
    | stmt 
    ;

stmt

    : expr
    {

      VERBOSE; 

      $1->type_check ( top_scope, main_env ); // will throw an error if there is a problem

      Value * ans = $1->eval ( top_scope );

      if ( ans->get_type() != Value::UNIT ) { ans->print(); NL; NL;} 

      delete ans;

      //delete $1;

    }

    | expr t_ASSIGN expr
    { 

      VERBOSE;

      Expr * lhs = $1,
           * rhs = $3;

      !rhs->type_check ( top_scope, main_env );

      if ( lhs->get_type() == Expr::VARIABLE && top_scope->get ( lhs->get_name() ) == NULL) {

        top_scope->add ( lhs->get_name(), rhs->eval ( top_scope ) );

      } else {

	std::list<TypeExpr *> garbage, ng;

	if ( ! TypeExpr::unify ( lhs->compute_type ( top_scope, main_env, &ng, &garbage ),
				 rhs->compute_type ( top_scope, main_env, &ng, &garbage ) ) ) {

          TYPE_ERROR ( "Type Error: The symbol <i>" << lhs->tostring()
                       << "</i> has a different type than does the expression <i>" << rhs->tostring() << ".<i>" );

	} else if ( ! lhs->is_lhs() ) {

          TYPE_ERROR ( "Type Error: illegal expression " << lhs->tostring() << " on left hand side of assignment." );

	} else {

	  top_scope->set ( lhs, rhs->eval ( top_scope ) );

	}

	TypeExpr::take_out ( &garbage );

      }

      delete $1;
      delete $3;

    }

    | t_FUN t_VAR varlist t_DOT expr {

      VERBOSE;

      fun_temp = new TypeExpr ( new TypeExpr(), new TypeExpr() );

      PUSH_WITH_TYPE ( $2, fun_temp ); 

      std::list<char *>::iterator i;
      Expr * e = $5;
      SET_INFO(e);

      for ( i=$3->begin(); i != $3->end(); i++ ) {
	e = FUNC ( (*i), e );
	free (*i);
	SET_INFO(e);
      }

      delete $3;

      e->type_check ( top_scope, main_env );

      fun_temp->reset();
      fun_temp->free_children();

      POP; /* frees fun_temp */

      Value * answer = e->eval ( top_scope );
      top_scope->add ( $2, answer );
      top_scope->set_recursive ( $2 );
      delete e;

      free ( $2 );

    }

    | t_INTERNAL type_expr t_VAR t_LPAREN type_expr_list t_RPAREN t_STRING

    {

      VERBOSE;

      const char * error;

      EXTERNAL_CCLI_FUNCTION f = lookup_ccl_function ( $7 );

      if ( f == NULL )  {
        ERROR ( "Could not find symbol <i>" << $7 << "</i> in list of internally defined functions." );
      } 

      Value * efunc = new Value ( f, $2, $5 );
      top_scope->add ( $3, efunc );
      free ( $3 );

      while ( main_env != NULL )
	POP_SIMP;

      free ( $7 );

    }

    | t_PROGRAM t_VAR t_LPAREN arglist t_RPAREN {

      VERBOSE;

      std::list<char *>::iterator i;

      for ( i = $4->begin(); i != $4->end(); i++ ) {  // push parameter list onto main_env
	PUSH ( *i );
      }

    } t_ASSIGN progexpr {

      std::list<char *>::iterator i;

      $8->set_args($4);
      $8->compute_arg_types ( main_env ); // doesn't do anything if $8 is atomic
      top_scope->add($2,$8);
      $8->set_name ( $2 );

      for ( i = $4->begin(); i != $4->end(); i++ ) {  // pop parameter list
	POP;
      }

      TypeExpr::take_out ( &program_garbage );

      free ( $2 );

    } 

    |

    ;

arglist
    : arglist t_COMMA t_VAR { $$ = $1; $$->push_back ( $3 ); }
    | t_VAR { $$ = new std::list<char *>; $$->push_back ( $1 ); }
    | { $$ = new std::list<char *>; }
    ;

progexpr

    : progexpr t_PLUS atomic share { 

        $$ = new CompositeProgram ( $1, $3, $4, current_file_name[include_stack_ptr], lineNum[include_stack_ptr] );
	FREE_STR_LIST ( $4 );

      }

    | t_COMPOSE t_VAR t_IN expr t_COLON t_VAR t_LPAREN exprlist t_RPAREN share {

      // 1) $4 evaluates to a list of constants, and that it isn't empty?

      if ( ! $4->type_check ( top_scope, NULL ) ) {  // note: using NULL instead of main_env here because the
                                                     // index set should not depend on parameters of some enclosing
                                                     // program definition
        TYPE_ERROR ( "Something wrong with </i>" << $4->tostring() << "<i> in program composition expression." );
      }

      Value * L = $4->eval ( top_scope );

      if ( L->get_type() != Value::LIST ) {
        TYPE_ERROR ( "Index set in program composition does not evaluate to a list." );
      }

      if ( L->list_value()->size() < 1 )  {
        ERROR ( "Index set in program composition is empty." );
      }

      // 2) $6 is a program?
      Program * p = top_scope->get_program ( $6 );

      if ( p == NULL ) {
         ERROR ( "The symbol <i>" << $6 << "</i> is undefined, or is not defined as a program." );
      }

      // 3) $6->args and $8 have same length?
      if ( p->get_args()->size() != $8->size() ) {
         ERROR ( "The program <i>" << $6 << "</i> is instantiated with the wrong number of arguments "
                << "(should have " << p->get_args()->size() << ")." );
      }

      // 4) Make the composition
      $$ = Program::Composition ( p, $8, $2, L->list_value(), $10, top_scope, main_env, 
				  current_file_name[include_stack_ptr], lineNum[include_stack_ptr] );

      // 5) Delete old stuff
      free($2);
      free($6);
      FREE_STR_LIST ( $10 );
      FREE_EXPR_LIST ( $8 );
      delete $4;
      delete L;

    }

    | atomic { $$ = $1; }

    ;

share
    : t_SHARING arglist { $$ = $2; }
    | { $$ = new std::list<char *>; }
    ;

atomic

    : t_LCURLY stat_list t_RCURLY 

    {

      AtomicProgram * p = new AtomicProgram ( current_file_name[include_stack_ptr], lineNum[include_stack_ptr] );
      Clause * init = new Clause ( CONST ( true ) );
      std::list<Statement *>::iterator i;
      std::list<TypeExpr *> ng, needs_garbage;
      TypeExpr * T;
      int program_var_counter = 0;

      for ( i=$2->begin(); i!=$2->end(); i++ ) {

	if ( (*i)->com != NULL ) { // initializer //////////////////////////////////////////////////////////////////

	  init->addCommand ( (*i)->com );
	  T = (*i)->com->get_rhs()->compute_type ( top_scope, main_env, &ng, &program_garbage );

          if ( !T->is_consistent() ) TYPE_ERROR ( "Inconsistent type." );

          Expr * lhs = (*i)->com->get_lhs();

          if ( lhs != NULL 
	       && lhs->get_type() == Expr::VARIABLE 
	       && RETRIEVE(main_env,lhs->get_name()) == NULL ) {

	    //	    p->add_symbol ( lhs->get_name(), new Value ( Value::UNIT ) ); 
	    PUSH_WITH_TYPE ( lhs->get_name(), T );
	    program_var_counter++;

	  }

	  if ( lhs != NULL ) { // check types of lhs and rhs

	    TypeExpr * Tlhs = lhs->compute_type ( top_scope, main_env, &ng, &program_garbage );

            if ( !Tlhs->is_consistent() ) TYPE_ERROR ( "Inconsistent type." );

	    if ( ! TypeExpr::unify ( T, Tlhs ) ) {

              TYPE_ERROR ( "Incompatible types in assignment. "
                << "The left hand side has type <i>" << Tlhs->tostring()
                << "</i> while the right hand side has type <i>" << T->tostring() << ".</i>" );
	      
	    }

	  }

	} else if ( (*i)->cl != NULL ) { // clause /////////////////////////////////////////////////////////////////

	  T = (*i)->cl->get_guard()->compute_type ( top_scope, main_env, &ng, &program_garbage );
          if ( !T->is_consistent() ) TYPE_ERROR ( "Inconsistent type." );

          TypeExpr * B = new TypeExpr ( Value::BOOLEAN );
	  program_garbage.push_back ( B );

	  if ( ! TypeExpr::unify ( T, B ) ) {

            TYPE_ERROR ( "Clause does not have Boolean guard. In particular, "
              << "<i>" << (*i)->cl->get_guard()->tostring() << "</i> has type "
              << "<i>" << T->tostring() << ".</i>" );

	  }

	  std::list<Command *>::iterator j;

	  for ( j = (*i)->cl->get_commands()->begin(); j != (*i)->cl->get_commands()->end(); j++ ) {

	    if ( (*j)->get_lhs() != NULL ) {

	      TypeExpr * T1 = (*j)->get_lhs()->compute_type ( top_scope, main_env, &ng, &program_garbage ),
		* T2 = (*j)->get_rhs()->compute_type ( top_scope, main_env, &ng, &program_garbage );
	      
              if ( ! TypeExpr::unify ( T1, T2 ) ) {

                TYPE_ERROR ( "Incompatible types in assignment in command part of guarded command. "
                  << "<i>" << (*j)->get_lhs()->tostring() << "</i> has type <i>" << T1->tostring()
                  << "</i> while <i>" << (*j)->get_rhs()->tostring() << "</i> has type <i>" << T2->tostring() << ".</i>" );

	      }

	    } else {

	      TypeExpr * T = (*j)->get_rhs()->compute_type ( top_scope, main_env, &ng, &program_garbage );
	      if ( !T->is_consistent() )
		return -1;

	    }

	    //TypeExpr::take_out ( &program_garbage );

	  }

	  p->add_clause ( (*i)->cl );

	} else if ( (*i)->vars != NULL ) { // "needs" statement ////////////////////////////////////////////////////

          std::list<char *>::iterator j;

	  for ( j = (*i)->vars->begin(); j != (*i)->vars->end(); j++ ) {
	    PUSH ( *j );
	    needs_garbage.push_back ( main_env->get_type() );
            p->add_needs ( *j ); 
	    program_var_counter++;
	    free ( *j );
	  }

	  delete (*i)->vars;

        }

	delete (*i);

      } // done reading statements

      delete $2;
      p->set_init ( init );
      $$ = p;

      // save parameter and local variable types
      {

	Environment * env = main_env;
	std::list<TypeExpr *> * vars = new std::list<TypeExpr *>, 
                              * ng_save = new std::list<TypeExpr *>;

	while ( env != NULL ) {
	  env->get_type()->compute_non_generics ( vars );
	  env = env->get_tail();
	}
	
	COPY_TYPEVAR_LIST ( vars, ng_save );

	p->set_non_generics ( ng_save );

	std::list<TypeExpr *>::iterator w;

	Environment * temp = main_env, * locals = NULL;

	for ( int j = 0; j < program_var_counter; j++ ) { // this copies variables in assignments and in needs statements
	  locals = new Environment ( temp->get_name(), temp->get_type()->copy_ex ( vars, ng_save ), locals );
	  temp = temp->get_tail();
	}

	p->set_locals ( locals );
	
	Environment * arg_types = NULL;
	if ( temp != NULL )
	  arg_types = temp->copy_ex ( vars, ng_save );

	p->set_arg_types ( arg_types );

	delete vars;

      } 

      // pop local program vars and add default values 
      for ( int j = 0; j < program_var_counter; j++ ) {
	p->add_symbol ( main_env->get_name(), main_env->get_type()->default_value() );
	POP_SIMP;

      }

      // free needs type exprs
      std::list<TypeExpr *>::iterator q;
      for ( q=needs_garbage.begin(); q != needs_garbage.end(); q++ )
	delete (*q);

    }

    | t_VAR t_LPAREN exprlist t_RPAREN

    {

      Program * p = top_scope->get_program($1);

      if ( p == NULL ) {
         ERROR ( "Symbol <i>" << $1 << "</i> undefined." );
      }

      std::list<char *> * args = p->get_args();

      if ( args->size() != $3->size() ) {
         ERROR ( "The program <i>" << $1 << "</i> is instantiated with the wrong number of arguments "
             << "(should have " << args->size() << ")." );
      }

      p = p->copy();
      p->set_insts ( $3, top_scope, main_env, $1 );
      $$ = new InstProgram ( p, current_file_name[include_stack_ptr], lineNum[include_stack_ptr] ); 

      free ( $1 );

    }

    | t_LPAREN progexpr t_RPAREN { $$ = $2; }

    ;

stat_list

    : stat_list stat
    {
      $$ = $1;
      $$->push_back ( $2 );
    }
    | stat
    {
      $$ = new std::list<Statement *>;
      $$->push_back ( $1 );
    }
    ;

stat

    : assignment t_SEMICOLON { $$ = new Statement ( $1 );  }
    | t_NEEDS varlist_comma t_SEMICOLON { $$ = new Statement ( $2 ); }
    | clause t_SEMICOLON { $$ = new Statement ( $1 ); }
    | clause { $$ = new Statement ( $1 ); }
    ;

clause

    : expr t_COLON t_LCURLY alist t_RCURLY { $$ = new Clause ( $1, $4 ); }
    | expr t_COLON t_LCURLY alist separator t_RCURLY { $$ = new Clause ( $1, $4 ); }
    ;

alist

    : alist separator assignment
    {
      $$ = $1;
      $$->push_back($3);
    }
    | assignment 
    {
      $$ = new std::list<Command *>;
      $$->push_back($1);
    }
    ;

separator

    : t_SEMICOLON | t_COMMA
    ;

assignment

    : expr t_ASSIGN expr { $$ = new Command ( $1, $3 ); }
    | expr { $$ = new Command ( NULL, $1 ); }
    ;

varlist

    : varlist t_VAR
    {
      $1->push_front ( $2 );
      $$ = $1;
    }
    | t_VAR 
    {
      $$ = new std::list<char *>; 
      $$->push_back ( $1 );
    }
    ;

varlist_comma

    : varlist_comma t_COMMA t_VAR
    {
      $1->push_front ( $3 );
      $$ = $1;
    }
    | t_VAR 
    {
      $$ = new std::list<char *>; 
      $$->push_back ( $1 );
    }
    ;

expr

    : expr1

    | t_LAMBDA t_VAR { PUSH($2); } t_DOT expr { 

      $$ = FUNC ( $2, $5 ); 
      SET_INFO($$);
      free ( $2 ); 
      POP;

    }

    ;

expr1

    : expr12 { $$ = $1; }
    | expr1 t_AND expr12 { $$ = BINEXP ( Expr::AND, $1, $3 ); SET_INFO($$); }
    | expr1 t_OR expr12  {  $$ = BINEXP ( Expr::OR, $1, $3 ); SET_INFO($$); }
    | expr1 t_STRCAT expr12 { $$ = BINEXP ( Expr::STR_CAT, $1, $3 ); SET_INFO($$); }

    ;

expr12

    : expr2 { $$ = $1; }
    | expr12 t_LESS_THAN expr2 { $$ = BINEXP ( Expr::LT, $1, $3 ); SET_INFO($$); }
    | expr12 t_LESS_THAN_OR_EQUAL expr2 { $$ = BINEXP ( Expr::LE, $1, $3 ); SET_INFO($$); }
    | expr12 t_GREATER_THAN expr2 { $$ = BINEXP ( Expr::GT, $1, $3 ); SET_INFO($$); }
    | expr12 t_GREATER_THAN_OR_EQUAL expr2 { $$ = BINEXP ( Expr::GE, $1, $3 ); SET_INFO($$); }
    | expr12 t_EQUAL expr2 { $$ = BINEXP ( Expr::EQ, $1, $3 ); SET_INFO($$); }
    | expr12 t_NOT_EQUAL expr2 { $$ = BINEXP ( Expr::NE, $1, $3 ); SET_INFO($$); }
    | t_NOT expr2 { $$ = UNEXP ( Expr::NOT, $2 ); SET_INFO($$); }
    ;

expr2

    : expr3 { $$ = $1; }
    | expr2 t_PLUS expr3 { $$ = BINEXP ( Expr::PLUS, $1, $3 ); SET_INFO($$); }
    | expr2 t_MINUS expr3 { $$ = BINEXP ( Expr::MINUS, $1, $3 ); SET_INFO($$); }
    | expr2 t_RECORD_ADD expr3 { $$ = BINEXP ( Expr::RECORD_ADD, $1, $3 ); SET_INFO($$); }
    ;

expr3

    : expr4 { $$ = $1; }
    | expr3 t_MULT expr4 { $$ = BINEXP ( Expr::MULT, $1, $3 ); SET_INFO($$); }
    | expr3 t_MOD expr4 { $$ = BINEXP ( Expr::MOD, $1, $3 ); SET_INFO($$); }
    | expr3 t_DIV expr4 { $$ = BINEXP ( Expr::DIV, $1, $3 ); SET_INFO($$); }
    | t_MINUS expr3 { $$ = UNEXP ( Expr::UMIN, $2 ); SET_INFO($$); }
    | expr3 t_CONCAT expr4 { $$ = BINEXP ( Expr::CONCAT, $1, $3 ); SET_INFO($$); } 
    | expr3 t_CONS expr4 { $$ = BINEXP ( Expr::CONS, $1, $3 ); SET_INFO($$); }

    | t_BACKQUOTE expr3 { 
	std::list<char *> locals;
	$$ = $2->prevify ( &locals ); 
	delete $2; 
    }
    ;

expr4

    : expr5 { $$ = $1; }
    | expr4 t_POWER expr5 { $$ = BINEXP ( Expr::POW, $1, $3 ); SET_INFO($$); }
    | t_HEAD expr4 { $$ = UNEXP ( Expr::HEAD, $2 ); SET_INFO($$); }
    | t_TAIL expr4 { $$ = UNEXP ( Expr::TAIL, $2 ); SET_INFO($$); }
    ;

expr5

    : expr6 { $$ = $1; }
    | expr5 expr6 { $$ = APP ( $1, $2 ); SET_INFO($$); }
    | expr5 t_DOT expr6 { $$ = BINEXP ( Expr::DOT, $1, $3 ); SET_INFO($$); }
    | expr5 t_LBRACKET expr t_RBRACKET { $$ = BINEXP ( Expr::LIST_EL, $1, $3 ); SET_INFO($$); }

    | t_VAR t_LPAREN exprlist t_RPAREN { // this could be either a function application, an external function application
                                         // or possibly a program

      Value * f = top_scope->get ( $1 );

      if ( f && f->get_type() == Value::EX_FUNCTION ) { // external function 

        if ( !f ) {
          ERROR ( "Seriously odd error. The symbol <i>" << $1 << "</i> is undefined." );
        }

        $$ = EAPP ( f->copy(), $3 ); 
	SET_INFO($$);

	free ( $1 );

      } else if ( top_scope->get_program ( $1 ) ) {

          TYPE_ERROR ( "The program <i>" << $1 << "</i> appears mixed with expressions that are not programs." );

      } else {

	if ( $3->size() == 1 ) {

          Expr * arg = VAR ( $1 );
	  SET_INFO ( arg );

	  $$ = APP ( arg, *($3->begin()) );
	  SET_INFO($$);

	  free ( $1 );
	  delete $3;

	} else {

          ERROR ( "Parse error." );

	}

      }

    }

    | t_VAR t_LPAREN exprlist t_COMMA t_PROGRAM progexpr t_RPAREN {

      Value * f = top_scope->get ( $1 );

      if ( f && f->get_type() == Value::EX_FUNCTION ) { // external function 

        if ( !f ) {
          ERROR ( "Seriously odd error. The symbol <i>" << $1 << "</i> is undefined." );
        }

        $3->push_back ( new Expr ( $6 ) );
        $$ = EAPP ( f->copy(), $3 ); 
	SET_INFO($$);

      } else {

        ERROR ( "<i>" << $1 << "</i> should refer to an external function since it has a program as an argument." );

      }

      free ( $1 );

    }

    ;

expr6
 
    : term { $$ = $1; }

    | t_LPAREN expr t_RPAREN { $$ = $2; SET_INFO($$); }

    | t_LET let_list t_IN expr t_END { 

      std::list<Statement *>::iterator i;
      Expr * result = NULL;

      for ( i = $2->begin(); i != $2->end(); i++ ) {

        Command * c = (*i)->com;

        if ( c == NULL || c->get_lhs()->get_type() != Expr::VARIABLE ) {
          ERROR ( "Error in let expression (not a variable assignment). " );
	} 

        if ( result == NULL ) {

          result = LETEXP ( c->get_lhs()->get_name(), c->get_rhs()->copy(), $4 );

        } else {

          result = LETEXP ( c->get_lhs()->get_name(), c->get_rhs()->copy(), result );

	}

      }

      for ( i = $2->begin(); i != $2->end(); i++ ) {

        Command * c = (*i)->com;
	POP;
	delete c;

      }

      if ( result == NULL ) {
        ERROR ( "Empty assignment section in let statement." );
      }

      $$ = result;
      SET_INFO($$);

      delete $2;

    }

    | t_IF expr t_THEN expr t_ELSE expr t_END 

    { 

      $$ = IFEXP ( $2, $4, $6 ); 
      SET_INFO($$);

    }

    | t_FOREACH t_VAR t_IN expr t_DO expr t_END {

      Expr * elambda = FUNC ( $2, $6 );

      char buf[80];
      sprintf ( buf, "fmanglewahwah%d", foreach_index++ );

      Expr * body = 
        IFEXP ( 
	 BINEXP ( Expr::EQ, VAR("w"), EMPTY ),
	 EMPTY,
	 BINEXP ( 
             Expr::CONS, 
	     APP ( elambda, UNEXP ( Expr::HEAD, VAR("w") ) ),
             APP ( VAR(buf), UNEXP ( Expr::TAIL, VAR("w") ) ) 
           )
        );

      top_scope->add ( buf, new Value ( "w", body ) );
      top_scope->set_recursive ( buf );

      $$ = APP ( VAR(buf), $4 );
      SET_INFO($$);
      free ( $2 );

    }

    | t_MAP_TO_CELLS expr t_END {

      EXTERNAL_CCLI_FUNCTION m2c = lookup_ccl_function ( "map_to_cells" );

      Expr * body = $2;

      std::list<char *> * fv = new std::list<char *>;
      std::list<char *>::iterator i;
      $2->freevars ( fv );
      
      for ( i=fv->begin(); i!=fv->end(); i++ ) {
        body = FUNC ( *i, body );
      }

      TypeExpr * R = new TypeExpr();
      std::list<TypeExpr *> * Ta = new std::list<TypeExpr *>;
      Ta->push_back ( new TypeExpr () );

      std::list<Expr *> * args = new std::list<Expr *>;
      args->push_back ( body );

      $$ = EAPP ( new Value ( m2c, R, Ta ), args );

    } 

    ;

let_sub
    : t_VAR t_ASSIGN 
    { 
      PUSH($1); 
      $$ = $1;
    }
    ;

let_list

    : let_sub expr t_COMMA let_list {

      $$ = $4;
      $$->push_back ( new Statement ( new Command ( VAR ( $1 ), $2 ) ) );

    }

    | let_sub expr {
 
      $$ = new std::list<Statement *>;
      $$->push_front ( new Statement ( new Command ( VAR ( $1 ), $2 ) ) );

    }

    ;

exprlist

    : exprlist t_COMMA expr 
    {

      $$ = $1;
      $$->push_back ( $3 );

    }

    | expr { 

      $$ = new std::list<Expr *>; 
      $$->push_back ( $1 );

    }

    | { $$ = new std::list<Expr *>; }
    ;

term

    : t_INTEGER { $$ = CONST ( $1 ); SET_INFO($$); }

    | t_VAR { $$ = VAR ( $1 ); free($1); SET_INFO($$); }
    | t_REAL { $$ = CONST ( $1 ); SET_INFO($$); }
    | t_STRING { $$ = CONST ( $1 ); SET_INFO($$); free ( $1 ); }
    | t_TRUE { $$ = CONST ( true ); SET_INFO($$); }
    | t_FALSE { $$ = CONST ( false ); SET_INFO($$); }
    | t_LCURLY list_body t_RCURLY { $$ = $2; SET_INFO($$); }

    | t_LBRACKET rec_el_list t_RBRACKET

      {

	std::list<Expr::FIELD *>::iterator i;

	$$ = RECEXP;
	SET_INFO($$);

	for ( i = $2->begin(); i != $2->end(); i++ ) {
	  $$->addField ( (*i)->name, (*i)->e );
	  free ( (*i)->name );
          delete *i;
	}

	delete $2;

      }

    ;

list_body

    : expr t_COMMA list_body {

      $$ = BINEXP ( Expr::CONS, $1, $3 );
      SET_INFO($$);

    }

    | expr { 

       $$ = BINEXP ( Expr::CONS, $1, EMPTY ); 
       SET_INFO($$);

    }

    | { $$ =  EMPTY; }
    ;

rec_el_list

    :  rec_el_list t_COMMA t_VAR t_ASSIGN expr
      {
	$$ = $1;
        Expr::FIELD * f = new Expr::FIELD;
	f->name = $3;
	f->e = $5;
	$$->push_back ( f );
      }

    | t_VAR t_ASSIGN expr 
      {
        $$ = new std::list<Expr::FIELD *>; 
	Expr::FIELD * f = new Expr::FIELD;
	f->name = $1;
	f->e = $3;
        $$->push_back ( f );
      }

    | { $$ = new std::list<Expr::FIELD *>; }
    ;

type_expr

    : t_BACKQUOTE t_VAR

    {

      TypeExpr * T = RETRIEVE ( main_env, $2 );

      if ( T == NULL ) {

	PUSH ( $2 );
	$$ = main_env->get_type();

      } else 

	$$ = T;

      free ( $2 );

    }

    | t_UNIT { $$ = new TypeExpr ( Value::UNIT ); }
    | t_BOOL { $$ = new TypeExpr ( Value::BOOLEAN ); }
    | t_REAL { $$ = new TypeExpr ( Value::REAL ); }
    | t_INT { $$ = new TypeExpr ( Value::INTEGER ); }
    | t_STRING { $$ = new TypeExpr ( Value::STRING ); }
    | type_expr t_LIST { $$ = new TypeExpr ( $1 ); }
    | t_LPAREN type_expr t_RPAREN { $$ = $2; }
    | t_VARARGS { $$ = new TypeExpr ( true ); }
    | t_LBRACKET { rec_extensible_flag = false; } type_assign_list t_RBRACKET { 
      $$ = new TypeExpr ( $3 ); 
      $$->set_extensibility ( rec_extensible_flag );
    }

    | type_expr2 { $$ = $1; }

    ;

type_assign_list 

    : t_VAR t_ASSIGN type_expr t_COMMA type_assign_list

    {

      $$ = new Environment ( $1, $3, $5 );
      free ( $1 );

    }

    | t_VAR t_ASSIGN type_expr

    {

      $$ = new Environment ( $1, $3, NULL );
      free ( $1 );

    }

    | t_VARARGS t_COMMA type_assign_list 

    {
      $$ = $3;
      rec_extensible_flag = true;
    }

    | t_VARARGS

    {
      $$ = NULL;
      rec_extensible_flag = true;
    }

    | { $$ = NULL; }

    ;

type_expr2

    : type_expr t_INTO type_expr { $$ = new TypeExpr ( $1, $3 ); }

    ;

type_expr_list

    : type_expr t_COMMA type_expr_list

    {

      $$ = $3;
      $$->push_front ( $1 );

    }

    | type_expr

    {

      $$ = new std::list<TypeExpr *>;
      $$->push_back ( $1 );

    }

    | { $$ =  new std::list<TypeExpr *>; }

    ;

%%

int yyerror ( char * s ) { 

    std::stringstream strm;
    strm << s << ": near line " << lineNum[include_stack_ptr] << " of " << current_file_name[include_stack_ptr];

    throw ( strm.str() );

    return -1;

}

extern FILE * yyin;

extern void RegisterCCLInternals ( void );

int readOrganismProgram ( Scope * sc, const char * path ) {

  RegisterCCLInternals();

  int i;

  top_scope = sc;

  if ( path == NULL )
    path = strdup ( "(null)" );

  if ( ( yyin = fopen ( path, "r" ) ) == NULL ) {
    ERROR ( "gro is unable to open file named <i>" << path << "</i>" );
  }

  yyrestart ( yyin );

  include_stack_ptr = 0;

  for ( i=0; i<MAX_INCLUDE_DEPTH+1; i++ ) 
    lineNum[i] = 1;  

  current_file_name[0] = strdup ( path );

  try {
    i = yyparse();
  }

  catch ( std::string err ) {
     fclose ( yyin );
     throw err;
  }

  fclose ( yyin );

  while ( !ccl_function_list.empty() ) {
    delete ccl_function_list.front();
    ccl_function_list.pop_front();
  }

  return i;

}

void register_ccl_function ( std::string name, EXTERNAL_CCLI_FUNCTION f ) {

  InternalFunction * inf = new InternalFunction;

  inf->name = name;
  inf->f = f;

  ccl_function_list.push_back ( inf );

}

EXTERNAL_CCLI_FUNCTION lookup_ccl_function ( std::string name ) {

  std::list<InternalFunction *>::iterator i;

  for ( i = ccl_function_list.begin(); i != ccl_function_list.end(); i++ ) {

    if ( (*i)->name == name )
      return (*i)->f;
  }

  return NULL;

}
