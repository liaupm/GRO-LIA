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

#include "Schedulers.h"

void standard_scheduler ( Program * p, Scope * s ) {

  p->init_params ( s );
  p->init ( s );
  
  while ( 1 ) 
    p->step ( s );

}

void random_epoch_scheduler  ( Program * p, Scope * s ) {

  CLAUSE_LIST * cl[2];
  cl[0] = new CLAUSE_LIST;
  cl[1] = new CLAUSE_LIST;
  int current = 0, len, max, n, j;
  CLAUSE_LIST::iterator i;
  Scope clause_scope;
  std::list<SymbolTable *>::reverse_iterator k;

  p->make_clause_list ( cl[current], s );
  len = cl[current]->size();
  max = len;

  p->init_params ( s );
  p->init ( s );

  while ( 1 ) {

    // reset to other list if current list is empty
    if ( len == 0 ) {
      current = ( current + 1 ) % 2;
      len = max;
    }

    // choose a clause to execute
    n = rand() % len;
    i = cl[current]->begin();
    for ( j=0; j<n; j++ ) i++;

    // set up scope
    for ( k = (*i)->second->rbegin(); k != (*i)->second->rend(); k++ ) 
      clause_scope.push ( *k );

    // execute the clause
    (*i)->first->execute ( &clause_scope );

    // get rid of scope
    clause_scope.pop_all();

    // add the clause to the non-current list and
    // remove it from the current list
    cl[(current+1)%2]->push_back ( *i );
    cl[current]->erase ( i );

    len--;

  }

}

void debugger_scheduler  ( Program * p, Scope * s ) {

  CLAUSE_LIST * cl = new CLAUSE_LIST;
  CLAUSE_LIST::iterator i;
  Scope clause_scope;
  std::list<SymbolTable *>::reverse_iterator k;

  p->make_clause_list ( cl, s );
  p->init_params ( s );
  p->init ( s );

  i = cl->begin();

  while ( 1 ) {

    if ( i == cl->end() )
      i = cl->begin();

    // set up scope
    for ( k = (*i)->second->rbegin(); k != (*i)->second->rend(); k++ ) 
      clause_scope.push ( *k );

    // execute the clause
    (*i)->first->debug_execute ( &clause_scope );

    // get rid of scope
    clause_scope.pop_all();

    i++;

  }

}
