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

#ifndef _MICRO_H_
#define _MICRO_H_

#ifndef NOGUI
#include "GroPainter.h"
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <list>
#include <vector>
#include <string>
#include <map>

#include <stdint.h>

#include "ccl.h"


#include "Cell.h"
#include "Defines.h"
#include "Utility.h"

#include "Grid.h"

#ifndef NOGUI
#include "Theme.h"
#endif

#include "erreenege.h"
#include "GenListPlasmid.h"

//void set_throttle ( bool val );
void register_gro_functions ( void );


////////////////////////////////////////////////////////////////////////////////
// Signal
//

class Signal {

 public:

  //Signal ( cpVect p, cpVect q, int nx, int ny, float kdi, float kde );
  Signal ( ceVector2 p, ceVector2 q, int nx, int ny, float kdi, float kde );
  ~Signal ( void );

  inline int row ( float x ) { return ( (int) floor ( (x-minp.x)/dw ) ); }
  inline int col ( float y ) { return ( (int) floor ( (y-minp.y)/dh ) ); }
  inline float max ( float a, float b ) { return a<b?b:a; }
  void set ( int i, int j, float c );
  void set ( float x, float y, float c );
  void set_rect ( float x1, float y1, float x2, float y2, float c );
  void inc ( int i, int j, float c );
  void inc ( float x, float y, float c );
  void dec ( int i, int j, float c );
  void dec ( float x, float y, float c );
  float get ( int i, int j );
  float get ( float x, float y );
  void zero ( void );

  void integrate ( float dt );

  inline int get_numx ( void ) { return numx; }
  inline int get_numy ( void ) { return numy; }
  inline ceVector2 get_minp ( void ) { return minp; }
  inline ceVector2 get_maxp ( void ) { return maxp; }
  //inline cpVect get_minp ( void ) { return minp; }
  //inline cpVect get_maxp ( void ) { return maxp; }
  inline float get_dw ( void ) { return dw; }
  inline float get_dh ( void ) { return dh; }
  inline float get_val ( int i, int j ) { return sig[i][j]; }

  inline std::vector< std::vector<float> > * get_signal_matrix ( void ) {
      return &sig;
  }

 private:

  int numx, numy;
  //cpVect minp, maxp;
  ceVector2 minp, maxp;
  float w, h, dw, dh;
  float kdiff, kdeg;
  std::vector< std::vector<float> > sig, dsig;

};

class Reaction {

public:

    Reaction(float k);
    void add_reactant ( int i );
    void add_product ( int i );
    void integrate ( std::vector<Signal *> * signal_list, int i, int j, float dt );

private:

    std::vector<int> reactants, products;
    float rate;

};

////////////////////////////////////////////////////////////////////////////////
// Program
//

class MicroProgram {

 public:

  MicroProgram ( void ) {}
  virtual void init ( World * ) {}
  virtual void update ( World *, Cell * ) {}
  virtual Value * eval ( World * , Cell * , Expr * ) { return NULL; }
  virtual void world_update ( World * ) {}
  virtual void destroy ( World * ) {}
  virtual std::string name ( void ) const { return "Untitled Program"; }
  
 private:

};

////////////////////////////////////////////////////////////////////////////////
// Messages
//

class MessageHandler {

 public:

  MessageHandler ( void );
  ~MessageHandler ( void );

  void add_message ( int i, const char * str );
  void clear_messages ( int i ); // clears a quadrant
  void clear_messages_all ( void );  // clears all quadrants

#ifndef NOGUI
  void render ( GroPainter * painter );
#endif

 private:

  std::list<std::string> message_buffer[4];

};

////////////////////////////////////////////////////////////////////////////////
// World
//

struct Barrier {

    float x1, y1, x2, y2;

};

class GroThread;

typedef void (Cell::*FnPointer)(std::list<std::string>);

class World { 

 public:

#ifdef NOGUI
  World ( void );
#else
  World ( GroThread * ct );
#endif

  ~World ( void );

  void set_program ( MicroProgram * p ) { prog = p; }
  MicroProgram * get_program ( void ) { return prog; }

  void init ();
  void restart ( void );
  void replating (float);
  void update ();

#ifndef NOGUI
  void render ( GroPainter * painter );
#endif

  void add_cell ( Cell * c );

  void add_signal ( Signal * s );
  void add_reaction ( Reaction r ) { reaction_list.push_back(r); }
  float get_signal_value ( Cell * c, int i );
  void emit_signal ( Cell * c, int i, float ds );
  void absorb_signal ( Cell * c, int i, float ds );
  std::vector< std::vector<float> > * get_signal_matrix ( int i );
  int num_signals ( void ) { return signal_list.size(); }
  inline void set_signal ( int i, float x, float y, float c ) { signal_list[i]->set(x,y,c); }
  inline void set_signal_rect ( int i, float x1, float y1, float x2, float y2, float c ) { signal_list[i]->set_rect(x1,y1,x2,y2,c); }

  //inline cpSpace * get_space ( void ) { return space; }
  inline ceSpace * get_space ( void ) { return space; }

  void set_chemostat_mode ( bool mode ) { chemostat_mode = mode; }
  void toggle_chemostat_mode ( void ) { chemostat_mode = chemostat_mode ? false : true; }
  bool get_chemostat_mode ( void ) { return chemostat_mode; }
  //cpVect chemostat_flow ( float, float y, float mag );
  ceVector2 chemostat_flow ( float, float y, float mag );

  void print ( void );
  void set_print_rate ( int r ) { print_rate = r; }

  void create_dirs ( const char * path, int n );
  bool snapshot ( const char * path );
  void set_movie_rate ( int r ) { movie_rate = r; }

  void histogram  ( float x, float y, float width, float height, int channel );
  void scatter ( float x, float y, float width, float height, int channel1, int channel2 );

  float get_time ( void ) { return t; }

  int get_pop_size ( void ) { return population->size(); }
  void randomize_population ( void );

  void message ( int i, std::string str ) { message_handler.add_message ( i, str.c_str() ); }
  void clear_messages ( int i ) { message_handler.clear_messages(i); }

  void select_cells ( int x1, int y1, int x2, int y2 );
  void deselect_all_cells ( void );
  void select_random_cell ( void );

  inline void  set_sim_dt ( float x ) { parameters["dt"] = x; }
  inline float get_sim_dt ( void ) { return parameters["dt"]; }

  inline void  set_chip_dt ( float x ) { chip_dt = x; }
  inline float get_chip_dt ( void ) { return chip_dt; }

  inline void set_param ( std::string str, float val ) { parameters[str] = val;
    //
    //for( std::map<std::string,float>::iterator ii = parameters.begin(); ii != parameters.end(); ++ii ) {
    // printf ( "%s -> %f\n", (*ii).first.c_str(), (*ii).second );
    //}

  }

  inline bool initialized (void ) { return program_initialized; }

  // note that if the parameter doesn't exist, the [] operator will insert it
  // with a default value, which I assume would be 0.0 for a float.
  inline float get_param ( std::string str ) { return parameters[str]; }

  inline std::map<std::string,float> get_param_map ( void ) { return parameters; }

  Value * map_to_cells ( Expr * e );

  void set_theme ( Value * v );

  void set_zoom ( float z ) { zoom = z; } //;

#ifndef NOGUI
  Theme * get_theme ( void ) { return &theme; }
#endif

  void emit_message ( std::string str, bool clear = false );

  void set_stop_flag ( bool f ) { stop_flag = f; }
  bool get_stop_flag ( void ) { bool b = stop_flag; stop_flag = false; return b; }

  std::vector<FILE *> fileio_list;

  int check_gen_cond_pop ( std::vector<std::pair<std::string, int>> );
  int check_plasmid_cond_pop ( std::vector<std::pair<std::string, int>> );

  void dump ( FILE * fp );
  float dump_left ( void );
  float dump_right ( void );
  float dump_top ( void );
  float dump_bottom ( void );

  void add_barrier ( float x1, float y1, float x2, float y2 );


  //CORREGIR TODAS LAS FUNCIONES DE ACCION
  void add_action_prot(std::map<std::string, int> b){action_prot_list.push_back(b);}
  void add_action_param( std::list<std::string> ls){action_param_list.push_back(ls);}
  void add_action_names ( std::string s ){action_names.push_back (s);}

  void remove_all_actions();

  std::map<std::string, int> get_action_prot(int f){return action_prot_list[f];}
  std::string get_action_name(int i) {return action_names[i];}
  std::list<std::string> get_action_param( int i){return action_param_list[i];}

  int get_num_actions ( void ) { return num_actions; }
  void set_num_actions ( int n ) { num_actions=n; }

  FnPointer get_action (std::string);

  inline void init_actions_map()
  {
      map_actions.insert(std::make_pair("paint",&Cell::paint_from_list));
      map_actions.insert(std::make_pair("d_paint",&Cell::delta_paint_from_list));
      map_actions.insert(std::make_pair("die",&Cell::die_from_list));
      map_actions.insert(std::make_pair("conjugate",&Cell::conjugate_from_list));
      map_actions.insert(std::make_pair("conjugate_directed",&Cell::conjugate_directed_from_list));
      //map_actions.insert(std::make_pair("c_and_p",&Cell::conj_and_paint_from_list));
      map_actions.insert(std::make_pair("lose_plasmid",&Cell::lose_plasmid_from_list));
      map_actions.insert(std::make_pair("set_eex",&Cell::set_eex_from_list));
      map_actions.insert(std::make_pair("remove_eex",&Cell::remove_eex_from_list));
      map_actions.insert(std::make_pair("set_growth_rate",&Cell::change_gt_from_list));
      map_actions.insert(std::make_pair("emit_cf",&Cell::emit_cross_feeding_signal_from_list));
      map_actions.insert(std::make_pair("get_cf",&Cell::get_cross_feeding_signal_from_list));
      map_actions.insert(std::make_pair("s_absorb_signal_area", &Cell::s_absorb_signal_area));
      map_actions.insert(std::make_pair("s_absorb_signal", &Cell::s_absorb_signal));
      map_actions.insert(std::make_pair("s_emit_signal_area", &Cell::s_emit_signal_area));
      map_actions.insert(std::make_pair("s_emit_signal", &Cell::s_emit_signal));
      map_actions.insert(std::make_pair("s_get_signal", &Cell::s_get_signal));
      map_actions.insert(std::make_pair("s_get_signal_area", &Cell::s_get_signal_area));
      map_actions.insert(std::make_pair("s_absorb_QS", &Cell::s_absorb_QS));
      map_actions.insert(std::make_pair("s_get_QS", &Cell::s_get_QS));
      //map_actions.insert(std::make_pair("s_set_signal_multiple", &Cell::s_set_signal_multiple));
      map_actions.insert(std::make_pair("s_set_signal", &Cell::s_set_signal));
      map_actions.insert(std::make_pair("s_set_signal_rect", &Cell::s_set_signal_rect));
      map_actions.insert(std::make_pair("s_emit_cf",&Cell::s_emit_cross_feeding_signal_from_list));
      map_actions.insert(std::make_pair("s_get_cf",&Cell::s_get_cross_feeding_signal_from_list));
      map_actions.insert(std::make_pair("s_absorb_cf",&Cell::s_absorb_cross_feeding_signal_from_list));

  }

  bool get_output_started();
  void set_output_started(bool);
  bool get_output_started2();
  void set_output_started2(bool);

  inline GenListPlasmid* get_globalPlasmidList() {return plasmidList;}
  inline GenPlasmid* get_globalPlasmid() {return globalPlasmid;}
  inline GenOperon* get_globalOperon() {return globalOperon;}

  inline erreenege* getRNG(){return(this->rng);}
  inline void setRNG(erreenege* r){this->rng = r;}

  inline void add_cross_feeding_max_emit(int id, float conc)
  {
      cross_feeding_max_emit.insert(std::make_pair(id,conc));
  }

  inline float get_cross_feeding_max_emit(int id)
  {
      return(cross_feeding_max_emit[id]);
  }

  inline void set_cross_feeding_max_emit(int id, float v)
  {
      cross_feeding_max_emit[id] = v;
  }

  inline void add_cross_feeding_max_absorb(int id, float conc)
  {
      cross_feeding_max_absorb.insert(std::make_pair(id,conc));
  }

  inline float get_cross_feeding_max_absorb(int id)
  {
      return(cross_feeding_max_absorb[id]);
  }

  inline void set_cross_feeding_max_absorb(int id, float v)
  {
      cross_feeding_max_absorb[id] = v;
  }

  Grid<CSCell>* handler = nullptr;
  Grid<DBCell>* dhandler = nullptr;
  //std::map<int, std::string> diff_methods;
  //std::vector<GReaction> reactions;
  std::string grid_type = "";
  std::string diff_method = "";
  float digital_prob = 0.0;
  int digital_prop = 0;
  std::map<int,double> signal_concs;
  int s_signal_id;
  std::map<int,float> cross_feeding_max_emit;
  std::map<int,float> cross_feeding_max_absorb;

 private:

  //cpSpace * space;
  ceSpace * space;
  signalGrid_t * signalGrid;
  std::list<Cell *> * population;
  std::vector<Signal *> signal_list;
  std::vector<Reaction> reaction_list;
  MicroProgram * prog;
  bool chemostat_mode;
  int next_id;
  int step;
  int print_rate, movie_rate;
  float t;
  float max_val;
  bool program_initialized;
  bool program_restarted;
  std::string gro_message;
  MessageHandler message_handler;
  float zoom;
  std::list<Barrier> * barriers;

  std::vector<std::map<std::string, int> > action_prot_list;
  std::vector<std::list<std::string> > action_param_list; //lista de parametros de funciones
  std::vector<std::string> action_names;

  typedef std::map<std::string,FnPointer> actions; //mapa de nombres de funciones
  actions map_actions;

  int num_actions;

  //CellPro
  erreenege *rng;
  GenListPlasmid *plasmidList;
  GenPlasmid* globalPlasmid;
  GenOperon* globalOperon;
  GenPromoter* globalPromoter;

  int stt_time;

  bool output_started, output_started2;

#ifndef NOGUI
  Theme theme;
#endif

  float chip_dt;
  std::map<std::string,float> parameters;

#ifndef NOGUI
  GroThread * calling_thread;
#endif

  bool stop_flag;

};

#endif
