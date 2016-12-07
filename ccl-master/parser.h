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

#ifndef _PARSER__H
#define _PARSER__H

#include "Expr.h"
#include <string>

#define MAX_INCLUDE_DEPTH 50

// for statement lists in clauses
class Statement {

  public:

    Statement ( Command * c ) : com ( c ), cl ( NULL ), vars ( NULL ) {}
    Statement ( Clause * c ) : com ( NULL ), cl ( c ), vars ( NULL ) {}
    Statement ( std::list<char *> * L ) : com ( NULL ), cl ( NULL ), vars ( L ) {}
    Statement ( void ) : com ( NULL ), cl ( NULL ), vars ( NULL ) {}
    Command * com;
    Clause * cl;
    std::list<char *> * vars;

};

Scope * readProgram ( char * path, int argc, char * argv[] );

int readOrganismProgram ( Scope * sc, const char * path );

struct InternalFunction {

  EXTERNAL_CCLI_FUNCTION f;
  std::string name;

};

void register_ccl_function ( std::string name, EXTERNAL_CCLI_FUNCTION f );

EXTERNAL_CCLI_FUNCTION lookup_ccl_function ( std::string name );

#endif
