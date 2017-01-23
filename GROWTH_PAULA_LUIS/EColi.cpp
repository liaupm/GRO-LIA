/////////////////////////////////////////////////////////////////////////////////////////
//
// gro is protected by the UW OPEN SOURCE LICENSE, which is summarized here.
// Please see the file LICENSE.txt for the complete license.
//
// THE SOFTWARE (AS DEFINED BELOW) AND HARDWARE DESIGNS (AS DEFINED BELOW) IS PROVIDED
// UNDER THE TERMS OF THISS OPEN SOURCE LICENSE (“LICENSE”).  THE SOFTWARE IS PROTECTED
// BY COPYRIGHT AND/OR OTHER APPLICABLE LAW.  ANY USE OF THISS SOFTWARE OTHER THAN AS
// AUTHORIZED UNDER THISS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
//
// BY EXERCISING ANY RIGHTS TO THE SOFTWARE AND/OR HARDWARE PROVIDED HERE, YOU ACCEPT AND
// AGREE TO BE BOUND BY THE TERMS OF THISS LICENSE.  TO THE EXTENT THISS LICENSE MAY BE
// CONSIDERED A CONTRACT, THE UNIVERSITY OF WASHINGTON (“UW”) GRANTS YOU THE RIGHTS
// CONTAINED HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.
//
// TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
//
//

#include "EColi.h"
#include "Programs.h"
#include "Operon.h"
#include <iostream>
#include <math.h>
#include <limits>

#ifdef _WIN32
#include "Rands.h"
#endif //_WIN32

using namespace std;

#define FMULT 0.125

void EColi::compute_parameter_derivatives ( void ) {


}

EColi::EColi ( World * w, float x, float y, float a, float v ) :
    Cell ( w )
{
    float sd = get_param ( "ecoli_division_size_variance" );
    float sd_init = sd*4;
    div_length = get_param ( "ecoli_division_size_mean" ) - sd + frand() * sd * 2;
    init_length = get_param ("ecoli_init_size") - sd_init + frand() * sd_init * 2;
    length= init_length;
    d_length = 0;

    body = ceCreateBody(w->get_space(), DEFAULT_ECOLI_SCALE * init_length, ceGetVector2(x,y), a);
    ceSetData(body, this);

    int i;
    for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = 0;
    for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = 0;

    div_count = 0;
    force_div = false;

    this->gt_inst = (log(this->div_length/this->init_length))/get_param ( "ecoli_growth_rate" );


}

EColi::EColi ( World * w, float v, ceBody* body ) : Cell ( w )
{
    float sd = get_param ( "ecoli_division_size_variance" );
    div_length = get_param ( "ecoli_division_size_mean" ) - sd + frand() * sd * 2;
    init_length = body->length / DEFAULT_ECOLI_SCALE;
    length= init_length;
    d_length = 0;

    this->body = body;
    ceSetData(body, this);

    int i;
    for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = 0;
    for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = 0;

    div_count = 0;
    force_div = false;

    this->gt_inst = (log(this->div_length/this->init_length))/get_param ( "ecoli_growth_rate" );

}

#ifndef NOGUI

void EColi::render ( Theme * theme, GroPainter * painter ) {
    float length = body->length;
    ceVector2 center = body->center;

    double
            gfp = ( rep[GFP] / length - world->get_param ( "gfp_saturation_min" ) ) / ( world->get_param ( "gfp_saturation_max" ) - world->get_param ( "gfp_saturation_min" ) ),
            rfp = ( rep[RFP] / length - world->get_param ( "rfp_saturation_min" ) ) / ( world->get_param ( "rfp_saturation_max" ) - world->get_param ( "rfp_saturation_min" ) ),
            yfp = ( rep[YFP] / length - world->get_param ( "yfp_saturation_min" ) ) / ( world->get_param ( "yfp_saturation_max" ) - world->get_param ( "yfp_saturation_min" ) ),
            cfp = ( rep[CFP] / length - world->get_param ( "cfp_saturation_min" ) ) / ( world->get_param ( "cfp_saturation_max" ) - world->get_param ( "cfp_saturation_min" ) );

    theme->apply_ecoli_edge_color ( painter, is_selected() );



    QColor col;

    col.setRgbF( qMin(1.0,rfp + yfp),
                 qMin(1.0,gfp + yfp + cfp),
                 qMin(1.0,cfp),
                 1.0f);

    painter->translate(center.x, center.y);
    painter->rotate(body->rotation*CE_RADIAN);
    painter->translate(-center.x, -center.y);

    painter->setBrush(col);
    /*if(d_length <= 0.0f)
    {

        painter->setBrush( QColor ("#4172C4"));


    }*/
    painter->drawRoundedRect(center.x - length/2, center.y - WIDTH/2, length, WIDTH, WIDTH/2, WIDTH/2);

    painter->translate(center.x, center.y);
    painter->rotate(-body->rotation*CE_RADIAN);
    painter->translate(-center.x, -center.y);
}

#endif

void EColi::update ( void )
{

    float threshold = get_param("nutrient_consumption_rate");
    this->monod = 1.0;
    this->d_length = 0;


    if(get_param("nutrients") == 1.0)
    {
        if (available <= 0) {
             available = 0;
             this->monod = 0;
             d_length = 0;
         } else {
            this->monod = ( available/(available+threshold) );
            d_length = get_param ( "ecoli_growth_rate" ) * this->monod * world->get_sim_dt() * this->cross_input_coefficient * length;
         }
    }
    else
    {
        this->monod = 1.0;
        d_length = get_param ( "ecoli_growth_rate" ) *this->monod * world->get_sim_dt() * this->cross_input_coefficient * length;
    }

    this->gt_inst = (log(this->div_length/this->init_length))/(get_param ( "ecoli_growth_rate" )* this->monod * this->cross_input_coefficient);

    length += d_length;

    ceGrowBody(body, 10*d_length);

    if (length > div_length)
    {
        div_count++;
    }

    if ( program != NULL )
         program->update ( world, this );
}

Value * EColi::eval ( Expr * e ) {

    if ( program != NULL )
        program->eval ( world, this, e );

}

EColi * EColi::divide ( void ) {

    if ( div_count >= 1 || force_div ) {


        div_count = 0;
        force_div = false;

        float frac = 0.5 + 0.1 * ( frand() - 0.5 );

        float daugtherLength = (1-frac)*length;

        length = frac * length;
        init_length = length;

        float da = 0.25 * (frand()-0.5);

        // OJO!!! Suponemos da en radianes!!!
        ceBody* daughterBody = ceDivideBody(body, da, frac);

        EColi * daughter = new EColi ( world, daugtherLength, daughterBody );

        int i,j;

        for( i=0; i<this->nPlasmids(); i++)
        {
            daughter->plasmids[i] = this->plasmids[i];
            daughter->received_plasmids[i] = this->received_plasmids[i];
            daughter->just_conjugated[i] = this->just_conjugated[i];
            daughter->just_conjugated_reset[i] = this->just_conjugated_reset[i];
            daughter->eex[i] = this->eex[i];
        }

        for( i=0; i<this->num_proteins; i++)
        {
            daughter->internal_proteins[i]=false;
            daughter->internal_RNAs[i]=this->internal_RNAs[i];
            //daughter->internal_RNAs[i]=false;
            daughter->degr_prot[i]=this->degr_prot[i];
            daughter->degr_rna[i]=this->degr_rna[i];
        }

        for ( i=0; i<world->num_operons(); i++ ){
            for(j=0;j<num_proteins;j++){
                daughter->last_state[i][j] =this->last_state[i][j];
                daughter->last_state_rna[i][j] =this->last_state_rna[i][j];
                //daughter->last_state[i][j] = false;
                daughter->rna_operon[i][j] =this->rna_operon[i][j];
                daughter->prot_actv_times[i][j]=this->prot_actv_times[i][j];
                daughter->prot_degr_times[i][j]=this->prot_degr_times[i][j];
                daughter->rna_actv_times[i][j]=this->rna_actv_times[i][j];
                daughter->rna_degr_times[i][j]=this->rna_degr_times[i][j];
                //this->last_state[i][j]=false;

                daughter->proteins_operon[i][j] = false;
                if(this->proteins_operon[i][j]){
                    Operon * op = world->get_operon(i);
                    double up_p = op->get_prot_up_time(j);
                    double up_p_err = op->get_prot_up_time_err(j);
                    double up_r = op->get_rna_up_time(j);
                    double up_r_err = op->get_rna_up_time_err(j);
                    daughter->prot_actv_times[i][j]=world->get_time() + (drand48() * ((up_p + up_p_err)-(up_p-up_p_err))+(up_p-up_p_err))/2; //la mitad del tiempo de activacion normal
                    daughter->rna_actv_times[i][j]=world->get_time() + (drand48() * ((up_r + up_r_err)-(up_r-up_r_err))+(up_r-up_r_err))/2; //la mitad del tiempo de activacion normal
                    this->prot_actv_times[i][j]=world->get_time() + (drand48() * ((up_p + up_p_err)-(up_p-up_p_err))+(up_p-up_p_err))/2; //la mitad del tiempo de activacion normal
                    this->rna_actv_times[i][j]=world->get_time() + (drand48() * ((up_r + up_r_err)-(up_r-up_r_err))+(up_r-up_r_err))/2; //la mitad del tiempo de activacion normal
                    this->proteins_operon[i][j] = false;
                }
            }
        }


        daughter->set_param_map ( get_param_map() );

        if ( gro_program != NULL ) {
            daughter->set_gro_program ( split_gro_program ( gro_program, frac ) );
        }

        daughter->init ( q, rep, 1-frac );



        for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = (int) ceil(frac*q[i]);
        for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = (int) ceil(frac*rep[i]);

        set_division_indicator(true);
        daughter->set_division_indicator(true);
        daughter->set_daughter_indicator(true);

        return daughter;

    } else return NULL;

}

