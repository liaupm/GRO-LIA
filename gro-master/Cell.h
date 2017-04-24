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

#ifndef CELL_H
#define CELL_H

//#include "chipmunk_private.h"
//#include "chipmunk_unsafe.h"
#include "CellEngine.h"
#include "CellNutrient.h"
#include "cg/Genome.hpp"
#include "ccl.h"
#include "Defines.h"
#include "Utility.h"

#ifndef NOGUI
#include "Theme.h"
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

#define UNDIRECTED 0
#define DIRECTED 1


class World;
class MicroProgram;


////////////////////////////////////////////////////////////////////////////////
// Cell
//

class Cell {

 public:

  Cell ( World * w );
  ~Cell ( void );

  void init ( const int * q0, const int * rep0, float frac );

  inline void set ( int i, int x ) { q[i] = x; }
  inline int get ( int i ) { return q[i]; }
  inline int * get_q ( void ) { return q; }
  inline int get_rep ( int i ) { return rep[i]; }

  inline void inc ( int i, int x ) { q[i] += x; }
  inline void dec ( int i, int x ) { q[i] -= x; }

  inline void inc ( int i ) { q[i]++; }
  inline void dec ( int i ) { q[i]--; }

  inline void set_rep ( int i, int x ) { rep[i] = x; }

  inline void select ( void ) { selected = true; }
  inline void deselect ( void ) { selected = false; }
  inline bool is_selected ( void ) { return selected; }

  void set_prog ( MicroProgram * p ) { program = p; }

  float get_x ( void ) { return body->center.x; }
  float get_y ( void ) { return body->center.y; }
  float get_vec_x ( void ) { return body->halfDimensionAABB.x; }
  float get_vec_y ( void ) { return body->halfDimensionAABB.y; }
  float get_theta ( void ) { return body->rotation; }

  virtual void update ( void ) {}
  virtual Value * eval ( Expr * ) { return NULL; }

#ifndef NOGUI
  virtual void render ( Theme *, GroPainter * ) {}
#endif

  virtual Cell * divide ( void ) { return NULL; }

  ceBody * get_body ( void ) { return body; }

  void set_id ( int i ) { id = i; }
  int get_id ( void ) { return id; }

  virtual float get_fluorescence ( int i ) { return rep[i]; }

  virtual float get_size ( void ) { return 1; }
  virtual float get_volume ( void ) { return 0.000001; }
  virtual float get_length ( void ) {}

  Program * get_gro_program ( void ) { return gro_program; }
  void set_gro_program ( Program * p ) { gro_program = p; }

  // getters and setters for parameters ////////////
  inline void set_param ( std::string str, float val ) { parameters[str] = val; }
  inline float get_param ( std::string str ) { return parameters[str]; }
  inline std::map<std::string,float> get_param_map ( void ) { return parameters; }
  inline void set_param_map ( std::map<std::string,float> p ) { parameters = p; }

  virtual void compute_parameter_derivatives ( void ) {  } // To avoid computing simple functions of parameters
                                                                 // over and over again, this function is called whenever gro
                                                                 // changes a parameter
  // end parameters ///////////////////////////////

  inline void mark_for_death ( void ) { marked = true; }
  inline bool marked_for_death ( void ) { return marked; }

  inline void set_division_indicator ( bool val ) { divided = val; }
  inline void set_daughter_indicator ( bool val ) { daughter = val; }

  inline bool just_divided ( void ) { return divided; }
  inline bool is_daughter ( void ) { return daughter; }


  /*void jc_reset ( void );
  void set_was_just_conjugated ( int, bool );
  bool was_just_conjugated ( int ) const;
  void set_conjugated_indicator ( int, bool );
  bool get_conjugated_indicator ( int ) const;*/

  //inline float get_d_vol ( void ) { return d_vol; }
  inline float get_d_length ( void ) { return d_length; }
  inline float get_available( void ) { return my_available; }

  inline float get_input_cf_coef() { return cross_input_coefficient;}
  inline void set_input_cf_coef(float v) {this->cross_input_coefficient = v;}

  inline float get_output_cf_coef() { return cross_output_coefficient;}
  inline void set_output_cf_coef(float v) {this->cross_output_coefficient = v;}

  /*inline bool was_just_conjugated ( int n ) { return just_conjugated_reset[n]; }
  inline void set_conjugated_indicator ( int n, bool val ) { just_conjugated[n] = val; }*/

  virtual void force_divide ( void ) {}

  /*void markForConjugation (int , int mode);
  void conjugate(int, double);
  void transfer_plasmid(int);
  void conjugate_directed(int, double);*/

  //void print_state();

  float get_gt_inst(void);

  int getNProts(void);
  void setNProts(int);

  /*void set_initial_protein(int, bool);
  void set_initial_last_state(void);

  void set_initial_rna(int, bool);*/

  /*void set_analog_molecule(int i, int val) {analog_molecules_list[i]=val;}
  bool get_analog_molecule(int i) {return analog_molecules_list[i];}*/

  int check_gen_condition(std::vector<uint64_t>);
  int check_plasmid_condition(std::vector<std::pair<std::string, int>>);

  inline cg::Genome& getGenome(void) {return genome;}

  void reset_signal_prot(std::string prot);
  void reset_signal_prots();

  void conjugate(std::string, double, int);

  void check_action();

  float get_n_cells_d(float);
  float area_concentration(int, float);

  void paint_from_list(std::list<std::string> ls);
  void conjugate_from_list(std::list<std::string> ls);
  void conjugate_directed_from_list(std::list<std::string> ls);
  void lose_plasmid_from_list(std::list<std::string> ls);
  void set_eex_from_list(std::list<std::string> ls);
  void remove_eex_from_list(std::list<std::string> ls);
  void die_from_list(std::list<std::string> ls);
  void conj_and_paint_from_list(std::list<std::string> ls);
  void delta_paint_from_list(std::list<std::string> ls);
  void change_gt_from_list(std::list<std::string> ls);
  void emit_cross_feeding_signal_from_list(std::list<std::string> ls);
  void get_cross_feeding_signal_from_list(std::list<std::string> ls);

  void s_absorb_signal_area(std::list<std::string> ls);
  void s_absorb_signal(std::list<std::string> ls);
  void s_emit_signal_area(std::list<std::string> ls);
  void s_emit_signal(std::list<std::string> ls);
  void s_get_signal_area(std::list<std::string> ls);
  void s_get_signal(std::list<std::string> ls);
  void s_absorb_QS(std::list<std::string> ls);
  void s_get_QS(std::list<std::string> ls);
  /*void s_set_signal_multiple(std::list<std::string> ls);*/
  void s_set_signal(std::list<std::string> ls);
  void s_set_signal_rect(std::list<std::string> ls);
  void s_emit_cross_feeding_signal_from_list(std::list<std::string> ls);
  void s_get_cross_feeding_signal_from_list(std::list<std::string> ls);
  void s_absorb_cross_feeding_signal_from_list(std::list<std::string> ls);

  void state_to_file(FILE *fp, std::vector<std::string>);
  void state_to_file_reduced(FILE *fp, std::vector<std::string>);

  inline void add_eex(std::string name)
  {
      const cg::Plasmid* to_eex= genome.getPlasmidPool()->getPlasmidByName(name);

      if(to_eex != nullptr)
      {
            excluded.insert(std::make_pair(to_eex->getName(), to_eex));
      }
  }

  inline void remove_eex(std::string name)
  {
      excluded.erase(name);
  }

  inline bool is_eex(std::string name)
  {
      return(excluded.find(name) != excluded.end());
  }

 protected:

  Cell(World* w, cg::Genome& g, bool mut);

  ceSpace * space;
  ceBody * body;

  int q[MAX_STATE_NUM],
    rep[MAX_REP_NUM];

  World * world;
  MicroProgram * program;
  Program * gro_program;
  int id;

  // these ought to be gotten rid of
  float growth_rate,
  old_growth_rate,
  division_size_mean,
  division_size_variance;

  int num_analog_molecules;
  int n_prots;

  cg::Genome genome;

  std::map<std::string, const cg::Plasmid*> excluded;

  /*int num_analog_molecules;*/

  bool * analog_molecules_list; // Que moleculas se leen desde la bacteria... esto esta pensado para senales analogicas

  bool marked, divided, daughter, selected;
  //bool markForConj;

  float gt_inst;

  std::map<std::string,float> parameters;

  //float in_vol, gt_inst;
  //float in_vol, d_vol;
  float d_length;
  float in_vol;
  float monod;
  float my_available;

  float cross_output_coefficient;
  float cross_input_coefficient;

};

#endif // CELL_H
