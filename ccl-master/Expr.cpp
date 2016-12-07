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

#define DBG {                                             \
    printf ( "Debug statement: reached %s, line %d\n",    \
                    __FILE__, __LINE__);                  \
    fflush ( stdout );                                    \
}

#define NL printf ( "\n" ); fflush (stdout);

#define LINE_INFO_SET { line = -1; file = NULL; } // printf ( "new expression (%x): ", this ); print(); NL }

static int expr_count = 0;

int num_exprs ( void ) { return expr_count; }

Expr::Expr ( Expr::TYPE t ) : type(t) {

  LINE_INFO_SET;
  expr_count++;
}

Expr::Expr ( void ) : type(RECORD) {

  LINE_INFO_SET;
  fields = new std::list<FIELD *>;
  expr_count++;

}

Expr::Expr ( Value * v ) : type(CONSTANT), val(v) { LINE_INFO_SET; expr_count++; }

Expr::Expr ( Program * p ) : type(PROGRAM), program(p) { LINE_INFO_SET; expr_count++; }

Expr::Expr ( char * str ) : type(VARIABLE) {

  name = strdup ( str );
  LINE_INFO_SET;
  expr_count++;

}

Expr::Expr ( Expr * f, Expr * a ) : type (APPLICATION), e1(f), e2(a) { LINE_INFO_SET; expr_count++; }

Expr::Expr ( OPERATOR p, Expr * e ) : type (UNARY), op(p), e1(e) { LINE_INFO_SET; expr_count++; }

Expr::Expr ( OPERATOR p, Expr * a, Expr * b ) : type (BINARY), op(p), e1(a), e2(b) { LINE_INFO_SET; expr_count++; }

Expr::Expr ( const char * str, Expr * v, Expr * e ) : type(LET), e1(v), e2(e)  {

  name = strdup ( str );
  LINE_INFO_SET;
  expr_count++;

}

Expr::Expr ( Value * f, std::list<Expr *> * args ) : type(EX_APPLICATION), val(f), arglist(args) { LINE_INFO_SET; expr_count++; }

Expr::Expr ( Expr * a, Expr * b, Expr * c ) : e1(a), e2(b), e3(c), type(IF) { LINE_INFO_SET; expr_count++; }

Expr::~Expr ( void ) {

  //  printf ( "deleting (%x): ", this ); print(); NL;

  switch ( type ) {

  case CONSTANT:

    delete val;
    break;

  case VARIABLE:

    free ( name );
    break;

  case APPLICATION:

    delete e1;
    delete e2;
    break;

  case EX_APPLICATION:
    
    {

      std::list<Expr *>::iterator i;

      for ( i = arglist->begin(); i != arglist->end(); i++ )
	delete (*i);

      delete arglist;
      delete val;

    }
    break;

  case UNARY:

    delete e1;
    break;

  case BINARY:

    delete e1;
    delete e2;
    break;

  case LET:

    free ( name );
    delete e1;
    delete e2;
    break;

  case IF:

    delete e1;
    delete e2;
    delete e3;
    break;

  case RECORD:

    {

      std::list<FIELD *>::iterator i;

      for ( i = fields->begin(); i != fields->end(); i++ ) {
	free ( (*i)->name );
	delete (*i)->e;
	delete (*i);
      }

      delete fields;

    }
    break;

  case PROGRAM:
    delete program;
    break;

  case THISS:
  case RANDOM:
    break;

  default:
    fprintf (stderr, "WARNING: unimplemented delete method\n" );

  }

  expr_count--;

  //  if ( expr_count == 0 )
  //  printf ( "  no exprs left\n" );

  if ( expr_count < 0 )
    printf ( "WARNING: instance expr_count in ~Expr is negative!\n" );

}

static char op_strings[][4] = { "&", "|", "!", 
                          "%",
                          "^",
                          "+", "-", "*", "/", "-", "<", ">", "<=", ">=", 
                          "<>",
                          "hd", "tl", "@", "#", "[]",
                          "=", "!=",
                          ".", "'", "<<" };

static char html_op_strings[][10] = { "&amp;", "|", "!",
                          "%",
                          "^",
                          "+", "-", "*", "/", "-", "&lt;", "&gt;", "&lt;=", "&gt;=",
                          "<>",
                          "hd", "tl", "@", "#", "[]",
                          "=", "!=",
                          ".", "'", "&lt;&lt;" };

std::string Expr::tostring ( void ) {

    std::stringstream strm;
    tostring_aux ( &strm );
    return strm.str();

}

void Expr::tostring_aux ( std::stringstream * strm ) {

    switch ( type ) {

    case CONSTANT:
        val->tostring_aux ( strm );
        break;

    case VARIABLE:
        *strm << name;
        break;

    case APPLICATION:
        *strm << "( "; e1->tostring_aux ( strm );
        *strm << " ";  e2->tostring_aux ( strm );
        *strm << " )";
        break;

    case EX_APPLICATION:

      {

        *strm << "(";
        val->tostring_aux(strm);
        *strm << " ( ";

        std::list<Expr *>::iterator i, j;

        for ( i = arglist->begin(); i != arglist->end(); i++ ) {
            (*i)->tostring_aux(strm);
            j = i;
            j++;
            if ( j != arglist->end() )
                *strm << ", ";
        }

        *strm << " )) ";

      }

      break;

    case UNARY:

        *strm << "(" << html_op_strings[op];
        e1->tostring_aux ( strm );
        *strm << ")";
        break;

    case BINARY:

        *strm << "(";
        e1->tostring_aux ( strm );

        if ( op == LIST_EL ) {

            *strm << "[";
            e2->tostring_aux ( strm );
            *strm << "]";

        } else {

            *strm << html_op_strings[op];
            e2->tostring_aux ( strm );

        }

        *strm << ")";

        break;

    case LET:

        *strm << "let" << name;
        e1->tostring_aux ( strm );
        *strm << " in ";
        e2->tostring_aux ( strm );
        break;

    case IF:
      {
        *strm << "if "; e1->tostring_aux ( strm );
        *strm << " then "; e2->tostring_aux ( strm );
        *strm << " else "; e3->tostring_aux ( strm );
        *strm << " ";
      }
      break;

    case RECORD:
      {

        std::list<FIELD *>::iterator i, j;

        *strm << "[ ";

        for ( i = fields->begin(); i != fields->end(); i++ ) {
            *strm << (*i)->name << " :=";
            (*i)->e->tostring_aux ( strm );
            j = i;
            j++;
            if ( j != fields->end() )
                *strm << "; ";
            else
                *strm <<" ";

        }

        *strm << "]";

      }
      break;

    case PROGRAM:
        *strm << "program";
        break;

    case THISS:
        *strm << "this";
        break;

    default:
        throw ( std::string ( "unimplemented tostring_aux method for Expr" ) );

    }

}

void Expr::print ( FILE * fp ) {

  switch ( type ) {

  case CONSTANT:
    val->print(fp);
    break;

  case VARIABLE:
    fprintf ( fp, "%s", name );
    break;

  case APPLICATION:
    fprintf ( fp, "( " ); e1->print(fp); 
    fprintf ( fp, " " ); e2->print(fp);
    fprintf ( fp, " )" );
    break;

  case EX_APPLICATION:

    {

      fprintf ( fp, "(" );
      val->print(fp); fprintf ( fp, " ( " );
      std::list<Expr *>::iterator i, j;

      for ( i = arglist->begin(); i != arglist->end(); i++ ) {
        (*i)->print(fp);
        j = i;
        j++;
        if ( j != arglist->end() )
          fprintf ( fp, ", " );
      }

      fprintf ( fp, " )) " );

    }

    break;

  case UNARY:
    fprintf ( fp, "(%s ", op_strings[op] );
    e1->print(fp);
    fprintf ( fp, ")" );
    break;

  case BINARY:

    fprintf ( fp, "(" );
    e1->print(fp);

    if ( op == LIST_EL ) {

      fprintf ( fp, "[" ); 
      e2->print(fp);
      fprintf ( fp, "]" );

    } else {

      fprintf ( fp, "%s", op_strings[op] );
      e2->print(fp); 

    }

    fprintf ( fp, ")" );

    break;

  case LET:
    fprintf ( fp, "let %s := ", name );
    e1->print(fp);
    fprintf ( fp, " in " );
    e2->print(fp);
    break;

  case IF:
    {
      fprintf ( fp, "if " ); e1->print(fp); 
      fprintf ( fp, " then " ); e2->print(fp);
      fprintf ( fp, " else " ); e3->print(fp);
      fprintf ( fp, " " );
    }
    break;

  case RECORD:
    {

      std::list<FIELD *>::iterator i, j;

      fprintf ( fp, "[ " );

      for ( i = fields->begin(); i != fields->end(); i++ ) {
	fprintf ( fp, "%s := ", (*i)->name );
	(*i)->e->print(fp);
	j = i;
	j++;
	if ( j != fields->end() )
	  fprintf ( fp, "; " );
	else
	  fprintf ( fp, " " );

      }

      fprintf ( fp, "]" );

    }
    break;
  
  case PROGRAM:
    fprintf ( fp, "program" );
    break;

  case RANDOM:
    fprintf ( fp, "random" );
    break;

  case THISS:
    fprintf ( fp, "this" );
    break;

  default:
    fprintf ( fp, "unimplemented print method for Expr\n" );

  } 

}

Expr * Expr::copy ( void ) {

  Expr * e;

  //  printf ( "copying (%x): ", this ); NL;

  switch ( type ) {

  case CONSTANT:
    e = new Expr ( val->copy() );
    break;

  case VARIABLE:
    e = VAR ( name );
    break;

  case UNARY:
    e = UNEXP ( op, e1->copy() );
    break;

  case BINARY:
    e = BINEXP ( op, e1->copy(), e2->copy() );
    break;

  case APPLICATION:
    e = APP ( e1->copy(), e2->copy() );
    break;

  case LET:
    e = LETEXP ( name, e1->copy(), e2->copy() );
    break;

  case IF:
    e = IFEXP ( e1->copy(), e2->copy(), e3->copy() );
    break;

  case RECORD:

    {

      std::list<FIELD *>::iterator i;
      e = RECEXP;
      for ( i = fields->begin(); i != fields->end(); i++ )
        e->addField ( (*i)->name, (*i)->e->copy() );

    }
    break;

  case EX_APPLICATION:

    {

      std::list<Expr *> * elist = new std::list<Expr *>;
      std::list<Expr *>::iterator i;

      for ( i = arglist->begin(); i != arglist->end(); i++ )
        elist->push_back ( (*i)->copy() );

      e = EAPP ( val->copy(), elist );

    }
    break;

  case PROGRAM:
    e = new Expr ( program->copy() );
    break;

  case RANDOM:
    e = new Expr ( RANDOM );
    break;

  case THISS:
    e = new Expr ( THISS );
    break;

  default:

    fprintf ( stderr, "Copy not implemented for this type (%d) of Expr %x\n", type, this );
    exit ( -1 );

  }

  e->set_line_info ( line, file );

  return e;

}

void Expr::addField ( const char * str, Expr * e ) {

  if ( type != RECORD ) {

    fprintf ( stderr, "Attempted to add field %s to non-record\n", str );
    exit ( -1 );

  }

  FIELD * f = new FIELD;
  f->name = strdup ( str );
  f->e = e;

  fields->push_back ( f );

}

Value * Expr::get_lhs_value ( Scope * s ) {

  if ( type == VARIABLE ) {

    return s->get(name);

  } else if ( type == BINARY && op == DOT ) {

    Value * v = e1->get_lhs_value ( s );
    ASSERT ( v->get_type() == Value::RECORD && e2->get_type() == VARIABLE );
    Value * f = v->getField ( e2->get_name() );
    ASSERT ( f != NULL );

    return f;

  } else if ( type == BINARY && op == LIST_EL ) {

    Value * L = e1->get_lhs_value ( s );
    ASSERT ( L->get_type() == Value::LIST );
    Value * i = e2->eval(s);
    ASSERT ( i->get_type() == Value::INTEGER || i->get_type() == Value::REAL );

    int q = 0, j = i->int_value();
    std::list<Value *> * K = L->list_value();
    std::list<Value *>::iterator m = K->begin();

    if ( j < 0 || j >= K->size() ) {

      fprintf ( stderr, "Expr::get_lhs_value: array out of bounds on line %d of '%s'\n", line_num(), 
		file_name() == NULL ? "no file info" : file_name() );
      exit (-1);

    }

    while ( q < j ) { q++; m++; }
    delete i;
    return *m;

  } else {

    return NULL;

  }

}

bool Expr::is_lhs ( void ) {

  if ( type == VARIABLE )

    return true;

  else if ( ( type == BINARY && op == DOT ) || ( type == BINARY && op == LIST_EL ) ) 

    return e1->is_lhs();

  else 

    return false;

}

bool islocal ( char * name, std::list<char *> * L ) {

  std::list<char *>::iterator i;

  for ( i=L->begin(); i != L->end(); i++ ) 
    if ( ! strcmp ( name, *i ) )
      return true;

  return false;

}

Expr * Expr::prevify ( std::list<char *> * locals ) {

  Expr * e = NULL;

  switch ( type ) {

  case CONSTANT:

    if ( val->get_type() == Value::FUNCTION ) {

      locals->push_front ( val->func_arg_name() );
      e = FUNC ( val->func_arg_name(), val->func_body()->prevify ( locals ) );
      locals->pop_front();

    } else

      e = copy();

    break;

  case VARIABLE:

    if ( !islocal ( name, locals ) )
      e = UNEXP ( PREV, copy() );
    else
      e = copy();
    break;

  case UNARY:

    if ( op == PREV ) {
      fprintf ( stderr, "Warning: double '' near line %d of '%s'\nNote: to ccli, ''x is the same as 'x\n",
                         line_num(), file_name() != NULL ? file_name() : "no file info" );
      e = copy();
    } else
      e = UNEXP ( op, e1->prevify(locals) );
    break;

  case BINARY:

    if ( op == DOT )
      e = BINEXP ( op, e1->prevify(locals), e2->copy() );
    else
      e = BINEXP ( op, e1->prevify(locals), e2->prevify(locals) );
    break;

  case APPLICATION:

    e = APP ( e1->prevify(locals), e2->prevify(locals) );
    break;

  case LET:

    {
      Expr * A, * B;
      A = e1->prevify ( locals );
      locals->push_front ( name );
      B = e2->prevify ( locals );
      locals->pop_front();
      e = LETEXP ( name, A, B );
    }
    break;

  case IF:

    e = IFEXP ( e1->prevify(locals), e2->prevify(locals), e3->prevify(locals) );
    break;

  case RECORD:

    {

      std::list<FIELD *>::iterator i;
      e = RECEXP;
      for ( i = fields->begin(); i != fields->end(); i++ )
        e->addField ( (*i)->name, (*i)->e->prevify(locals) );

    }
    break;

  case EX_APPLICATION:

    {

      std::list<Expr *> * elist = new std::list<Expr *>;
      std::list<Expr *>::iterator i;

      for ( i = arglist->begin(); i != arglist->end(); i++ )
        elist->push_back ( (*i)->prevify(locals) );

      e = EAPP ( val->copy(), elist );

    }
    break;

  default:

    fprintf ( stderr, "Copy not implemented for this kind of Expr (%d): ", type );
    print(stderr); NL;
    exit ( -1 );

  }

  e->set_line_info ( line, file );

  return e;

  

}

