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

#include "EColi.h"
#include "Programs.h"
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

EColi::EColi ( World * w, float x, float y, float a) : Cell ( w ) {

  //float sd = get_param ( "ecoli_division_size_variance" );
  float sd = get_param ( "ecoli_division_size_variance" );
  float sd_init = sd*4;
  div_length = get_param ( "ecoli_division_length_mean" ) - sd + frand() * sd * 2;
  init_length = get_param ("ecoli_init_length") - sd_init + frand() * sd_init * 2;
  length= init_length;
  d_length = 0;
  d_vol = 0;

  body = ceCreateBody(w->get_space(), DEFAULT_ECOLI_SCALE * init_length, ceGetVector2(x,y), a);

  ceSetData(body, this);

  int i;
  for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = 0;
  for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = 0;

  div_count = 0;
  force_div = false;

  this->my_available = 0;

  this->gt_inst = (log(this->div_length/this->init_length))/get_param ( "ecoli_growth_rate" );

}

EColi::EColi ( World * w, ceBody* body ) : Cell ( w ) {

  float sd = get_param ( "ecoli_division_size_variance" );
  div_length = get_param ( "ecoli_division_length_mean" ) - sd + frand() * sd * 2;
  init_length = body->length / DEFAULT_ECOLI_SCALE;
  length= init_length;
  d_length = 0;
  d_vol = 0;

  this->body = body;
  ceSetData(body, this);

  int i;
  for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = 0;
  for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = 0;

  div_count = 0;
  force_div = false;

  this->my_available = 0;

  this->gt_inst = (log(this->div_length/this->init_length))/get_param ( "ecoli_growth_rate" );

}

#ifndef NOGUI

void EColi::render ( Theme * theme, GroPainter * painter ) {

  float length = body->length;
  ceVector2 center = body->center;

  //cpPolyShape * poly = (cpPolyShape *) shape;
  //int count = poly->numVerts;

  /*double
    gfp = ( rep[GFP] / volume - world->get_param ( "gfp_saturation_min" ) ) / ( world->get_param ( "gfp_saturation_max" ) - world->get_param ( "gfp_saturation_min" ) ),
    rfp = ( rep[RFP] / volume - world->get_param ( "rfp_saturation_min" ) ) / ( world->get_param ( "rfp_saturation_max" ) - world->get_param ( "rfp_saturation_min" ) ),
    yfp = ( rep[YFP] / volume - world->get_param ( "yfp_saturation_min" ) ) / ( world->get_param ( "yfp_saturation_max" ) - world->get_param ( "yfp_saturation_min" ) ),
    cfp = ( rep[CFP] / volume - world->get_param ( "cfp_saturation_min" ) ) / ( world->get_param ( "cfp_saturation_max" ) - world->get_param ( "cfp_saturation_min" ) );*/

  /*double
    gfp = ( rep[GFP] / length - world->get_param ( "gfp_saturation_min" ) ) / ( world->get_param ( "gfp_saturation_max" ) - world->get_param ( "gfp_saturation_min" ) ),
    rfp = ( rep[RFP] / length - world->get_param ( "rfp_saturation_min" ) ) / ( world->get_param ( "rfp_saturation_max" ) - world->get_param ( "rfp_saturation_min" ) ),
    yfp = ( rep[YFP] / length - world->get_param ( "yfp_saturation_min" ) ) / ( world->get_param ( "yfp_saturation_max" ) - world->get_param ( "yfp_saturation_min" ) ),
    cfp = ( rep[CFP] / length - world->get_param ( "cfp_saturation_min" ) ) / ( world->get_param ( "cfp_saturation_max" ) - world->get_param ( "cfp_saturation_min" ) );*/

  double
    gfp = ( rep[GFP]  - world->get_param ( "gfp_saturation_min" ) ) / ( world->get_param ( "gfp_saturation_max" ) - world->get_param ( "gfp_saturation_min" ) ),
    rfp = ( rep[RFP]  - world->get_param ( "rfp_saturation_min" ) ) / ( world->get_param ( "rfp_saturation_max" ) - world->get_param ( "rfp_saturation_min" ) ),
    yfp = ( rep[YFP]  - world->get_param ( "yfp_saturation_min" ) ) / ( world->get_param ( "yfp_saturation_max" ) - world->get_param ( "yfp_saturation_min" ) ),
    cfp = ( rep[CFP]  - world->get_param ( "cfp_saturation_min" ) ) / ( world->get_param ( "cfp_saturation_max" ) - world->get_param ( "cfp_saturation_min" ) );

  theme->apply_ecoli_edge_color ( painter, is_selected() );

  QColor col;

  col.setRgbF( qMin(1.0,rfp + yfp),
               qMin(1.0,gfp + yfp + cfp),
               qMin(1.0,cfp),
               0.75);

  /*col.setRgbF( qMin(1.0,rfp),
               qMin(1.0,gfp),
               qMin(1.0,cfp),
               0.75);*/

  /*painter->setBrush(col);

  QPainterPath path;
  cpVect v = poly->tVerts[0];
  path.moveTo(v.x,v.y);

  for ( int i=1; i<count; i++ ) {
      v = poly->tVerts[i];
      path.lineTo(v.x,v.y);
  }

  path.closeSubpath();
  painter->drawPath(path);*/

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

void EColi::update ( void ) {

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
    d_vol = d_length;

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

    length = frac * length;
    init_length = length;

    float da = 0.25 * (frand()-0.5);

    ceBody* daughterBody = ceDivideBody(body, da, frac);

    EColi * daughter = new EColi ( world, daughterBody );
 
    for(auto copy : *(this->getPlasmidList()->getPlasmids()))
    {
        daughter->getPlasmidList()->insertPlasmid(copy->getName(), copy);
        for(auto ops : *(copy->getOperons()))
        {
            if(!ops->getNoisef())
            {
                ops->setNoise(world->get_time(), world->get_sim_dt());
            }
        }
    }

    for(auto pls : *(daughter->getPlasmidList()->getPlasmids()))
    {
        pls->setParent(daughter->getPlasmidList());
        pls->setEnvPlasmid(daughter->getEnvPlasmid());
        for(auto ops : *(pls->getOperons()))
        {
            ops->getPromoter()->setListPlasmid(daughter->getPlasmidList());
        }
        daughter->getPlasmidList()->setRNG(world->getRNG());
        for(auto ops : *(pls->getOperons()))
        {
            ops->setNoise(world->get_time(), world->get_sim_dt());
        }
    }

    std::vector<GenCell*> temp_list_dec;
    std::vector<GenCell*> temp_list_act;
    for(auto pls : *(daughter->getPlasmidList()->getPlasmids()))
    {
        temp_list_act.clear();
        temp_list_dec.clear();
        for(auto ops: *(pls->getOperons()))
        {
            for(auto gen: (*(ops->getGens())))
            {
                if((gen->getTime() >= 0) && gen->getState() == 1)
                {
                    temp_list_dec.push_back(gen);
                }

                if((gen->getTime() >= 0) && gen->getState() == 0)
                {
                    temp_list_act.push_back(gen);
                }
            }
        }
        pls->setActList(temp_list_act);
        pls->setDecList(temp_list_dec);
    }

    daughter->set_param_map ( get_param_map() );

    if ( gro_program != NULL ) {
      daughter->set_gro_program ( split_gro_program ( gro_program, frac ) );
    }

    daughter->init ( q, rep, 1-frac );

    int i;

    for ( i=0; i<MAX_STATE_NUM; i++ ) q[i] = (int) ceil(frac*q[i]);
    for ( i=0; i<MAX_REP_NUM; i++ ) rep[i] = (int) ceil(frac*rep[i]);

    set_division_indicator(true);
    daughter->set_division_indicator(true);
    daughter->set_daughter_indicator(true);



    return daughter;

  } else return NULL;

}
