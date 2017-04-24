/////////////////////////////////////////////////////////////////////////////////////////
//
// gro is protected by the UW OPEN SOURCE LICENSE, which is summarized here.
// Please see the file LICENSE.txt for the complete license.
//
// THE SOFTWARE (AS DEFINED BELOW) AND HARDWARE DESIGNS (AS DEFINED BELOW) IS PROVIDED
// UNDER THE TERMS OF THIS OPEN SOURCE LICENSE (“LICENSE”).  THE SOFTWARE IS PROTECTED
// BY COPYRIGHT AND/OR OTHER APPLICABLE LAW.  ANY USE OF THIS SOFTWARE OTHER THAN AS
// AUTHORIZED UNDER THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
//
// BY EXERCISING ANY RIGHTS TO THE SOFTWARE AND/OR HARDWARE PROVIDED HERE, YOU ACCEPT AND
// AGREE TO BE BOUND BY THE TERMS OF THIS LICENSE.  TO THE EXTENT THIS LICENSE MAY BE
// CONSIDERED A CONTRACT, THE UNIVERSITY OF WASHINGTON (“UW”) GRANTS YOU THE RIGHTS
// CONTAINED HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.
//
// TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
//
//

#ifndef _ECOLI_H_
#define _ECOLI_H_

#include "Micro.h"
extern float consumido;
extern float available;

class EColi : public Cell {

 public:
  EColi ( World * w, float x, float y, float a );
  EColi ( World * w, float x, float y, float a, float div_mean, float div_variance);
  EColi ( World * w, ceBody* body , cg::Genome* genome );
  EColi ( World * w, ceBody* body , cg::Genome* genome, float div_mean, float div_variance);

#ifndef NOGUI
  void render ( Theme * theme, GroPainter * painter );
#endif

  void update ( void );
  Value * eval ( Expr * e );
  EColi * divide ( void );

  void compute_parameter_derivatives ( void );

  float get_fluorescence ( int i ) { return (float) get_rep(i) / volume; }
  float get_length ( void ) { return volume / ( 0.25 * PI * DEFAULT_ECOLI_DIAMETER * DEFAULT_ECOLI_DIAMETER ); }

  float get_volume ( void ) { 
    //return volume;
      return length;
  }

  void force_divide ( void ) { force_div = true; }
  bool get_force_divide ( void ) { return force_div; }
  void set_force_divide ( bool v ) { force_div = v; }

  int get_div_count ( void ) { return div_count; }
  void set_div_count ( int n ) { div_count = n; }
  
 private:
  //float volume, lambda, div_vol, div_length, init_length, length, d_length;
  float volume, lambda, div_vol, div_length, init_length, length;
  float sd, sm;
  int div_count;
  bool force_div;

};

#endif
