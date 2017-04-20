/////////////////////////////////////////////////////////////////////////////////////////
//
// gro is protected by the UW OPEN SOURCE LICENSE, which is summaraized here.
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
#include "Rands.h"
#include "CellNutrient.h"
#include <sys/time.h>

using namespace std;

#ifdef NOGUI
World::World ( void ) {
#else
World::World ( GroThread *ct ) : calling_thread ( ct ) , plasmidCloud(std::random_device()()) {
#endif

    prog = NULL;
    chemostat_mode = false;
    next_id = 0;
    step = 0;
    print_rate = -1;
    movie_rate = -1;
    program_initialized = false;
    program_restarted = false;
    gro_message = "";
    stop_flag = false;
    zoom = 1.0;
    barriers = new std::list<Barrier>;

    num_actions = 0;
    stt_time = 0;

    signal_concs.clear();
    s_signal_id = 0;

    noprot = true;
    noaction = true;

    proteins = new std::map<std::string, std::vector<float>>;
    operons = new std::map<std::string, operon_data>;
    signal_prots = new std::vector<std::string>;

    set_sim_dt ( DEFAULT_SIM_DT );
    set_chip_dt ( DEFAULT_CHIP_DT );

    output_started = false;

}

float consumido = 0;
float available = 0;
unsigned int cellLength = (MAX_LENGTH + MIN_LENGTH) * 0.5;

World::~World ( void ) {

    std::vector<Cell *>::iterator j;

    for ( j=population->begin(); j!=population->end(); j++ ) {
        delete (*j);
    }

    unsigned int k;

    for ( k=0; k<signal_list.size(); k++ )
        delete signal_list[k];

    /*cpSpaceFreeChildren(space);
    cpSpaceFree(space);*/

    delete proteins;
    delete operons;
    delete signal_prots;

    ceDestroySpace(space);
    csDestroyGrid(signalGrid);
    delete population;
    delete handler;

    prog->destroy(this);

}

void Cell::init ( const int * q0, const int * rep0, float frac ) {

    int i;

    for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = (int) floor(frac*q0[i]);
    for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = (int) floor(frac*rep0[i]);

}

void World::init () {

    // Time
    t = 0.0f;
    max_val = 0.0f;

    // Cells
    population = new std::vector<Cell *>; // deleted in ~World
    std::vector<Cell *>::iterator j;

    float *difdeg;
    difdeg = (float*)malloc(4*sizeof(float));

    // Chipmunk stuff
    /*cpResetShapeIdCounter();
    space = cpSpaceNew(); // freed in ~World
    cpSpaceResizeActiveHash(space, 60.0f, 10000);
    space->iterations = ITERATIONS;
    space->damping = DAMPING;*/
    space = ceCreateSpace();

    signalGrid = csCreateGrid(get_param("nutrient_amount"), get_param("nutrient_consumption_rate"), get_param("nutrient_variability"), (unsigned int)(get_param("nutrient_grid_length")),
                              (float)(get_param("nutrient_grid_cell_size")), (int)(get_param("nutrient_consumption_mode")), difdeg);

    if(!program_restarted)
    {
        //Nutrients
        set_param ("nutrients", 0.0);
        set_param ("nutrient_amount", 200.0);
        set_param ("nutrient_consumption_rate", 0.033);
        set_param ("nutrient_variability", 0.2);
        set_param ("nutrient_grid_length", 10);
        set_param ("nutrient_grid_cell_size", cellLength);
        set_param ("nutrient_consumption_mode", 0);
        set_param ("nutrient_diffusion", 0.0);
        set_param ("nutrient_kdiff", 0.0);
        set_param ("nutrient_degradation", 0.0);
        set_param ("nutrient_kdeg", 0.0);

        //Signals
        set_param ("signals", 1.0);
        //set_param ("signals", 0.0);
        set_param ("signals_grid_length", 10);
        set_param ("signals_grid_cell_size", 5);
        set_param ("signals_grid_neighborhood", 8);
        set_param ("signals_draw", 1.0);

        set_param ( "chemostat_width", 200);
        set_param ( "chemostat_height", 200);
        set_param ( "signal_area_width", 1000);
        //set_param ( "signal_num_divisions", 50);
        set_param ( "signal_num_divisions", 5);


        /*set_param ( "signal_area_width", 150);
        set_param ( "signal_num_divisions", 5);*/
        set_param ( "population_max", 1000 );

        set_param ( "signal_grid_width", 1000 );
        set_param ( "signal_grid_height", 1000 );
        set_param ( "signal_element_size", 5 );
        /*set_param ( "signal_grid_width", 150 );
        set_param ( "signal_grid_height", 150 );
        set_param ( "signal_element_size", 30 );*/
    }

    // Default parameters. These will be over-written when/if the program
    // defines them via "set". But just in case the user does not do this,
    // they are defined here.

    /*set_param ( "chemostat_width", 200);
    set_param ( "chemostat_height", 200);
    set_param ( "signal_area_width", 800);
    set_param ( "signal_num_divisions", 160);
    set_param ( "population_max", 1000 );

    set_param ( "signal_grid_width", 800 );
    set_param ( "signal_grid_height", 800 );
    set_param ( "signal_element_size", 5 );*/

    this->num_actions = 0;

    // Program
    ASSERT ( prog != NULL );

    if ( !program_initialized ) {

        prog->init(this); // if this throws an exception
        // the program will not be initialized
        // in the next line

        program_initialized = true;

    }

    csDestroyGrid(signalGrid);

    if(get_param("nutrients") == 1.0)
    {
        difdeg[0] = get_param("nutrient_diffusion");
        difdeg[1] = get_param("nutrient_kdiff");
        difdeg[2] = get_param("nutrient_degradation");
        difdeg[3] = get_param("nutrient_kdeg");

        float horizontal_initial_length_delta = this->dump_right() - this->dump_left();
        float vertical_initial_length_delta = this->dump_bottom() - this->dump_top();
        float largest_delta = 0;
        unsigned int grid_length = 0;
        if(horizontal_initial_length_delta == 0)
        {
            horizontal_initial_length_delta = this->dump_right();
        }
        if(horizontal_initial_length_delta < 0)
        {
            horizontal_initial_length_delta = -horizontal_initial_length_delta;
        }
        if(vertical_initial_length_delta == 0)
        {
            vertical_initial_length_delta = this->dump_bottom();
        }
        if(vertical_initial_length_delta < 0)
        {
            vertical_initial_length_delta = -vertical_initial_length_delta;
        }
        if(horizontal_initial_length_delta > vertical_initial_length_delta)
        {
            largest_delta = horizontal_initial_length_delta;
        }
        else
        {
            largest_delta = vertical_initial_length_delta;
        }

        grid_length = 2*(unsigned int)ceil((double)(largest_delta/get_param("nutrient_grid_cell_size")));

        if(grid_length <= 10)
        {
            grid_length = 10;
        }

        signalGrid = csCreateGrid(get_param("nutrient_amount"), get_param("nutrient_consumption_rate"), get_param("nutrient_variability"), grid_length,
                                 (float)(get_param("nutrient_grid_cell_size")), (int)(get_param("nutrient_consumption_mode")), difdeg);
    }

    // Chemostat
    if ( chemostat_mode ) {

        /*cpShape *shape;
        cpBody *staticBody = &space->staticBody;

        int w = get_param("chemostat_width")/2,
                h = get_param("chemostat_height")/2;

        shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-400,h), cpv(-w,h), 5.0f));
        shape->e = 1.0f; shape->u = 0.0f;

        shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-w,h), cpv(-w,-h), 5.0f));
        shape->e = 1.0f; shape->u = 0.0f;

        shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-w,-h), cpv(w,-h), 5.0f));
        shape->e = 1.0f; shape->u = 0.0f;

        shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(w,-h), cpv(w,h), 5.0f));
        shape->e = 1.0f; shape->u = 0.0f;

        shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(w,h), cpv(400,h), 5.0f));
        shape->e = 1.0f; shape->u = 0.0f;*/

    }

    free(difdeg);

    const std::map<std::string,const cg::Plasmid*>& active_plasmids = this->plasmidCloud.getPlasmids();
    for(auto it : active_plasmids)
    {
        conj_counts[it.second] = 0;
    }

    if(program_restarted) //This is for restart
    {
        //program_initialized = false;
        program_restarted = false;
    }

    randomize_population();
}

void World::emit_message ( std::string str, bool clear ) {

#ifndef NOGUI
    calling_thread->emit_message ( str, clear );
#else
    std::cerr << str << "\n";
#endif

}

void World::randomize_actions()
{
    std::random_shuffle(action_info.begin(), action_info.end());
}

void World::remove_all_actions()
{
    action_info.clear();
    map_actions.clear();
}

void World::restart ( void ) {

    std::vector<Cell *>::iterator j;

    for ( j=population->begin(); j!=population->end(); j++ ) {
        delete (*j);
    }

    /*cpSpaceFreeChildren(space);
    cpSpaceFree(space);*/

    ceDestroySpace(space);
    if(get_param("nutrients") == 1.0)
    {
        csDestroyGrid(signalGrid);
    }

    delete population;

    /* OBSERVE THIS! MAYBE NEEDED! */

    proteins->clear();
    operons->clear();
    signal_prots->clear();
    //signal_prots->clear();
    num_actions = 0;
    remove_all_actions();
    //action_info.clear();
    //map_actions.clear();
    signal_concs.clear();
    s_signal_id = 0;
    noprot = true;
    noaction = true;
    conj_counts.clear();

    if(get_param("signals") == 1.0)
    {
        delete handler;
    }

    //prog->destroy(this);
    /**/

    unsigned int k;
    for ( k=0; k<signal_list.size(); k++ ) {
        signal_list[k]->zero();
    }

    program_restarted = true;

    init();

    gro_message = "";

}

///////////////////////////////////////////////////REARMAR CUANDO ESTE LISTO TODO///////////////////////////////////////////
void World::replating ( float percentage )
{
    /*std::list<Cell *>::iterator j;
    std::list<Cell *>* population2;
    float pop_size = (float)population->size();
    int gap = 1, counter = 0;
    float *digdef;
    digdef = (float*)malloc(4*sizeof(float));

    if(percentage > 1)
    {
        percentage = 1;
    }
    else if(percentage < 0)
    {
        percentage = 1;
    }

    gap = (int)(pop_size/(pop_size*percentage));
    population2 = new std::list<Cell *>;

    for ( j=population->begin(); j!=population->end(); j++ )
    {
        if (counter == (gap-1))
        {
            population2->push_back((*j));
            counter = 0;
        }
        else
        {
            counter++;
        }
    }

    ceDestroySpace(space);

    if(get_param("nutrients") == 1.0)
    {
        csDestroyGrid(signalGrid);
    }
    delete population;

    unsigned int k;
    for ( k=0; k<signal_list.size(); k++ ) {
        signal_list[k]->zero();
    }

    // Time
    max_val = 0.0f;

    // Cells
    population = new std::list<Cell *>; // deleted in ~World

    space = ceCreateSpace();

    //Ve si hay que mover esto despues del traslado de bacterias por population2.
    if(get_param("nutrients") == 1.0)
    {
        digdef[0] = get_param("nutrient_diffusion");
        digdef[1] = get_param("nutrient_kdiff");
        digdef[2] = get_param("nutrient_degradation");
        digdef[3] = get_param("nutrient_kdeg");

        float horizontal_initial_length_delta = this->dump_right() - this->dump_left();
        float vertical_initial_length_delta = this->dump_bottom() - this->dump_top();
        float largest_delta = 0;
        unsigned int grid_length = 0;
        if(horizontal_initial_length_delta == 0)
        {
            horizontal_initial_length_delta = this->dump_right();
        }
        if(horizontal_initial_length_delta < 0)
        {
            horizontal_initial_length_delta = -horizontal_initial_length_delta;
        }
        if(vertical_initial_length_delta == 0)
        {
            vertical_initial_length_delta = this->dump_bottom();
        }
        if(vertical_initial_length_delta < 0)
        {
            vertical_initial_length_delta = -vertical_initial_length_delta;
        }
        if(horizontal_initial_length_delta > vertical_initial_length_delta)
        {
            largest_delta = horizontal_initial_length_delta;
        }
        else
        {
            largest_delta = vertical_initial_length_delta;
        }

        grid_length = 2*(unsigned int)ceil((double)(largest_delta/get_param("nutrient_grid_cell_size")));

        if(grid_length <= 10)
        {
            grid_length = 10;
        }

        signalGrid = csCreateGrid(get_param("nutrient_amount"), get_param("nutrient_consumption_rate"), get_param("nutrient_variability"), grid_length,
                                 (float)(get_param("nutrient_grid_cell_size")), (int)(get_param("nutrient_consumption_mode")), digdef);

    }

    if(get_param("signals") == 0.0 && handler != nullptr)
    {
        delete handler;
    }
    else if(get_param("signals") == 1.0)
    {
        //CREAR AQUI
        std::vector<GSignal*> old_signal_vector = handler->getSignalsVector();
        if(handler->)
        delete handler;
        handler = new Core((unsigned int)get_param ("signals_grid_length"), (unsigned int)get_param ("signals_grid_cell_size"));
        handler->setSignalsVector(old_signal_vector);
    }

    for(j=population2->begin(); j!=population2->end(); j++)
    {

        Program * prog = (*j)->get_gro_program()->copy();
        float x = 0, y = 0, theta = 0, vol;
        float max_radius = (float)(population2->size());

        x = (float) fRand(-max_radius, max_radius);
        y = (float) fRand(-max_radius, max_radius);

        while(((x*x) + (y*y)) > (max_radius*max_radius))
        {
            x = (float) fRand(-max_radius, max_radius);
            y = (float) fRand(-max_radius, max_radius);
        }

        theta = (float) fRand(0.0, 6.28);
        vol = (float)((*j)->get_volume());

        EColi * c = new EColi ( this, x, y, theta, vol );

        c->set_div_count((*j)->get_div_count());
        c->set_force_divide((*j)->get_force_divide());

        int i;

        for( i=0; i<(*j)->nPlasmids(); i++)
        {
            c->setPlasmid(i,(*j)->hasPlasmid(i));
            c->set_receivedPlasmid(i,(*j)->receivedPlasmid(i));
            c->set_conjugated_indicator(i,(*j)->get_conjugated_indicator(i));
            c->set_was_just_conjugated(i,(*j)->was_just_conjugated(i));
            c->setEEX(i,(*j)->hasEEX(i));
        }

        for( i=0; i<(*j)->get_num_proteins(); i++)
        {
            c->set_internal_protein(i,(*j)->get_internal_protein(i));
            c->set_internal_RNA(i,(*j)->get_internal_RNA(i));
            c->set_degr_prot(i,(*j)->get_degr_prot(i));
            c->set_degr_rna(i,(*j)->get_degr_rna(i));
        }

        for ( i=0; i<this->num_operons(); i++ ){
            for(k=0;k<(*j)->get_num_proteins();k++){
                c->set_last_state_value(i,k,(*j)->get_last_state(i,k));
                c->set_rna_operon(i,k,(*j)->get_rna_operon(i,k));
                c->set_prot_actv_time(i,k,(*j)->get_prot_actv_time(i,k));
                c->set_prot_degr_time(i,k,(*j)->get_prot_degr_time(i,k));
                c->set_rna_actv_time(i,k,(*j)->get_rna_actv_time(i,k));
                c->set_rna_degr_time(i,k,(*j)->get_rna_degr_time(i,k));

                c->set_protein_operon(i,k, c->get_protein_operon(i,k));
                if((*j)->get_protein_operon(i,k)){
                    Operon * op = this->get_operon(i);
                    double up_p = op->get_prot_up_time(k);
                    double up_p_err = op->get_prot_up_time_err(k);
                    double up_r = op->get_rna_up_time(k);
                    double up_r_err = op->get_rna_up_time_err(k);
                    c->set_prot_actv_time(i,k,(this->get_time() + (drand48() * ((up_p + up_p_err)-(up_p-up_p_err))+(up_p-up_p_err)))); //la mitad del tiempo de activacion normal
                    c->set_rna_actv_time(i,k,(this->get_time() + (drand48() * ((up_r + up_r_err)-(up_r-up_r_err))+(up_r-up_r_err)))); //la mitad del tiempo de activacion normal
                }
            }
        }


        c->set_param_map ( get_param_map() );

        if ( c->get_gro_program() == NULL ) {
            c->set_gro_program ( prog );
        }

        for ( i=0; i<MAX_STATE_NUM; i++ ) c->set(i,(*j)->get(i));
        for ( i=0; i<MAX_REP_NUM; i++ ) c->set_rep(i,(*j)->get_rep(i));
        c->set_param("ecoli_growth_rate", (*j)->get_param("ecoli_growth_rate"));

        this->add_cell ( c );

    }
    delete population2;*/

}

static char buf[1024];

#ifndef NOGUI

static void drawString ( GroPainter * painter, int x, int y, const char *str) {

    painter->drawText ( x, y, str );

}

void World::render ( GroPainter * painter ) {

    std::vector<Cell *>::iterator j;
    int dec = 0;

    theme.apply_background ( painter );

    painter->scale(zoom,zoom);

    if ( signal_list.size() > 0 ) {

        int i,j;
        unsigned int k;
        float r, g, b;
        QColor col;
        float sum;

        for ( i=0; i<signal_list.front()->get_numx(); i++ ) {

            for ( j=0; j<signal_list.front()->get_numy(); j++ ) {

                r = 0; g = 0; b = 0;

                // determine overall signal level
                sum = 0.0;
                for ( k=0; k<signal_list.size(); k++ )
                    sum += signal_list[k]->get_val(i,j);

                if ( sum > 0.01 ) {

                    for ( k=0; k<signal_list.size(); k++ ) {

                        // accumulate color
                        theme.accumulate_color ( k, signal_list[k]->get_val(i,j), &r, &g, &b );

                    }

                }

                if ( sum > 0.01  ) {

                    col.setRgbF(r/k,g/k,b/k);
                    painter->fillRect(
                                signal_list[0]->get_minp().x+signal_list[0]->get_dw()*i,
                                signal_list[0]->get_minp().y+signal_list[0]->get_dh()*j,
                                (int) ( signal_list[0]->get_dw() ),
                                (int) ( signal_list[0]->get_dh() ),
                                col);

                }

            }

        }

        theme.apply_message_color(painter);

        // draw cross hairs for corners of signal area
        int x = get_param ( "signal_grid_width" ) / 2,
                y = get_param ( "signal_grid_height" ) / 2;

        painter->drawLine ( x-20, y,  x+20, y );
        painter->drawLine ( x, y-20,  x, y+20 );

        painter->drawLine ( -x-20, y,  -x+20, y );
        painter->drawLine ( -x, y-20,  -x, y+20 );

        painter->drawLine ( x-20, -y,  x+20, -y );
        painter->drawLine ( x, -y-20,  x, -y+20 );

        painter->drawLine ( -x-20, -y,  -x+20, -y );
        painter->drawLine ( -x, -y-20,  -x, -y+20 );

    }

    if ( signal_concs.size() > 0 && get_param("signals_draw") == 1.0 ) {

        int i,j;
        unsigned int k;
        float r, g, b;
        QColor col;
        float sum;
        float firstX, firstY;

        if (this->grid_type == "continuous" && this->diff_method == "gro_original") {
            for ( i=0; i<handler->getLen(); i++ ) {

                for ( j=0; j<handler->getLen(); j++ ) {

                    r = 0; g = 0; b = 0;

                    // determine overall signal level
                    sum = 0.0;
                    for ( k=0; k<signal_concs.size(); k++ )
                    {
                        //printf("Signal %d concentration at (%d,%d): %f\n",k,i,j, handler->grid->getSignalGrid(i,j)->getConcentration(k));
                        sum += handler->getCell(i,j)->getValue(k);
                    }

                    if ( sum > 0.01 ) {

                        for ( k=0; k<signal_concs.size(); k++ ) {

                            // accumulate color
                            theme.accumulate_color ( k, handler->getCell(i,j)->getValue(k), &r, &g, &b );
                        }

                    }

                    if ( sum > 0.01  ) {

                        col.setRgbF(r/k,g/k,b/k);
                        firstX = -(((float)(handler->getLen()))/2)*(float)(handler->getCellSize());
                        firstY = firstX;
                        painter->fillRect(
                                    firstX+handler->getCellSize()*j,
                                    (firstY+handler->getCellSize()*i),
                                    (int) ( handler->getCellSize() ),
                                    (int) ( handler->getCellSize() ),
                                    col);

                    }

                }

            }


            theme.apply_message_color(painter);

            int x = (((float)(handler->getLen()*handler->getCellSize()))/2),
                y = (((float)(handler->getLen()*handler->getCellSize()))/2);

            painter->drawLine ( x-20, y,  x+20, y );
            painter->drawLine ( x, y-20,  x, y+20 );

            painter->drawLine ( -x-20, y,  -x+20, y );
            painter->drawLine ( -x, y-20,  -x, y+20 );

            painter->drawLine ( x-20, -y,  x+20, -y );
            painter->drawLine ( x, -y-20,  x, -y+20 );

            painter->drawLine ( -x-20, -y,  -x+20, -y );
            painter->drawLine ( -x, -y-20,  -x, -y+20 );

        } else if (this->grid_type == "continuous" && this->diff_method == "matrix") {
            for ( i=0; i<handler->getLen(); i++ ) {

                for ( j=0; j<handler->getLen(); j++ ) {

                    r = 0; g = 0; b = 0;

                    // determine overall signal level
                    sum = 0.0;
                    for ( k=0; k<signal_concs.size(); k++ )
                    {
                        //printf("Signal %d concentration at (%d,%d): %f\n",k,i,j, handler->grid->getSignalGrid(i,j)->getConcentration(k));
                        sum += handler->getCell(i,j)->getValue(k);
                    }

                    if ( sum > 0.01 ) {

                        for ( k=0; k<signal_concs.size(); k++ ) {

                            // accumulate color
                            theme.accumulate_color ( k, handler->getCell(i,j)->getValue(k), &r, &g, &b );
                        }

                    }

                    if ( sum > 0.01  ) {

                        col.setRgbF(r/k,g/k,b/k);
                        firstX = -(((float)(handler->getLen()))/2)*(float)(handler->getCellSize());
                        firstY = firstX;
                        painter->fillRect(
                                    firstX+handler->getCellSize()*j,
                                    (firstY+handler->getCellSize()*i),
                                    (int) ( handler->getCellSize() ),
                                    (int) ( handler->getCellSize() ),
                                    col);

                    }

                }

            }


            theme.apply_message_color(painter);

            int x = (((float)(handler->getLen()*handler->getCellSize()))/2),
                y = (((float)(handler->getLen()*handler->getCellSize()))/2);

            painter->drawLine ( x-20, y,  x+20, y );
            painter->drawLine ( x, y-20,  x, y+20 );

            painter->drawLine ( -x-20, y,  -x+20, y );
            painter->drawLine ( -x, y-20,  -x, y+20 );

            painter->drawLine ( x-20, -y,  x+20, -y );
            painter->drawLine ( x, -y-20,  x, -y+20 );

            painter->drawLine ( -x-20, -y,  -x+20, -y );
            painter->drawLine ( -x, -y-20,  -x, -y+20 );

        } else if (this->grid_type == "digital") {
            for ( i=0; i<dhandler->getLen(); i++ ) {

                for ( j=0; j<dhandler->getLen(); j++ ) {

                    r = 0; g = 0; b = 0;

                    // determine overall signal level
                    sum = 0.0;
                    for ( k=0; k<signal_concs.size(); k++ )
                    {
                        //printf("Signal %d concentration at (%d,%d): %f\n",k,i,j, dhandler->getCell(i,j)->getValue(k));
                        sum += dhandler->getCell(i,j)->getValue(k);
                    }

                    if ( sum > 0.01 ) {

                        for ( k=0; k<signal_concs.size(); k++ ) {

                            // accumulate color
                            theme.accumulate_color ( k, dhandler->getCell(i,j)->getValue(k), &r, &g, &b );
                        }

                    }

                    if ( sum > 0.01  ) {

                        col.setRgbF(r/k,g/k,b/k);
                        firstX = -(((float)(dhandler->getLen()))/2)*(float)(dhandler->getCellSize());
                        firstY = firstX;
                        painter->fillRect(
                                    firstX+dhandler->getCellSize()*j,
                                firstY+dhandler->getCellSize()*i,
                                (int) ( dhandler->getCellSize() ),
                                (int) ( dhandler->getCellSize() ),
                                col);

                    }

                }

            }

            theme.apply_message_color(painter);

            int x = (((float)(dhandler->getLen()*dhandler->getCellSize()))/2),
                    y = (((float)(dhandler->getLen()*dhandler->getCellSize()))/2);

            painter->drawLine ( x-20, y,  x+20, y );
            painter->drawLine ( x, y-20,  x, y+20 );

            painter->drawLine ( -x-20, y,  -x+20, y );
            painter->drawLine ( -x, y-20,  -x, y+20 );

            painter->drawLine ( x-20, -y,  x+20, -y );
            painter->drawLine ( x, -y-20,  x, -y+20 );

            painter->drawLine ( -x-20, -y,  -x+20, -y );
            painter->drawLine ( -x, -y-20,  -x, -y+20 );

        }
    }

    for ( j=population->begin(); j!=population->end(); j++ ) {
        (*j)->render ( &theme, painter );
    }

    theme.apply_chemostat_edge_color(painter);
    painter->setBrush( QBrush() );

    QPainterPath path;

    // Chemostat
    if ( chemostat_mode ) {

        int w = get_param("chemostat_width")/2,
                h = get_param("chemostat_height")/2;

        path.moveTo(-400,h);
        path.lineTo(-w,h);
        path.lineTo(-w,-h);
        path.lineTo(w,-h);
        path.lineTo(w,h);
        path.lineTo(400,h);

        //path.closeSubpath();
        painter->drawPath(path);

    }

    std::list<Barrier>::iterator b;

    for ( b=barriers->begin(); b != barriers->end(); b++ ) {
      path.moveTo((*b).x1,(*b).y1);
      path.lineTo((*b).x2,(*b).y2);
      painter->drawPath(path);
    }

    //  if ( 1 ) {
    //    static char buf[1000];
    //    sprintf ( buf, "%s", gro_message.c_str() );
    //    drawString ( painter, -360, 360-dec, buf );
    //    dec += 16;
    //  }

    painter->reset();

    theme.apply_message_color(painter);

    sprintf ( buf, "Cells: %d, Max: %d, t = %.2f min", (int) population->size(), (int) get_param ( "population_max" ), t );
    drawString ( painter, -painter->get_size().width()/2+10, -painter->get_size().height()/2+20, buf );
    dec = 32;

    message_handler.render(painter);

}

#endif

void World::add_cell ( Cell * c ) {

    population->push_back ( c );
    c->set_id ( next_id++ );
    //cpSpaceStep(space, get_chip_dt());

}

static bool out_of_bounds ( float x, float y ) {

    return x < -400 || x > 400 || y < -400 || y > 400;

}

/*cpVect World::chemostat_flow ( float, float y, float mag ) {

    if ( y > get_param("chemostat_height")/2 ) return cpv ( mag, 0 );
    else return cpv ( 0, 0 );

}*/

ceVector2 World::chemostat_flow ( float, float y, float mag ) {

    if ( y > get_param("chemostat_height")/2 ) return ceGetVector2 ( mag, 0 );
    else return ceGetVector2 ( 0, 0 );

}

void World::update ( void ) {

    ceStep(space);

    std::vector<Cell *>::iterator j;
    unsigned int n_bacteria = population->size();
    int index = 0, k=0, l=0; //m = 0;
    float* consumption;

    if(get_param("nutrients") == 1.0)
    {

        consumption = (float*)malloc(sizeof(float)*n_bacteria);
        //float consumption[n_bacteria][2];

        float coords[n_bacteria][4];
        for ( j=population->begin(); j!=population->end(); j++ ) {
            //if ((((*j)->get_x()) < RADIO*cellLength) || (((*j)->get_y()) < RADIO*cellLength) || (((*j)->get_x()) > -(RADIO*cellLength)) || (((*j)->get_y()) > -(RADIO*cellLength))) {
              coords[index][0] = (float)((*j)->get_x());
              coords[index][1] = (float)((*j)->get_y());
              coords[index][2] = (float)((*j)->get_vec_x());
              coords[index][3] = (float)((*j)->get_vec_y());
              index++;
            /*} else {
                Cell * c = (*j);
                ceDestroyBody(c->get_body());
            }*/
        }

        consumption = csGridStep(signalGrid, n_bacteria, coords);
    }

    if ( population->size() < get_param ( "population_max" ) ) {

        std::vector<Cell *> children;
        prog->world_update ( this );
        std::vector<Cell *>::iterator j;

        // update each cell
        int indice = 0;
        for ( j=population->begin(); j!=population->end() && indice < n_bacteria; j++ ) {

            if(get_param("nutrients") == 1.0)
            {
                available = consumption[indice];
            }

            if(!noprot)
            {
                (*j)->getGenome().update(this->t, get_sim_dt());
            }
            //Reset de proteinas de senal aqui?
            (*j)->reset_signal_prots();
            if(!noaction)
            {
                (*j)->check_action();
            }
            (*j)->update();

            // check for divisions
            /*Cell * d = (*j)->divide();
            if ( d != NULL ) add_cell ( d );*/

            if(!(*j)->marked_for_death())
            {
                Cell * d = (*j)->divide();
                if ( d != NULL ) children.push_back(d);
                indice++;
            }
        }

        for(unsigned int i = 0; i< children.size(); i++)
            add_cell(children[i]);

        unsigned int i, J, k;

        for ( k=0; k<reaction_list.size(); k++ )
            for ( int i=0; i<signal_list.front()->get_numx(); i++ )
                for ( int J=0; J<signal_list.front()->get_numy(); J++ )
                    reaction_list[k].integrate ( &signal_list, i, J, get_sim_dt() );

        for ( k=0; k<signal_list.size(); k++ )
            signal_list[k]->integrate ( get_sim_dt() );

        for ( j=population->begin(); j!=population->end();)
        {
            if ( (*j)->marked_for_death() ) {
                Cell * c = (*j);
                //ceDestroyBody(c->get_body());
                *j = population->back();
                population->pop_back();
                delete c;
            }
            else
            {
                j++;
            }
        }

        // chemostat updates
        /*if ( chemostat_mode ) {

            for ( j=population->begin(); j!=population->end(); j++ ) {

                cpBodyApplyForce ( (*j)->get_shape()->body, chemostat_flow ( (*j)->get_x(), (*j)->get_y(), 250*get_sim_dt() ), cpv(0,0) );

                if ( out_of_bounds ( (*j)->get_x(), (*j)->get_y() ) ) {

                    Cell * c = (*j);
                    j = population->erase ( j );
                    delete c;

                }

            }

        }

        for(int i=0; i<3; i++){
            cpSpaceStep(space, get_chip_dt());
        }*/

        if(get_param("signals") == 1.0)
        {
            if (this->grid_type == "continuous" && (this->diff_method == "matrix" || this->diff_method == "gro_original")) {
                handler->update(this->diff_method, get_sim_dt());
            } else if (this->grid_type == "digital" && (this->diff_method == "digital")) {
                dhandler->update(this->diff_method, this->digital_prob);
            } else if (this->grid_type == "digital" && (this->diff_method == "digital_proportion")) {
                dhandler->update(this->diff_method, this->digital_prop);
            }
        }

        t += get_sim_dt();

        if ( print_rate > 0 && step % print_rate == 0 )
            print();

        step++;

    } else {

        emit_message ( "Population limit reached. Increase the population limit via the Simulation menu, or by setting the parameter \"population_max\" in your gro program." );
        set_stop_flag(true);

    }

    std::random_shuffle ( population->begin(), population->end() );
    //cout << "Pase!!! Ah espera... no, no toca eso: " << (*population)[0]->get_id() << endl;

    //randomize_population();


}

void World::add_signal ( Signal * s ) {

    signal_list.push_back ( s );

}

float World::get_signal_value ( Cell * c, int i ) {

    float s = c->get_size() / 3.0, a = c->get_theta();

    return (
                signal_list[i]->get ( (float) ( c->get_x() ), (float) ( c->get_y() ) ) +
                signal_list[i]->get ( (float) ( c->get_x() + s * cos ( a ) ), (float) ( c->get_y() + s * sin ( a ) ) ) +
                signal_list[i]->get ( (float) ( c->get_x() - s * cos ( a ) ), (float) ( c->get_y() - s * sin ( a ) ) )
                ) / 3.0;

}

void World::emit_signal ( Cell * c, int i, float ds ) {

    signal_list[i]->inc (  c->get_x(), c->get_y(), ds );

}

void World::absorb_signal ( Cell * c, int i, float ds ) {

    signal_list[i]->dec (  c->get_x(), c->get_y(), ds );

}

std::vector< std::vector<float> > * World::get_signal_matrix ( int i ) {

  return signal_list[i]->get_signal_matrix();

}

void World::print ( void ) {

    std::vector<Cell *>::iterator j;
    int i;

    for ( j=population->begin(); j!=population->end(); j++ ) {

        printf ( "%d, %d, ", (*j)->get_id(), step );

        for ( i=0; i<MAX_REP_NUM-1; i++ )
            printf ( "%f, ", (*j)->get_fluorescence ( i ) );

        printf ( "%f\n", (*j)->get_fluorescence ( MAX_REP_NUM-1 ) );

    }

}

void World::create_dirs ( const char * path , int n )
{
    int i = 0, j = 0;
    char * command = (char *)malloc(2048*sizeof(char));
    char * dirs = (char *)malloc(2048*sizeof(char));
    char * number = (char *)malloc(5*sizeof(char));
    strcpy(command, "mkdir ");
    strcat(command, path);
    dirs[0] = '{';
    j++;
    for ( i = 0; i < (n-1); i++ )
    {
        sprintf(number, "%d", i+1);
        strcat(dirs, number);
        strcat(dirs, ",");
    }
    sprintf(number, "%d", i+1);
    strcat(dirs, number);
    strcat(dirs, "}");
    strcat(command, dirs);
    system(command);
    free(command);
    free(dirs);
    free(number);
}

bool World::snapshot ( const char * path ) {

#ifndef NOGUI
    return calling_thread->snapshot(path);
#endif

}

#define NUM_BINS 12
static int bins[NUM_BINS+1];
static char histbuf[100];

void World::histogram ( float x, float y, float width, float height, int channel ) {

    int max_freq;
    std::vector<Cell *>::iterator i;
    int j;
    float val;

    for ( j=0; j<NUM_BINS; j++ )
        bins[j]=0;

    max_val = 0.01;
    for ( i=population->begin(); i!=population->end(); i++ ) {
        val = (*i)->get_rep ( channel ) / (*i)->get_size();
        if ( val > max_val )
            max_val = val;
    }

    for ( i=population->begin(); i!=population->end(); i++ ) {
        val = (*i)->get_rep ( channel ) / (*i)->get_size();
        bins[ (int) ( ( NUM_BINS * val) / max_val )]++;
    }

    max_freq = 1;
    for ( j=0; j<NUM_BINS; j++ ) {
        if ( bins[j] > max_freq )
            max_freq = bins[j];
    }
#if PORTED_TO_QT
    if ( channel == 0 )
        glColor3f ( 0.5f, 0.8f, 0.5f);
    else if ( channel == 1 )
        glColor3f ( 0.8f, 0.5f, 0.5f);
    else if ( channel == 2 )
        glColor3f ( 0.8f, 0.8f, 0.5f);

    glBegin(GL_QUADS);
    for ( j=0; j<NUM_BINS; j++ ) {
        glVertex2f ( x + width * (j+0.1) / NUM_BINS, y  );
        glVertex2f ( x + width * (j+0.1) / NUM_BINS, y + height * bins[j] / max_freq );
        glVertex2f ( x + width * (j+1) / NUM_BINS, y + height * bins[j] / max_freq );
        glVertex2f ( x + width * (j+1) / NUM_BINS, y );
    }
    glEnd();

    glLineWidth(1.0f);

    glBegin(GL_LINE_STRIP);
    glVertex2f ( x, y+height );
    glVertex2f ( x, y );
    glVertex2f ( x+width, y );
    glEnd();

    glBegin(GL_LINES);
    glVertex2f ( x+width, y ); glVertex2f ( x+width, y-4 );
    glVertex2f ( x, y+height ); glVertex2f ( x-4, y+height );
    glEnd();

    sprintf ( histbuf, "Channel %d", channel );
    drawString ( x, y + height + 8, histbuf );

    sprintf ( histbuf, "%d", max_freq );
    drawString ( x-24, y + height-4, histbuf );

    sprintf ( histbuf, "%.2f", max_val );
    drawString ( x+width-8, y - 16, histbuf );
#endif

}

void World::scatter ( float x, float y, float width, float height, int channel1, int channel2 ) {

    float max_val = 0.0;
    std::vector<Cell *>::iterator i;

    for ( i=population->begin(); i!=population->end(); i++ ) {
        if ( (*i)->get_rep ( channel1 ) / (*i)->get_size() > max_val )
            max_val = (*i)->get_rep ( channel1 ) / (*i)->get_size();
        if ( (*i)->get_rep ( channel2 ) / (*i)->get_size() > max_val )
            max_val = (*i)->get_rep ( channel2 ) / (*i)->get_size();
    }
#if PORTED_TO_QT
    glLineWidth(1.0f);
    glColor3f ( 0.8f, 0.8f, 0.8f);

    glBegin(GL_LINE_STRIP);
    glVertex2f ( x, y+height );
    glVertex2f ( x, y );
    glVertex2f ( x+width, y );
    glEnd();

    glBegin(GL_LINES);
    glVertex2f ( x+width, y ); glVertex2f ( x+width, y-4 );
    glVertex2f ( x, y+height ); glVertex2f ( x-4, y+height );
    glEnd();

    glPointSize ( 2.0 );

    glBegin(GL_POINTS);
    for ( i=population->begin(); i!=population->end(); i++ ) {
        glVertex2f ( x + ( (*i)->get_rep ( channel1 ) / (*i)->get_size() ) * width / max_val,
                     y + ( (*i)->get_rep ( channel2 ) / (*i)->get_size() ) * height / max_val );
    }
    glEnd();

    sprintf ( histbuf, "Channel %d vs. %d", channel1, channel2 );
    drawString ( x, y + height + 8, histbuf );
#endif
}

void World::randomize_population ( void )
{
    if(population->size() >= 2)
    {
        std::vector<Cell *> * tmp_population;
        std::vector<Cell *>::iterator it;
        tmp_population = new std::vector<Cell *>;

        int n = population->size(), r=0;

        while(n>0)
        {
            r = rand()%n;
            it = population->begin();
            std::advance(it,r);
            tmp_population->push_back(*it);
            population->erase(it);
            n--;
        }
        //REVISAR
        delete population;
        population = tmp_population;
    }
}

void World::select_cells ( int x1, int y1, int x2, int y2 ) {

    std::vector<Cell *>::iterator i;

    int
            X1 = (1/zoom)*min ( x1, x2 ),
            X2 = (1/zoom)*max ( x1, x2 ),
            Y1 = (1/zoom)*min ( y1, y2 ),
            Y2 = (1/zoom)*max ( y1, y2 );

    for ( i=population->begin(); i!=population->end(); i++ ) {
        if ( X1-5 <= (*i)->get_x() && (*i)->get_x() <= X2+5 && Y1-5 <= (*i)->get_y() && (*i)->get_y() <= Y2+10 ) {
            (*i)->select();
        }
    }

}

void World::deselect_all_cells ( void ) {

    std::vector<Cell *>::iterator i;

    for ( i=population->begin(); i!=population->end(); i++ ) {
        (*i)->deselect();
    }

}

void World::select_random_cell ( void ) {

    std::vector<Cell *>::iterator i;
    int whichOne = rand()%population->size();
    int j = 0;


    for ( i=population->begin(); i!=population->end() && j != whichOne; i++ ) {
        j++;
    }

    (*i)->select();

}

int World::check_gen_cond_pop ( std::vector<uint64_t> cond ) {

    int n = 0;
    std::vector<Cell *>::iterator i;
    for ( i=population->begin(); i!=population->end(); i++ )
    {
        n += (*i)->check_gen_condition(cond);
    }
    return n;
}

int World::check_plasmid_cond_pop ( std::vector<std::pair<std::string, int>> cond ) {

    int n = 0;
    std::vector<Cell *>::iterator i;
    for ( i=population->begin(); i!=population->end(); i++ )
    {
        n += (*i)->check_plasmid_condition(cond);
    }
    return n;
}

void World::dump ( FILE * fp ) {

    std::vector<Cell *>::iterator i;

    fprintf ( fp, "id, x, y, theta, volume, gfp, rfp, yfp, cfp\n" );

    for ( i=population->begin(); i!=population->end(); i++ ) {
        fprintf ( fp, "%d, %f, %f, %f, %f, %d, %d, %d, %d\n",
                  (*i)->get_id(), (*i)->get_x(), (*i)->get_y(), (*i)->get_theta(), (*i)->get_volume(),
                  (*i)->get_rep(GFP), (*i)->get_rep(RFP), (*i)->get_rep(YFP), (*i)->get_rep(CFP) );
    }

}

float World::dump_left ( void ) {

    float min = 50000, x = 0;
    std::vector<Cell *>::iterator i;

    for ( i=population->begin(); i!=population->end(); i++ ) {
        x = (*i)->get_x();
        if(x < min)
        {
            min = x;
        }
    }
    return( min );
}

float World::dump_right ( void ) {

    float max = -50000, x = 0;
    std::vector<Cell *>::iterator i;

    for ( i=population->begin(); i!=population->end(); i++ ) {
        x = (*i)->get_x();
        if(x > max)
        {
            max = x;
        }
    }
    return( max );

}

float World::dump_top ( void ) {

    float min = 50000, y = 0;
    std::vector<Cell *>::iterator i;

    for ( i=population->begin(); i!=population->end(); i++ ) {
        y = (*i)->get_y();
        if(y < min)
        {
            min = y;
        }
    }
    return( min );
}

float World::dump_bottom ( void ) {

    float max = -50000, y = 0;
    std::vector<Cell *>::iterator i;

    for ( i=population->begin(); i!=population->end(); i++ ) {
        y = (*i)->get_y();
        if(y > max)
        {
            max = y;
        }
    }
    return( max );

}

FnPointer World::get_action( std::string str ) {
    std::map<std::string,FnPointer>::iterator it;
    if(map_actions.find(str) == map_actions.end())
        return NULL;

    it = map_actions.find(str);
    return (*it).second;
}

bool World::get_output_started()
{
    return this->output_started;
}

void World::set_output_started(bool v)
{
    this->output_started = v;
}

bool World::get_output_started2()
{
    return this->output_started2;
}

void World::set_output_started2(bool v)
{
    this->output_started2 = v;
}

void World::add_barrier ( float x1, float y1, float x2, float y2 ) {

    Barrier * b = new Barrier;

    b->x1 = x1;
    b->y1 = y1;
    b->x2 = x2;
    b->y2 = y2;

    barriers->push_back( *b );

}
