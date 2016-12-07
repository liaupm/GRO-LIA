#include <math.h>
#include <stdlib.h>
#include "SymbolTable.h"
#include "parser.h"
#include "Normal.h"
#include "Hill.h"
#include "Rands.h"

#ifdef MAC
 #include <time.h>
#endif

#ifdef WIN
 #include <time.h>
#endif


///////////////////////////////////////////////////////////////////////////////////////
// MATH
//

#define MATH_FUNC(_cclfunc_,_func_)                                       \
Value * _cclfunc_ ( std::list<Value *> * args, Scope * ) {          \
  return new Value ( _func_ ( (*args->begin())->num_value() ) );          \
} 

MATH_FUNC ( ccl_sin, sin )
MATH_FUNC ( ccl_cos, cos )
MATH_FUNC ( ccl_tan, tan )
MATH_FUNC ( ccl_asin, asin )
MATH_FUNC ( ccl_acos, acos )

Value * ccl_atan ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  double x = (*i)->num_value();
  i++;
  double y = (*i)->num_value();
  return new Value ( atan2 ( y, x ) );
}

Value * ccl_pow ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  double x = (*i)->num_value();
  i++;
  double y = (*i)->num_value();
  return new Value ( pow ( x, y ) );
}

MATH_FUNC ( ccl_sinh, sinh )
MATH_FUNC ( ccl_cosh, cosh )
MATH_FUNC ( ccl_tanh, tanh )
MATH_FUNC ( ccl_asinh, asinh )
MATH_FUNC ( ccl_acosh, acosh )
MATH_FUNC ( ccl_atanh, atanh )
MATH_FUNC ( ccl_log, log )
MATH_FUNC ( ccl_log10, log10 )
MATH_FUNC ( ccl_exp, exp )
MATH_FUNC ( ccl_ceil, ceil )
MATH_FUNC ( ccl_floor, floor )

Value * ccl_ceil_int ( std::list<Value *> * args, Scope * ) {
  double v = (*args->begin())->num_value();
  int c = 0;
  c = (int) (ceil(v));
  return new Value ( c );
}

Value * ccl_floor_int ( std::list<Value *> * args, Scope * ) {
   double v = (*args->begin())->num_value();
   int f = 0;
   f = (int) (floor(v));
   return new Value ( f );
}

MATH_FUNC ( ccl_sqrt, sqrt )

Value * ccl_rand ( std::list<Value *> * args, Scope * ) {
  int max = (int) (*args->begin())->num_value();
  if ( max < 1 ) 
    max = 1;
  return new Value ( rand() % max );
}

Value * ccl_srand ( std::list<Value *> * args, Scope * ) {

  int seed = (int) (*args->begin())->num_value();

  if ( seed <= 0 ) 
    srand ( (unsigned int) time ( NULL ) / 2 );
  else
    srand ( seed );

  return new Value ( Value::UNIT );

}

Value * ccl_nrand ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  float mu = (*i)->num_value();
  i++;
  float sigma = (*i)->num_value();
  return new Value ( gaussian_random(mu, sigma) );
}

Value * ccl_uneven_nrand ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  float mu = (*i)->num_value();
  i++;
  float sigma_left = (*i)->num_value();
  i++;
  float sigma_right = (*i)->num_value();
  return new Value ( uneven_gaussian(mu, sigma_left, sigma_right) );
}

Value * ccl_frand ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  double fmin = (*i)->num_value();
  i++;
  double fmax = (*i)->num_value();
  return new Value ( fRand(fmin, fmax) );
}

Value * ccl_hill ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  float Ka = (*i)->num_value();
  i++;
  float L = (*i)->num_value();
  i++;
  float n = (*i)->num_value();
  return new Value ( hill(Ka, L, n) );
}

Value * ccl_conc_pct_step ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  float max_conc = (*i)->num_value();
  i++;
  float pct = (*i)->num_value();
  i++;
  float current_conc = (*i)->num_value();
  return new Value ( conc_pct_step(max_conc, pct, current_conc) );
}

Value * ccl_time_pct_step ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  float tmax = (*i)->num_value();
  i++;
  float pct = (*i)->num_value();
  i++;
  float current_time = (*i)->num_value();
  return new Value ( time_pct_step(tmax, pct, current_time) );
}

Value * ccl_step ( std::list<Value *> * args, Scope * ) {
  std::list<Value *>::iterator i = args->begin();
  float current = (*i)->num_value();
  i++;
  float threshold = (*i)->num_value();
  return new Value ( step(current, threshold) );
}



///////////////////////////////////////////////////////////////////////////////////////
// STANDARD
//

Value * ccl_print ( std::list<Value *> * args, Scope * s ) {

  std::list<Value *>::iterator i;

  for ( i=args->begin(); i!=args->end(); i++ ) {

    if ( (*i)->get_type() == Value::STRING ) 
      printf ( (*i)->string_value().c_str() );
    else
      (*i)->print(stdout);

  }
  
  fflush ( stdout );

  return new Value(Value::UNIT);

}

Value * ccl_exit ( std::list<Value *> * args, Scope * s ) {

  exit(0);
  return new Value ( Value::UNIT );

}

Value * ccl_atoi ( std::list<Value *> * args, Scope * s ) {

  return new Value ( atoi ( (*args->begin())->string_value().c_str() ) );

}

Value * ccl_atof ( std::list<Value *> * args, Scope * s ) {

  return new Value ( atof ( (*args->begin())->string_value().c_str() ) );

}

Value * ccl_tostring ( std::list<Value *> * args, Scope * s ) {

  std::string str;
  (*args->begin())->print_to_string ( &str );
  return new Value ( str.c_str() );

}

Value * ccl_map ( std::list<Value *> * args, Scope * s ) {

  std::list<Value *>::iterator i = args->begin();

  Value * f = *i;
  i++;
  std::list<Value *> * L = (*i)->list_value(), 
                * result = new std::list<Value *>;
  Expr * e;

  for ( i = L->begin(); i != L->end(); i++ ) {

    e = APP ( new Expr ( f->copy() ), new Expr ( (*i)->copy() ) );
    result->push_back ( e->eval(s) );
    delete e;

  }

  return new Value ( result );

}

Value * ccl_sumlist ( std::list<Value *> * args, Scope * s ) {

    std::list<Value *>::iterator i = args->begin();

    std::list<Value *> * L = (*i)->list_value(),
            * result = new std::list<Value *>;

    float sum = 0;

    for ( i = L->begin(); i != L->end(); i++ ) {

        if ( (*i)->get_type() == Value::INTEGER || (*i)->get_type() == Value::REAL )
            sum += (*i)->num_value();

    }

    return new Value ( sum );

}

Value * ccl_length ( std::list<Value *> * args, Scope * s ) {

  return new Value ( (int) (*args->begin())->list_value()->size() );

}
  
Value * ccl_skip ( std::list<Value *> * args, Scope * s ) {

  return new Value ( Value::UNIT );

}


static bool init = false;

void RegisterCCLInternals ( void ) {

    // math
    register_ccl_function ( "sin", ccl_sin );
    register_ccl_function ( "cos", ccl_cos );
    register_ccl_function ( "tan", ccl_tan );

    register_ccl_function ( "asin", ccl_asin );
    register_ccl_function ( "acos", ccl_acos );
    register_ccl_function ( "atan", ccl_atan );

    register_ccl_function ( "sinh", ccl_sinh );
    register_ccl_function ( "cosh", ccl_cosh );
    register_ccl_function ( "tanh", ccl_tanh );

    register_ccl_function ( "asinh", ccl_asinh );
    register_ccl_function ( "acosh", ccl_acosh );
    register_ccl_function ( "atanh", ccl_atanh );

    register_ccl_function ( "log", ccl_log );
    register_ccl_function ( "log10", ccl_log10 );
    register_ccl_function ( "exp", ccl_exp );
    register_ccl_function ( "pow", ccl_pow );
    register_ccl_function ( "ceil", ccl_ceil );
    register_ccl_function ( "floor", ccl_floor );
    register_ccl_function ( "ceil_int", ccl_ceil_int );
    register_ccl_function ( "floor_int", ccl_floor_int );
    register_ccl_function ( "sqrt", ccl_sqrt );

    register_ccl_function ( "rand", ccl_rand );
    register_ccl_function ( "srand", ccl_srand );
    register_ccl_function ( "nrand", ccl_nrand );
    register_ccl_function ( "unrand", ccl_uneven_nrand );
    register_ccl_function ( "frand", ccl_frand );
    register_ccl_function ( "hill", ccl_hill );
    register_ccl_function ( "cpstep", ccl_conc_pct_step );
    register_ccl_function ( "tpstep", ccl_time_pct_step );
    register_ccl_function ( "step", ccl_step );

    // standard
    register_ccl_function ( "cprint", ccl_print );
    register_ccl_function ( "exit", ccl_exit );
    register_ccl_function ( "atoi", ccl_atoi );
    register_ccl_function ( "atof", ccl_atof );
    register_ccl_function ( "tostring", ccl_tostring );
    register_ccl_function ( "map", ccl_map );
    register_ccl_function ( "length", ccl_length );
    register_ccl_function ( "skip", ccl_skip );
    register_ccl_function ( "sumlist_aux", ccl_sumlist );
    
}

