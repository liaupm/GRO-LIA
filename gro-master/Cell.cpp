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

#ifndef NOGUI
#include <GroThread.h>
#endif

#include "Micro.h"
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "Rands.h"
#include "CESpace.h"


static int max_id = 0;

Cell::Cell ( World * w ) : world ( w ), gro_program(NULL), marked(false), selected(false), genome(w->getPlasmidPool()) {

  parameters = w->get_param_map();
  compute_parameter_derivatives();
  w->init_actions_map();

  program = w->get_program();
  space = w->get_space();
  body = NULL;

  int i;
  for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = 0;
  for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = 0;

  divided = false;
  daughter = false;

  cross_input_coefficient = 1.0;
  cross_output_coefficient = 1.0;

  n_prots = 0;

  set_id ( max_id++ );

}

Cell::Cell(World* w, cg::Genome& g, bool mut) : world (w), genome(g, mut), gro_program(NULL), marked(false), selected(false)
{
    parameters = w->get_param_map();
    compute_parameter_derivatives();
    w->init_actions_map();

    program = w->get_program();
    space = w->get_space();
    body = NULL;

    int i;
    for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = 0;
    for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = 0;

    divided = false;
    daughter = false;

    cross_input_coefficient = 1.0;
    cross_output_coefficient = 1.0;

    n_prots = 0;

    set_id ( max_id++ );
}

Cell::~Cell ( void ) {

  ceDestroyBody(get_body());
  if ( gro_program != NULL ) {
    delete gro_program;
  }

}

float Cell::get_gt_inst()
{
    return this->gt_inst;
}

int Cell::getNProts()
{
    return this->n_prots;
}

void Cell::setNProts(int v)
{
    this->n_prots = v;
}

void Cell::reset_signal_prots()
{
    for(auto x : *(world->get_signal_prots()))
    {
        genome.setProteinState(genome.getPlasmidPool()->getProteinByName(x),false);
    }
}

void Cell::reset_signal_prot(std::string prot)
{
    genome.setProteinState(genome.getPlasmidPool()->getProteinByName(prot),false);
}

void Cell::conjugate(std::string name, double n_conj, int mode)
{
    double r = fRand(0.00000000000000, 1.000000000000000);
    double p = 0;
    unsigned int size;
    int nc = 4;
    int target;
    int seen = 0;
    double dete = world->get_sim_dt();
    ceBody** neighbors = ceGetNearBodies(this->body,0.5,&size);

    const cg::Plasmid* toConj;
    const std::vector<const cg::Plasmid*>& plasmids = genome.getPlasmids();

    for(auto it : plasmids)
    {
        if(it->getName() == name)
        {
            toConj = it;

            if(size < nc)
            {
                p = ((n_conj * dete)/(this->gt_inst))*((double)((double)size/(double)nc));
            }
            else
            {
                p = ((n_conj * dete)/(this->gt_inst));
            }
            if(r < p && mode == UNDIRECTED)
            {
                target = rand()%size;
                if(!(((Cell*)(neighbors[target]->data))->marked_for_death()) &&
                        !(this->marked_for_death()) &&
                        !((Cell*)(neighbors[target]->data))->is_eex(name))
                {
                    ((Cell*)(neighbors[target]->data))->getGenome().add(toConj);
                    world->inc_conj_count(toConj);
                }
                free(neighbors);
            }

            else if(r < p && mode == DIRECTED)
            {
                target = rand() % size;
                seen++;
                while((
                          !(((Cell*)(neighbors[target]->data))->marked_for_death()) &&
                          !(this->marked_for_death()) && toConj != NULL &&
                          (((Cell*)(neighbors[target]->data))->is_eex(name))) &&
                          seen < size)
                {
                    target = (target + 1) % size;
                    seen++;
                }
                if(!(((Cell*)(neighbors[target]->data))->marked_for_death()) &&
                        !(this->marked_for_death()) && toConj != NULL &&
                        !(((Cell*)(neighbors[target]->data))->is_eex(name)))
                {
                    ((Cell*)(neighbors[target]->data))->getGenome().add(toConj);
                    world->inc_conj_count(toConj);
                }
                free(neighbors);
            }
            return;
        }
    }
}

//Idea: Ingresar numero de senal como parametro, de modo que puedo seleccionar quienes son las bacterias en la zona que no están infectadas por fago X.
float Cell::get_n_cells_d(float d_in_microns)
{
    float d_in_pixels = d_in_microns * DEFAULT_ECOLI_SCALE;
    float n_neighbors = 0;
    unsigned int size;
    ceVector2 area_origin = ceGetVector2(this->body->center.x - (d_in_pixels), this->body->center.y - (d_in_pixels));
    ceVector2 area_end = ceGetVector2(2*d_in_pixels, 2*d_in_pixels);
    ceBody ** neighbors = ceGetBodies(world->get_space(), &size, area_origin, area_end );
    //IDEA: Recorrer el listado de EColis que proviene de esta lista de Bodies (ver puntero data). Recordar distintas formas de identificar infeccion por el fago: Plasmido esta presente?
    n_neighbors = (float)size;
    return n_neighbors;
}

float Cell::area_concentration(int signal_id, float d_in_microns)
{
    double x = this->get_x();
    double y = this->get_y();
    float d_in_pixels = d_in_microns * DEFAULT_ECOLI_SCALE;
    double x_length = d_in_pixels;
    double y_length = d_in_pixels;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    float sig_val;
    sig_val =  (float)world->handler->getSignalValue(signal_id,coords,"exact");
    //printf("Get value: %f\n", sig_val);

    if(sig_val != -1)
    {
       return sig_val;
    }

    return -1;

}

void Cell::check_action()
{
    std::vector<uint64_t> action_proteins;
    uint64_t cell_proteins = genome.getProteins();
    uint64_t cond = 0, cell = 0;
    std::string absorbQS("s_absorb_QS");
    std::string getQS("s_get_QS");

    for(int row = 0; row < world->get_num_actions(); ++row)
    {
        action_proteins = world->get_action_prot(row);
        cond = action_proteins.at(0) & action_proteins.at(1);
        cell = action_proteins.at(0) & cell_proteins;

        if(cond == cell)
        {
            FnPointer fp = world->get_action(world->get_action_name(row));
            std::list<std::string> pl = world->get_action_param(row);
            ((*this).*fp)(pl);
        }
        /*else if((!world->get_action_name(row).compare(absorbQS) || !world->get_action_name(row).compare(getQS)) && cond != cell)
        {
            std::list<std::string> pl = world->get_action_param(row);
            std::string sig_prot = pl.back();
            reset_signal_prot(sig_prot);
        }*/
    }
    //world->randomize_actions();
}

void Cell::paint_from_list(std::list<string> ls){
    std::list<std::string>::iterator i = ls.begin();
    int gfp = std::atoi((*i).c_str());i++;
    int rfp = std::atoi((*i).c_str());i++;
    int yfp = std::atoi((*i).c_str());i++;
    int cfp = std::atoi((*i).c_str());
    this->set_rep(GFP,gfp);
    this->set_rep(RFP,rfp);
    this->set_rep(YFP,yfp);
    this->set_rep(CFP,cfp);
}

void Cell::delta_paint_from_list(std::list<string> ls){
    std::list<std::string>::iterator i = ls.begin();
    int gfp = 0, rfp = 0, yfp = 0, cfp = 0;
    int d_gfp = std::atoi((*i).c_str());i++;
    int d_rfp = std::atoi((*i).c_str());i++;
    int d_yfp = std::atoi((*i).c_str());i++;
    int d_cfp = std::atoi((*i).c_str());
    gfp = this->get_rep(GFP);
    rfp = this->get_rep(RFP);
    yfp = this->get_rep(YFP);
    cfp = this->get_rep(CFP);
    if(gfp + d_gfp < 0)
    {
        gfp = 0;
    }
    else
    {
        gfp = gfp + d_gfp;
    }
    if(rfp + d_rfp < 0)
    {
        rfp = 0;
    }
    else
    {
        rfp = rfp + d_rfp;
    }
    if(yfp + d_yfp < 0)
    {
        yfp = 0;
    }
    else
    {
        yfp = yfp + d_yfp;
    }
    if(cfp + d_cfp < 0)
    {
        cfp = 0;
    }
    else
    {
        cfp = cfp + d_cfp;
    }
    this->set_rep(GFP,gfp);
    this->set_rep(RFP,rfp);
    this->set_rep(YFP,yfp);
    this->set_rep(CFP,cfp);
}


void Cell::set_eex_from_list(std::list<string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    std::string plasmid_to_eex ((*i).c_str());
    add_eex(plasmid_to_eex);
}

void Cell::remove_eex_from_list(std::list<string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    std::string plasmid_to_eex ((*i).c_str());
    remove_eex(plasmid_to_eex);
}

void Cell::conjugate_from_list(std::list<std::string> ls){
    std::list<std::string>::iterator i = ls.begin();
    std::string id ((*i).c_str());
    i++;
    double n_conj = std::atof((*i).c_str());

    conjugate(id, n_conj, UNDIRECTED);
}

void Cell::conjugate_directed_from_list(std::list<std::string> ls){
    std::list<std::string>::iterator i = ls.begin();
    std::string id ((*i).c_str());
    i++;
    double n_conj = std::atof((*i).c_str());

    conjugate(id, n_conj, DIRECTED);
}

void Cell::lose_plasmid_from_list(std::list<std::string> ls){
    std::list<std::string>::iterator i = ls.begin();
    std::string name ((*i).c_str());
    const std::vector<const cg::Plasmid*>& plasmids = genome.getPlasmids();

    for(auto it : plasmids)
    {
        if(it->getName() == name)
        {
            genome.remove(it);
            return;
        }
    }
}

void Cell::die_from_list(std::list<std::string> ls){
    std::list<std::string>::iterator i = ls.begin();
    int j = std::atoi((*i).c_str());
    if ( this != NULL && !this->marked_for_death())
        this->mark_for_death();
    else
        printf ( "Warning: Called die() from outside a cell program. No action taken\n" );
}

/*void Cell::conj_and_paint_from_list(std::list<std::string> ls){
    std::list<std::string>::iterator i = ls.begin();
    std::map<std::string,int> plasmids_Map = this->world->get_plasmids_map();
    std::string plasmid_to_conj ((*i).c_str());
    //int j = (std::atoi((*i).c_str()))-1;
    int j = plasmids_Map[plasmid_to_conj];
    i++;
    double n_conj = std::atof((*i).c_str());
    i++;
    //double d_n_conj = (double)n_conj;
    //print_state();
    conjugate(j, n_conj);
    int gfp = std::atoi((*i).c_str());i++;
    int rfp = std::atoi((*i).c_str());i++;
    int yfp = std::atoi((*i).c_str());i++;
    int cfp = std::atoi((*i).c_str());
    this->set_rep(GFP,gfp);
    this->set_rep(RFP,rfp);
    this->set_rep(YFP,yfp);
    this->set_rep(CFP,cfp);
}*/

void Cell::change_gt_from_list(std::list<std::string> ls){
    std::list<std::string>::iterator i = ls.begin();
    float growth_rate = std::atof((*i).c_str());
    this->set_param("ecoli_growth_rate", growth_rate);
}

void Cell::emit_cross_feeding_signal_from_list(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str());
    float signal_conc = this->world->get_cross_feeding_max_emit(signal_id);
    float dl_max = 0, dl = 0;

    dl_max = get_param ( "ecoli_growth_rate" ) * this->get_volume() * this->world->get_sim_dt();
    //dl = this->get_d_vol();
    dl = this->get_d_length();
    this->cross_output_coefficient = dl/dl_max;
    if(this->cross_output_coefficient > 1.0)
    {
        this->cross_output_coefficient = 1.0;
    }
    this->world->emit_signal(this,signal_id,(this->cross_output_coefficient*signal_conc));

}

void Cell::get_cross_feeding_signal_from_list(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    int benefit = std::atoi((*i).c_str());
    float signal_level = world->get_signal_value ( this, signal_id );
    float growth_rate = get_param( "ecoli_growth_rate" );

    switch(benefit)
    {
        case 1:
            this->cross_input_coefficient = (signal_level/(this->world->get_cross_feeding_max_emit(signal_id)));
            break;
        case -1:
            this->cross_input_coefficient = (1 - (signal_level/(this->world->get_cross_feeding_max_emit(signal_id))));
            break;
        default:
            this->cross_input_coefficient = 1;
            break;
    }
    if(this->cross_input_coefficient > 1)
    {
        this->cross_input_coefficient = 1;
    }
    else if(this->cross_input_coefficient < 0)
    {
        this->cross_input_coefficient = 0;
    }
}

void Cell::s_emit_signal_area(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;
    std::string emision_type = *i;
    double x = this->get_x();
    double y = this->get_y();
    double x_length = this->get_vec_x();
    double y_length = this->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->emit_signal(signal_id, conc, coords, emision_type.c_str());
    } else if (world->grid_type == "digital") {
        world->dhandler->emit_signal(signal_id, conc, coords, emision_type.c_str());
    }
}

void Cell::s_emit_signal(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;
    std::string emision_type = *i;

    double x = this->get_x();
    double y = this->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->emit_signal(signal_id, conc, coords, emision_type.c_str());
    } else if (world->grid_type == "digital") {
       world->dhandler->emit_signal(signal_id, conc, coords, emision_type.c_str());
    }
}

void Cell::s_get_signal_area(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str());
    double x = this->get_x();
    double y = this->get_y();
    double x_length = this->get_vec_x();
    double y_length = this->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    double sig_val;
    if (world->grid_type == "continuous") {
        sig_val =  world->handler->getSignalValue(signal_id,coords, "area");
    } else if (world->grid_type == "digital") {
        sig_val =  world->dhandler->getSignalValue(signal_id,coords, "area");
    }

    if(sig_val != -1)
    {
        world->signal_concs[signal_id] = sig_val;
    }
}

void Cell::s_get_signal(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str());
    double x = this->get_x();
    double y = this->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    double sig_val;
    if (world->grid_type == "continuous") {
        sig_val =  world->handler->getSignalValue(signal_id,coords, "exact");
    } else if (world->grid_type == "digital") {
        sig_val =  world->dhandler->getSignalValue(signal_id,coords, "exact");
    }

    if(sig_val != -1)
    {
        world->signal_concs[signal_id] = sig_val;
    }
}

void Cell::s_absorb_signal_area(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;
    std::string absorb_type = *i;
    double x = this->get_x();
    double y = this->get_y();
    double x_length = this->get_vec_x();
    double y_length = this->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->absorb_signal(signal_id, conc, coords, absorb_type.c_str());
    } else if (world->grid_type == "digital") {
       world->dhandler->absorb_signal(signal_id, conc, coords, absorb_type.c_str());
    }
}

void Cell::s_absorb_signal(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;
    std::string absorb_type = *i;
    double x = this->get_x();
    double y = this->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->absorb_signal(signal_id, conc, coords, absorb_type.c_str());
    } else if (world->grid_type == "digital") {
        world->dhandler->absorb_signal(signal_id, conc, coords, absorb_type.c_str());
    }
}

void Cell::s_absorb_QS(std::list<std::string> ls)
{
    std::list<std::string>::iterator li = ls.begin();
    int signal_id = std::atoi((*li).c_str()); li++;
    std::string compsign ((*li).c_str()); li++;
    double threshold = std::atof((*li).c_str()); li++;
    std::string protein ((*li).c_str());
    std::string lt("<");

    double x = this->get_x();
    double y = this->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    double sig_val;
    /*std::map<std::string, int> plasmids;
    std::map<std::string, int> operons;
    bool has_noise = false;*/

    //plasmids = this->getPlasmidList()->isGenExist(protein);

    sig_val =  world->handler->getSignalValue(signal_id,coords,"exact");

    /*if(!(plasmids.empty()))
    {
        for(auto pl : plasmids)
        {
            operons = this->getPlasmidList()->getPlasmidById(pl.first)->isGenExist(protein);
            for(auto ops : operons)
            {
                if(this->getPlasmidList()->getPlasmidById(pl.first)->getOperonById(ops.first)->getNoisef())
                {
                    has_noise = true;
                }
            }
        }

        if(!has_noise)
        {

        }
    }*/

    if(((compsign.compare(lt) == 0 && sig_val < threshold) || (compsign.compare(lt) != 0 && sig_val >= threshold)))
    {
        genome.setProteinState(genome.getPlasmidPool()->getProteinByName(protein),true);
        //this->getPlasmidList()->activateGen(protein);
    }
    else
    {
        genome.setProteinState(genome.getPlasmidPool()->getProteinByName(protein),false);
        //this->getPlasmidList()->deactivateGen(protein);
    }

    world->handler->absorb_signal(signal_id, threshold, coords, "exact");
}

void Cell::s_get_QS(std::list<std::string> ls)
{
    std::list<std::string>::iterator li = ls.begin();
    int signal_id = std::atoi((*li).c_str()); li++;
    std::string compsign ((*li).c_str()); li++;
    double threshold = std::atof((*li).c_str()); li++;
    std::string protein ((*li).c_str());
    std::string lt("<");

    double x = this->get_x();
    double y = this->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    double sig_val;
    /*std::map<std::string, int> plasmids;
    std::map<std::string, int> operons;
    bool has_noise = false;

    plasmids = this->getPlasmidList()->isGenExist(protein);*/

    sig_val =  world->handler->getSignalValue(signal_id,coords,"exact");

    /*if(!(plasmids.empty()))
    {
        for(auto pl : plasmids)
        {
            operons = this->getPlasmidList()->getPlasmidById(pl.first)->isGenExist(protein);
            for(auto ops : operons)
            {
                if(this->getPlasmidList()->getPlasmidById(pl.first)->getOperonById(ops.first)->getNoisef())
                {
                    has_noise = true;
                }
            }
        }

        if(!has_noise)
        {

        }
    }*/

    if(((compsign.compare(lt) == 0 && sig_val < threshold) || (compsign.compare(lt) != 0 && sig_val >= threshold)))
    {
        genome.setProteinState(genome.getPlasmidPool()->getProteinByName(protein),true);
        //this->getPlasmidList()->activateGen(protein);
    }
    else
    {
        genome.setProteinState(genome.getPlasmidPool()->getProteinByName(protein),false);
        //this->getPlasmidList()->deactivateGen(protein);
    }
}

//OJO con como implementar crecimiento junto con get.

/*void Cell::s_set_signal_multiple(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;

    double x = 0;
    double y = 0;
    double x_length = 0;
    double y_length = 0;
    double coords[4];

    for(todas las ubicaciones - sobre el iterador de ls)
    {
        x = std::atof((*i).c_str()); i++;
        y = std::atof((*i).c_str()); i++;
        x_length = std::atof((*i).c_str()); i++;
        y_length = std::atof((*i).c_str()); i++;

        coords[0] = x;
        coords[1] = y;
        coords[2] = x_length;
        coords[3] = y_length;

        world->handler->setSignalValue(signal_id, conc, coords);
    }
}*/

void Cell::s_set_signal_rect(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;
    double x = std::atof((*i).c_str()); i++;
    double y = std::atof((*i).c_str()); i++;
    double x_length = std::atof((*i).c_str()); i++;
    double y_length = std::atof((*i).c_str());

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
        world->handler->setSignalValue(signal_id, conc, coords, "area");
    } else if (world->grid_type == "digital") {
        world->dhandler->setSignalValue(signal_id, conc, coords, "area");
    }

}

void Cell::s_set_signal(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    double conc = std::atof((*i).c_str()); i++;
    double x = std::atof((*i).c_str()); i++;
    double y = std::atof((*i).c_str());
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
        world->handler->setSignalValue(signal_id, conc, coords, "exact");
    } else if (world->grid_type == "digital") {
        world->dhandler->setSignalValue(signal_id, conc, coords, "exact");
    }

}

void Cell::s_emit_cross_feeding_signal_from_list(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    float signal_conc = std::atof((*i).c_str()); i++;
    std::string emission_type = *i;

    float dl_max = 0, dl = 0;
    double x = this->get_x();
    double y = this->get_y();
    double x_length = this->get_vec_x();
    double y_length = this->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    dl_max = get_param ( "ecoli_growth_rate" ) * this->get_volume() * this->world->get_sim_dt();
    //dl = this->get_d_vol();
    dl = this->get_d_length();

    this->cross_output_coefficient = dl/dl_max;

    if(this->cross_output_coefficient > 1.0)
    {
        this->cross_output_coefficient = 1.0;
    }

    if (world->grid_type == "continuous") {
        world->handler->emit_signal(signal_id,(this->cross_output_coefficient*signal_conc),coords, emission_type.c_str());
    } else if (world->grid_type == "digital") {
        world->dhandler->emit_signal(signal_id,(this->cross_output_coefficient*signal_conc),coords, emission_type.c_str());
    }
}

void Cell::s_get_cross_feeding_signal_from_list(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    float signal_conc = std::atof((*i).c_str()); i++;
    int benefit = std::atoi((*i).c_str());
    double x = this->get_x();
    double y = this->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;
    float signal_level = world->handler->getSignalValue(signal_id, coords, "exact");
    float growth_rate = get_param( "ecoli_growth_rate" );

    if(signal_level != -1)
    {
        world->signal_concs[signal_id] = signal_level;
    }
    else
    {
        signal_level = 0;
    }

    switch(benefit)
    {
        case 1:
            this->cross_input_coefficient = (signal_level/(signal_conc));
            break;
        case -1:
            this->cross_input_coefficient = (1 - (signal_level/(signal_conc)));
            break;
        default:
            this->cross_input_coefficient = 1;
            break;
    }
    if(this->cross_input_coefficient > 1)
    {
        this->cross_input_coefficient = 1;
    }
    else if(this->cross_input_coefficient < 0)
    {
        this->cross_input_coefficient = 0;
    }
}

void Cell::s_absorb_cross_feeding_signal_from_list(std::list<std::string> ls)
{
    std::list<std::string>::iterator i = ls.begin();
    int signal_id = std::atoi((*i).c_str()); i++;
    float signal_conc = std::atof((*i).c_str()); i++;
    int benefit = std::atoi((*i).c_str()); i++;
    std::string absorb_type = *i;
    double x = this->get_x();
    double y = this->get_y();
    double x_length = this->get_vec_x();
    double y_length = this->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;
    float pre_signal_level;

    if (world->grid_type == "continuous") {
        pre_signal_level = world->handler->getSignalValue(signal_id, coords, "exact");
    } else if (world->grid_type == "digital") {
        pre_signal_level = world->dhandler->getSignalValue(signal_id, coords, "exact");
    }

    float growth_rate = get_param( "ecoli_growth_rate" );
    float post_signal_level;
    float dif_level = 0;


    if(pre_signal_level != -1)
    {
        if (world->grid_type == "continuous") {
            world->handler->absorb_signal(signal_id, signal_conc, coords, "exact");
            post_signal_level = world->handler->getSignalValue(signal_id, coords, "exact");
        } else if (world->grid_type == "digital") {
            world->dhandler->absorb_signal(signal_id, signal_conc, coords, absorb_type.c_str());
            post_signal_level = world->dhandler->getSignalValue(signal_id, coords, "exact");
        }
        dif_level = pre_signal_level - post_signal_level;
    }
    else
    {
        dif_level = 0;
    }

    switch(benefit)
    {
        case 1:
            this->cross_input_coefficient = (dif_level/(signal_conc));
            break;
        case -1:
            this->cross_input_coefficient = (1 - (dif_level/(signal_conc)));
            break;
        default:
            this->cross_input_coefficient = 1;
            break;
    }

    if(this->cross_input_coefficient > 1)
    {
        this->cross_input_coefficient = 1;
    }
    else if(this->cross_input_coefficient < 0)
    {
        this->cross_input_coefficient = 0;
    }
}

int Cell::check_gen_condition(std::vector<uint64_t> cond)
{
    int result = 0;

    uint64_t input = cond.at(1) ^ ~genome.getProteins();
    uint64_t gateValue = (cond.at(0) & input);

    if(gateValue == cond.at(0))
    {
        result = 1;
    }

    return result;
}

int Cell::check_plasmid_condition(std::vector<std::pair<std::string, int>> cond)
{
    int result = 1;

    const std::vector<const cg::Plasmid*> plasmids = genome.getPlasmids();
    std::map<std::string,const cg::Plasmid*> temp;

    for(auto it : plasmids)
    {
        temp.insert(std::pair<std::string,const cg::Plasmid*>(it->getName(),it));
    }

    for(auto it : cond)
    {
        if((temp.find(it.first) == temp.end() && it.second == 1) || (temp.find(it.first) != temp.end() && it.second == -1))
        {
            result = 0;
        }
    }
    return result;
}

void Cell::state_to_file(FILE *fp, std::vector<std::string> prot_names)
{
    fprintf ( fp, "%f, %d, %f, %f, %f, %f, %f, %d, %d, %d, %d",
              this->world->get_time(),
              this->get_id(), this->get_x(), this->get_y(), this->get_theta(), this->get_volume(), this->get_gt_inst(),
              this->get_rep(GFP), this->get_rep(RFP), this->get_rep(YFP), this->get_rep(CFP));

    for(auto it : prot_names)
    {
        if(genome.getProteins() & genome.getPlasmidPool()->getProteinByName(it)->getID())
        {
            fprintf(fp, ", 1");
        }
        else
        {
            fprintf(fp, ", 0");
        }
    }

    fprintf(fp, "\n");

}

void Cell::state_to_file_reduced(FILE *fp, std::vector<std::string> prot_names)
{
    fprintf ( fp, "%6.1f, %6.1f",
              this->world->get_time(), this->get_x());

    float uno = 1;
    float cero = 0;

    for(auto it : prot_names)
    {
        if(genome.getProteins() & genome.getPlasmidPool()->getProteinByName(it)->getID())
        {
            fprintf(fp, ", %6.0f",uno);
        }
        else
        {
            fprintf(fp, ", %6.0f", cero);
        }
    }

    fprintf(fp, "\n");

}
