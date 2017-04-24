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

#include "Micro.h"
#include "EColi.h"
//#include "Yeast.h"
#include "Programs.h"
#include <vector>
#include "Rands.h"
#include <map>
#include "cg/Operon.hpp"

static gro_Program * current_gro_program = NULL;
static Cell * current_cell = NULL;

bool parse_error = false;

Program * split_gro_program ( Program * parent, float frac ) {

  Program * child = parent->copy();
  SymbolTable * symtab = parent->get_symtab();
  SymbolTable * new_symtab = child->get_symtab();

  //printf ( "before: %d %d\n", symtab->get("yfp")->int_value(), new_symtab->get("yfp")->int_value() );

  symtab->divide ( new_symtab, frac );

  //printf ( "after: %d %d\n", symtab->get("yfp")->int_value(), new_symtab->get("yfp")->int_value() );

  return child;

}

/*Value * new_ecoli ( std::list<Value *> * args, Scope * s ) {

  std::list<Value *>::iterator i = args->begin();
  Value * settings = *i; i++;

  ASSERT ( args->size() == 2 );

  Program * prog = (*i)->program_value()->copy(); // this copy is deleted in ~Cell?
  //printf ( ">> prog = %x in new_ecoli\n", prog );

  World * world = current_gro_program->get_world();

  float x = 0, y = 0, theta = 0;

  if ( settings->get_type() == Value::RECORD ) {

    if ( settings->getField ( "x" ) )
      x = settings->getField ( "x" )->real_value();

    if ( settings->getField ( "y" ) )
      y = settings->getField ( "y" )->real_value();

    if ( settings->getField ( "theta" ) )
      theta = settings->getField ( "theta" )->real_value();

  } else {

    fprintf ( stderr, "First argument to 'ecoli' should be a record\n" );
    exit ( -1 );

  }

  EColi * c = new EColi ( world, x, y, theta );

  current_cell = c;
  c->set_gro_program ( prog ); // prog is deleted if/when the cell is deleted in ~Cell
  world->add_cell ( c );
  prog->init_params ( current_gro_program->get_scope() );
  prog->init ( current_gro_program->get_scope() );
  current_cell = NULL;

  return new Value(Value::UNIT);

}*/

Value * new_ecoli ( std::list<Value *> * args, Scope * s ) {

    std::list<Value *>::iterator i = args->begin();
    Value * settings = *i; i++;

    ASSERT ( args->size() == 2 || args->size() == 3  );

    Value* Plas;
    /*Value* Prot;
    Value* mrna;*/

    if(args->size() == 3)
    {
        Plas = *i;
        //Prot = *i; i++;
        //mrna = *i;i++;
    }

    Program * prog = (*i)->program_value()->copy();

    World * world = current_gro_program->get_world();

    float x = 0, y = 0, theta = 0;

    if ( settings->get_type() == Value::RECORD ) {

        if ( settings->getField ( "x" ) )
            x = settings->getField ( "x" )->real_value();

        if ( settings->getField ( "y" ) )
            y = settings->getField ( "y" )->real_value();

        if ( settings->getField ( "theta" ) )
            theta = settings->getField ( "theta" )->real_value();

    } else {

        fprintf ( stderr, "First argument to 'ecoli' should be a record\n" );
        exit ( -1 );
    }

    EColi * c = new EColi ( world, x, y, theta );

    if(args->size() == 3)
    {
        for ( i=Plas->list_value()->begin(); i != Plas->list_value()->end(); i++ )
        {
            c->getGenome().add(world->getPlasmidPool()->getPlasmidByName((*i)->string_value()));
        }

        /*for ( i=Prot->list_value()->begin(); i != Prot->list_value()->end(); i++ )
        {
            std::map<std::string,int> plasHaveProt = c->getPlasmidList()->isGenExist((*i)->string_value());
            if(!plasHaveProt.empty())
            {
                for(auto const& ent1 : plasHaveProt)
                {
                    c->getPlasmidList()->getPlasmidById(ent1.first)->activateGen((*i)->string_value());
                }
            }
        }

        for ( i=mrna->list_value()->begin(); i != mrna->list_value()->end(); i++ ) {
            std::map<std::string,int> plasHaveRna = c->getPlasmidList()->isGenExist((*i)->string_value());
            if(!plasHaveRna.empty())
            {
                for(auto const& ent1 : plasHaveRna)
                {
                    c->getPlasmidList()->getPlasmidById(ent1.first)->activateGen((*i)->string_value());
                }
            }
        }*/
    }

    current_cell = c;
    c->set_gro_program ( prog ); // prog is deleted if/when the cell is deleted in ~Cell
    world->add_cell ( c );
    prog->init_params ( current_gro_program->get_scope() );
    prog->init ( current_gro_program->get_scope() );
    //c->set_initial_last_state();
    current_cell = NULL;

    return new Value(Value::UNIT);

}

Value * new_ecolis_random_circle ( std::list<Value *> * args, Scope * s ) {

    std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator h;
    int j = 0;

    ASSERT ( args->size() == 5 || args->size() == 6 );

    int n_ecoli = (*i)->int_value();
    i++;
    double x_center = (*i)->real_value();
    i++;
    double y_center = (*i)->real_value();
    i++;
    double max_radius = (*i)->real_value();
    i++;

    Value* Plas;
    /*Value* Prot;
    Value* mrna;*/

    std::vector<cg::Plasmid*>* insert_plasmids;

    if(args->size() == 6)
    {
        Plas = *i; i++;
        //Prot = *i; i++;
        //mrna = *i; i++;
    }

    while(j < n_ecoli)
    {
        Program * prog = (*i)->program_value()->copy(); // this copy is deleted in ~Cell?

        World * world = current_gro_program->get_world();

        float x = 0, y = 0, theta = 0;

        x = (float) fRand((x_center-max_radius), (x_center+max_radius));
        y = (float) fRand((y_center-max_radius), (y_center+max_radius));

        while((((x-x_center)*(x-x_center)) + ((y-y_center)*(y-y_center))) > (max_radius*max_radius))
        {
            x = (float) fRand((x_center-max_radius), (x_center+max_radius));
            y = (float) fRand((y_center-max_radius), (y_center+max_radius));
        }

        theta = (float) fRand(0.0, 6.28);

        EColi * c = new EColi ( world, x, y, theta );

        if(args->size() == 6)
        {
            for ( h=Plas->list_value()->begin(); h != Plas->list_value()->end(); h++ )
            {
                c->getGenome().add(world->getPlasmidPool()->getPlasmidByName((*h)->string_value()));
            }

            /*for ( h=Prot->list_value()->begin(); h != Prot->list_value()->end(); h++ )
            {
                std::map<std::string,int> plasHaveProt = c->getPlasmidList()->isGenExist((*h)->string_value());
                if(!plasHaveProt.empty())
                {
                    for(auto const& ent1 : plasHaveProt)
                    {
                        c->getPlasmidList()->getPlasmidById(ent1.first)->activateGen((*h)->string_value());
                    }
                }
            }

            for ( h=mrna->list_value()->begin(); h != mrna->list_value()->end(); h++ ) {
                std::map<std::string,int> plasHaveRna = c->getPlasmidList()->isGenExist((*h)->string_value());
                if(!plasHaveRna.empty())
                {
                    for(auto const& ent1 : plasHaveRna)
                    {
                        c->getPlasmidList()->getPlasmidById(ent1.first)->activateGen((*h)->string_value());
                    }
                }
            }*/
        }

        current_cell = c;
        c->set_gro_program ( prog ); // prog is deleted if/when the cell is deleted in ~Cell
        world->add_cell ( c );
        prog->init_params ( current_gro_program->get_scope() );
        prog->init ( current_gro_program->get_scope() );
        current_cell = NULL;
        j++;
    }

    return new Value(Value::UNIT);
}

Value * new_ecolis_random_circle_growth ( std::list<Value *> * args, Scope * s ) {

    std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator h;
    int j = 0;

    ASSERT ( args->size() == 7 || args->size() == 8 );

    int n_ecoli = (*i)->int_value();
    i++;
    double x_center = (*i)->real_value();
    i++;
    double y_center = (*i)->real_value();
    i++;
    double max_radius = (*i)->real_value();
    i++;
    double gt = (*i)->real_value();
    i++;

    Value* div_params;
    div_params = *i;
    i++;

    Value* Plas;

    /*Value* Prot;
    Value* mrna;*/

    std::vector<cg::Plasmid*>* insert_plasmids;

    if(args->size() == 8)
    {
        Plas = *i; i++;
        //Prot = *i; i++;
        //mrna = *i; i++;
    }

    while(j < n_ecoli)
    {
        Program * prog = (*i)->program_value()->copy(); // this copy is deleted in ~Cell?

        World * world = current_gro_program->get_world();

        float x = 0, y = 0, theta = 0;

        x = (float) fRand((x_center-max_radius), (x_center+max_radius));
        y = (float) fRand((y_center-max_radius), (y_center+max_radius));

        while((((x-x_center)*(x-x_center)) + ((y-y_center)*(y-y_center))) > (max_radius*max_radius))
        {
            x = (float) fRand((x_center-max_radius), (x_center+max_radius));
            y = (float) fRand((y_center-max_radius), (y_center+max_radius));
        }

        theta = (float) fRand(0.0, 6.28);

        h = div_params->list_value()->begin();

        float sm = (float)((*h)->real_value());
        h++;
        float sd = (float)((*h)->real_value());
        //world->set_param("ecoli_division_size_variance",(float)((*h)->real_value()));

        EColi * c = new EColi ( world, x, y, theta, sm, sd);

        c->set_param("ecoli_growth_rate",gt);



        if(args->size() == 8)
        {
            for ( h=Plas->list_value()->begin(); h != Plas->list_value()->end(); h++ )
            {
                c->getGenome().add(world->getPlasmidPool()->getPlasmidByName((*h)->string_value()));
            }
        }

        current_cell = c;
        c->set_gro_program ( prog ); // prog is deleted if/when the cell is deleted in ~Cell
        world->add_cell ( c );
        prog->init_params ( current_gro_program->get_scope() );
        prog->init ( current_gro_program->get_scope() );
        current_cell = NULL;
        j++;
    }

    return new Value(Value::UNIT);
}

/*
Value * new_yeast ( std::list<Value *> * args, Scope * s ) {

  std::list<Value *>::iterator i = args->begin();
  Value * settings = *i; i++;

  ASSERT ( args->size() == 2 );

  Program * prog = (*i)->program_value()->copy(); // this copy is deleted in ~Cell?
  //printf ( ">> prog = %x in new_ecoli\n", prog );

  World * world = current_gro_program->get_world();

  float x = 0, y = 0, theta = 0, vol;

  if ( settings->get_type() == Value::RECORD ) {

    if ( settings->getField ( "x" ) )
      x = settings->getField ( "x" )->real_value();

    if ( settings->getField ( "y" ) )
      y = settings->getField ( "y" )->real_value();

    if ( settings->getField ( "theta" ) )
      theta = settings->getField ( "theta" )->real_value();

    if ( settings->getField ( "volume" ) )
      vol = settings->getField ( "volume" )->real_value();
    else
      vol = 1.0;

  } else {

    fprintf ( stderr, "First argument to 'yeast' should be a record\n" );
    exit ( -1 );

  }

  Yeast * c = new Yeast ( world, x, y, theta, vol, false );

  current_cell = c;
  c->set_gro_program ( prog ); // prog is deleted if/when the cell is deleted in ~Cell
  world->add_cell ( c );
  prog->init_params ( current_gro_program->get_scope() );
  prog->init ( current_gro_program->get_scope() );
  current_cell = NULL;

  return new Value(Value::UNIT);

}

*/

/////////////////////////////////////////////////////////////////////////////////////////////
// PROSPEC - GENE EXPRESSION
// EXTERNAL GRO FUNCTIONS
//

Value *new_operon (std::list<Value *> * args, Scope * s )
{
    World * world = current_gro_program->get_world();

    ASSERT ( args->size() == 1);

    cg::Promoter::Gate gate = cg::Promoter::Gate::AND;
    int counter = 0;
    //int which_riboswitch = 0;
    std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator j,k,l;
    Value * data = *i;
    std::list<Value*> * proteins = NULL;
    //std::list<Value*> * rnas = NULL;
    std::list<Value*> * transcription_factors = NULL;
    //std::list<Value*> * riboswitches = NULL;
    //std::list<Value*> * temp_riboswitch = NULL;
    std::list<Value*> * protein_sigma = NULL;
    std::list<Value*> * protein_times = NULL;
    //std::list<Value*> * rna_sigma = NULL;
    //std::list<Value*> * rna_times = NULL;
    Value * promoter = NULL;
    Value* noise = NULL;
    float onNoise = 0.0;
    float offNoise = 0.0;
    float noiseTime = 0.0;

    Value * proteinAtimes = NULL;
    Value * proteinDtimes = NULL;
    //Value * rnaAtimes = NULL;
    //Value * rnaDtimes = NULL;

    std::string circuit;

    std::string temp_tf;

    std::string output_prot;
    float act_time = 0;
    float act_var_time = 0;
    float deg_time = std::numeric_limits<float>::max();
    float deg_var_time = 0;

    float stored_deg_time = 0;

    //std::string riboswitch_location_gene;
    //map<std::string,int> riboMap;

    std::string operon_name;
    operon_data op_data;


    counter = 0;
    operon_name.clear();
    circuit.clear();
    temp_tf.clear();
    //riboswitch_location_gene.clear();

    if ( data->get_type() == Value::RECORD )
    {
        if ( data->getField ( "name" ) && data->getField("name")->get_type() == Value::STRING)
        {
            operon_name = data->getField ( "name" )->string_value();
        }
        /*if(operon_name.empty())
        {
            int i = 0;
            i = (world->get_globalPlasmidList()->getPlasmidById("listPP0")->getSize());
            std::string base = "Operon";
            operon_name.assign(base);
            operon_name.append(std::to_string(i));
        }*/

        //operon_name

        if ( data->getField ( "proteins" ) && data->getField("proteins")->get_type() == Value::LIST)
        {
            proteins = data->getField ( "proteins" )->list_value();
        }
        /*if ( data->getField ( "rnas" ) && data->getField("rnas")->get_type() == Value::LIST)
        {
            rnas = data->getField ( "rnas" )->list_value();
        }*/
        if ( data->getField ( "promoter" ) && data->getField("promoter")->get_type() == Value::RECORD)
        {
            promoter = data->getField ( "promoter" );
            if(promoter != NULL)
            {
                if( promoter->getField ( "function" ) && promoter->getField ( "function" )->get_type() == Value::STRING)
                {
                    circuit = promoter->getField( "function" )->string_value();
                    if(!circuit.empty())
                    {
                        if(circuit.compare("TRUE") == 0 || circuit.compare("true") == 0)
                        {
                            gate = cg::Promoter::Gate::OPEN;
                        }
                        else if(circuit.compare("FALSE") == 0 || circuit.compare("false") == 0)
                        {
                            gate = cg::Promoter::Gate::CLOSED;
                        }
                        else if(circuit.compare("AND") == 0 || circuit.compare("and") == 0)
                        {
                            gate = cg::Promoter::Gate::AND;
                        }
                        else if(circuit.compare("NAND") == 0 || circuit.compare("nand") == 0)
                        {
                            gate = cg::Promoter::Gate::NAND;
                        }
                        else if(circuit.compare("OR") == 0 || circuit.compare("or") == 0)
                        {
                            gate = cg::Promoter::Gate::OR;
                        }
                        else if(circuit.compare("XOR") == 0 || circuit.compare("xor") == 0)
                        {
                            gate = cg::Promoter::Gate::XOR;
                        }
                        else if(circuit.compare("YES") == 0 || circuit.compare("yes") == 0)
                        {
                            gate = cg::Promoter::Gate::AND;
                            //Mensaje de error!
                        }
                        else if(circuit.compare("NOT") == 0 || circuit.compare("not") == 0)
                        {
                            gate = cg::Promoter::Gate::NAND;
                            //Mensaje de error!
                        }
                    }
                    op_data.promoter.gate = gate;
                    // promoter gate

                }

                if( promoter->getField( "transcription_factors" ) && promoter->getField( "transcription_factors" )->get_type() == Value::LIST)
                {
                    transcription_factors = promoter->getField( "transcription_factors" )->list_value();
                    if(transcription_factors != NULL && !transcription_factors->empty())
                    {
                        j = transcription_factors->begin();
                        do
                        {
                            temp_tf = (*j)->string_value();
                            if(temp_tf.at(0) == '-')
                            {
                                op_data.promoter.regulation.push_back(false);
                                temp_tf.erase(temp_tf.begin());
                            }
                            else
                            {
                                op_data.promoter.regulation.push_back(true);
                            }
                            op_data.promoter.names.push_back(temp_tf);
                            std::vector<float> times = {std::numeric_limits<float>::max(), (float)0.0f};
                            (world->get_protein_map())->insert(std::make_pair(temp_tf, times));
                            j++;
                        }while(j!= transcription_factors->end());
                    }
                    //tfs
                }

                /*else
                {
                    std::vector<std::string> tf_names;
                    temp_tf = "signaltempxyz";
                    tf_names.push_back(temp_tf);
                    //tfs signal
                }*/

                if( promoter->getField( "noise" ) && promoter->getField("noise")->get_type() == Value::RECORD)
                {
                    noise = promoter->getField( "noise" );
                    if(noise != NULL)
                    {
                        if( noise->getField( "toOn" ) && noise->getField( "toOn" )->get_type() == Value::REAL )
                        {
                            onNoise = (float)(noise->getField( "toOn" )->real_value());
                            if(onNoise > 1.0)
                            {
                                onNoise = 1.0;
                            }
                            else if(onNoise < 0.0)
                            {
                                onNoise = 0.0;
                            }
                        }
                        if( noise->getField( "toOff" ) && noise->getField( "toOff" )->get_type() == Value::REAL )
                        {
                            offNoise = (float)(noise->getField( "toOff" )->real_value());
                            if(offNoise > 1.0)
                            {
                                offNoise = 1.0;
                            }
                            else if(offNoise < 0.0)
                            {
                                offNoise = 0.0;
                            }
                        }
                        if( noise->getField( "noise_time" ) && noise->getField( "noise_time" )->get_type() == Value::REAL )
                        {
                            noiseTime = (float)(noise->getField( "noise_time" )->real_value());
                            if(noiseTime < 0.0)
                            {
                                noiseTime = 0.0;
                            }
                        }

                    }

                    //noise

                }
                op_data.promoter.noise = {onNoise, offNoise, noiseTime}; //esta linea ha cambiado de lugar
            }
        }

        //PENDIENTE LOS RIBOSWITCH
        /*if ( data->getField ( "riboswitches" ) && data->getField( "riboswitches" )->get_type() == Value::LIST)
        {
            riboswitches = data->getField ( "riboswitches" )->list_value();
            pMap = world->get_protein_and_rna_map();
            riboMap = world->get_riboswitches_map();
            if(riboswitches != NULL && !pMap.empty())
            {
                for(j=riboswitches->begin(); j!=riboswitches->end(); j++)
                {
                    temp_riboswitch = (*j)->list_value();
                    if(temp_riboswitch != NULL)
                    {
                        l=temp_riboswitch->begin();
                        //which_riboswitch = atoi(((*l)->string_value()).c_str());
                        which_riboswitch = riboMap[(*l)->string_value()];
                        l++;
                        riboswitch_location_gene = (*l)->string_value();
                        op->add_RBS(pMap[riboswitch_location_gene],which_riboswitch);
                    }
                }
            }
        }*/

        if ( data->getField ( "prot_act_times" ) && data->getField( "prot_act_times" )->get_type() == Value::RECORD)
        {
            proteinAtimes = data->getField( "prot_act_times" );
            if( proteinAtimes != NULL )
            {
                if( proteinAtimes->getField( "times" ) && proteinAtimes->getField( "times" )->get_type() == Value::LIST )
                {
                    protein_times = proteinAtimes->getField( "times" )->list_value();
                    if(protein_times != NULL)
                    {
                        if( proteinAtimes->getField( "variabilities" ) && proteinAtimes->getField( "variabilities" )->get_type() == Value::LIST )
                        {
                            protein_sigma = proteinAtimes->getField( "variabilities" )->list_value();
                        }
                    }
                }
            }

            if(proteins != NULL)
            {
                if(protein_times != NULL)
                {
                    j = protein_times->begin();
                }
                if(protein_sigma != NULL)
                {
                    l = protein_sigma->begin();
                }
                for(k=proteins->begin(); k!=proteins->end(); k++)
                {
                    output_prot = (*k)->string_value();

                    if(protein_times != NULL)
                    {
                        act_time = (*j)->real_value();
                        j++;
                    }
                    else
                    {
                        act_time = 0;
                    }
                    if(protein_sigma != NULL)
                    {
                        act_var_time = (*l)->real_value();
                        l++;
                    }
                    else
                    {
                        act_var_time = 0;
                    }

                    op_data.output_protein_names.push_back(output_prot);
                    op_data.act_times.push_back(act_time);
                    op_data.act_vars.push_back(act_var_time);
                }
            }
        }

        if ( data->getField ( "prot_deg_times" ) && data->getField( "prot_deg_times" )->get_type() == Value::RECORD)
        {
            proteinDtimes = data->getField( "prot_deg_times" );
            if( proteinDtimes != NULL )
            {
                if( proteinDtimes->getField( "times" ) && proteinDtimes->getField( "times" )->get_type() == Value::LIST )
                {
                    protein_times = proteinDtimes->getField( "times" )->list_value();
                    if(protein_times != NULL)
                    {
                        if( proteinDtimes->getField( "variabilities" ) && proteinDtimes->getField( "variabilities" )->get_type() == Value::LIST )
                        {
                            protein_sigma = proteinDtimes->getField( "variabilities" )->list_value();
                        }
                    }
                }
            }

            if(proteins != NULL)
            {
                if(protein_times != NULL)
                {
                    j = protein_times->begin();
                }
                if(protein_sigma != NULL)
                {
                    l = protein_sigma->begin();
                }


                for(k=proteins->begin(); k!=proteins->end(); k++)
                {
                    output_prot = (*k)->string_value();

                    if(protein_times != NULL)
                    {
                        deg_time = (*j)->real_value();
                        j++;
                    }
                    else
                    {
                        deg_time = std::numeric_limits<float>::max();
                    }
                    if(protein_sigma != NULL)
                    {
                        deg_var_time = (*l)->real_value();
                        l++;
                    }
                    else
                    {
                        deg_var_time = 0;
                    }


                    if((world->get_protein_map())->find(output_prot) != (world->get_protein_map())->end())
                    {
                        stored_deg_time = (world->get_protein_map())->at(output_prot).at(0);
                        if(deg_time < stored_deg_time)
                        {
                            (*world->get_protein_map())[output_prot] = {deg_time, deg_var_time};
                        }
                    }
                    else
                    {
                        (*world->get_protein_map())[output_prot] = {deg_time, deg_var_time};
                    }

                }
            }
        }

        /*if ( data->getField ( "rna_act_times" ) && data->getField( "rna_act_times" )->get_type() == Value::RECORD)
        {
            rnaAtimes = data->getField( "rna_act_times" );
            if( rnaAtimes != NULL )
            {
                if( rnaAtimes->getField( "times" ) && rnaAtimes->getField( "times" )->get_type() == Value::LIST )
                {
                    rna_times = rnaAtimes->getField( "times" )->list_value();
                    if(rna_times != NULL)
                    {
                        if( rnaAtimes->getField( "variabilities" ) && rnaAtimes->getField( "variabilities" )->get_type() == Value::LIST )
                        {
                            rna_sigma = rnaAtimes->getField( "variabilities" )->list_value();
                        }
                    }
                }
            }

            if(rnas != NULL)
            {
                if(rna_times != NULL)
                {
                    j = rna_times->begin();
                }
                if(rna_sigma != NULL)
                {
                    l = rna_sigma->begin();
                }
                for(k=rnas->begin(); k!=rnas->end(); k++)
                {
                    if(world->get_globalPlasmidList()->getPlasmidById("listPP0")->getOperonById("listPP0O0")->getGen((*k)->string_value()) == NULL)
                    {
                        GenCell* rna = new GenCell((*k)->string_value(), 1, 0.0, 0.0);
                        world->get_globalPlasmidList()->getPlasmidById("listPP0")->getOperonById("listPP0O0")->insertGens(rna);
                    }

                    if(rna_times != NULL)
                    {
                        if(!op->isGenExist((*k)->string_value()))
                        {
                            op->insertGens(world->get_globalPlasmidList()->getPlasmidById("listPP0")->getOperonById("listPP0O0")->getGen((*k)->string_value()));
                            op->getGen((*k)->string_value())->setParent(op);
                        }
                        op->getGen((*k)->string_value())->setTimeAct((double)(*j)->real_value());
                        j++;
                    }
                    if(rna_sigma != NULL)
                    {
                        op->getGen((*k)->string_value())->setSigmaAct((double)(*l)->real_value());
                        l++;
                    }
                }
            }
        }

        if ( data->getField ( "rna_deg_times" ) && data->getField( "rna_deg_times" )->get_type() == Value::RECORD)
        {
            rnaDtimes = data->getField( "rna_deg_times" );
            if( rnaDtimes != NULL )
            {
                if( rnaDtimes->getField( "times" ) && rnaDtimes->getField( "times" )->get_type() == Value::LIST )
                {
                    rna_times = rnaDtimes->getField( "times" )->list_value();
                    if(rna_times != NULL)
                    {
                        if( rnaDtimes->getField( "variabilities" ) && rnaDtimes->getField( "variabilities" )->get_type() == Value::LIST )
                        {
                            rna_sigma = rnaDtimes->getField( "variabilities" )->list_value();
                        }
                    }
                }
            }

            if(rnas != NULL)
            {
                if(rna_times != NULL)
                {
                    j = rna_times->begin();
                }
                if(rna_sigma != NULL)
                {
                    l = rna_sigma->begin();
                }
                for(k=rnas->begin(); k!=rnas->end(); k++)
                {
                    if(world->get_globalPlasmidList()->getPlasmidById("listPP0")->getOperonById("listPP0O0")->getGen((*k)->string_value()) == NULL)
                    {
                        GenCell* rna = new GenCell((*k)->string_value(), 1, 0.0, 0.0);
                        world->get_globalPlasmidList()->getPlasmidById("listPP0")->getOperonById("listPP0O0")->insertGens(rna);
                    }

                    if(rna_times != NULL)
                    {
                        if(!op->isGenExist((*k)->string_value()))
                        {
                            op->insertGens(world->get_globalPlasmidList()->getPlasmidById("listPP0")->getOperonById("listPP0O0")->getGen((*k)->string_value()));
                            op->getGen((*k)->string_value())->setParent(op);
                        }
                        op->getGen((*k)->string_value())->setTimeDeg((double)(*j)->real_value());
                        j++;
                    }
                    if(rna_sigma != NULL)
                    {
                        op->getGen((*k)->string_value())->setSigmaDeg((double)(*l)->real_value());
                        l++;
                    }
                }
            }
        }*/

        (world->get_operon_map())->insert(std::make_pair(operon_name, op_data));
        world->setNoProt(false);
    }
    return new Value ( Value::UNIT );
}

Value * assign_operons_to_plasmids (std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    ASSERT(args->size() == 1);
    //for(std::map<std::string, std::vector<float>>::iterator it = world->get_protein_map()->begin(); it != world->get_protein_map()->end(); ++it)
    for(auto it : *world->get_protein_map())
    {
        world->getPlasmidPool()->createProtein(it.second[0], it.second[1], it.first);
    }

    //for(std::map<std::string, std::vector<float>>::iterator it = world->get_operon_map()->begin(); it != world->get_operon_map()->end(); ++it)
    for(auto it : *world->get_operon_map())
    {
        std::vector<std::string> names = it.second.promoter.names;
        std::vector<bool> regulations = it.second.promoter.regulation;
        uint64_t mask = 0, mask_unique = 0;
        uint64_t values = 0, values_unique = 0;
        for(unsigned int i = 0; i < names.size(); ++i)
        {
            uint64_t id = world->getPlasmidPool()->getProteinByName(names[i])->getID();
            uint64_t value = id * regulations[i];
            mask |= id;
            values |= value;
            if(i == 0)
            {
               mask_unique |= id;
               values_unique |= value;
            }
        }

        float rateOn = 0.0f, rateOff = 0.0f;
        if(it.second.promoter.noise[2] != 0.0f)
        {
            if(it.second.promoter.noise[0] != 0.0f)
                rateOn = -log(1-(it.second.promoter.noise[0]))/it.second.promoter.noise[2];
            if(it.second.promoter.noise[1] != 0.0f)
                rateOff =-log(1-(it.second.promoter.noise[1]))/it.second.promoter.noise[2];

        }

        cg::Promoter P0(mask, values, it.second.promoter.gate, cg::Promoter::BreakingState::NOT_BROKEN, 1.0f / rateOn, 1.0f / rateOff);

        std::vector<std::string>& gene_names = it.second.output_protein_names;
        //A partir del numero de genes, se crean y leen los tiempos y vars y se van colocando en el vector genes que
        // luego va al operon - creado en la plasmidpool
        std::vector<cg::Gene> genes;

        for(unsigned int i = 0; i < gene_names.size(); ++i)
        {
            cg::Gene g(world->getPlasmidPool()->getProteinByName(it.second.output_protein_names[i]), it.second.act_times[i], it.second.act_vars[i]);
            genes.push_back(g);
        }

        world->getPlasmidPool()->createOperon(P0,genes,it.first);
    }

    //Recuerda cargar datos de operon, proteina y genes!

    Value * data = *i;
    std::list<Value*> * operons = NULL;
    std::list<Value*>::iterator j;

    SymbolTable* fields;
    std::list<std::string> field_names;

    std::vector<const cg::Operon*> insert_op;

    if ( data->get_type() == Value::RECORD )
    {
        fields = data->get_fields();
        field_names = fields->field_names_as_string_list();

        for(std::list<std::string>::iterator iter = field_names.begin(); iter != field_names.end(); ++iter)
        {
            std::string k =  (*iter);
            if ( data->getField ( k.c_str() ) && data->getField( k.c_str() )->get_type() == Value::LIST)
            {
                operons = data->getField ( k.c_str() )->list_value();
                if(operons != NULL)
                {
                    for(j=operons->begin(); j!=operons->end(); j++)
                    {
                        if((world->getPlasmidPool()->getOperonByName((*j)->string_value())) != NULL)
                        {
                            insert_op.push_back((world->getPlasmidPool()->getOperonByName((*j)->string_value())));
                        }
                        else
                        {
                            //Error! Imprime (no hay operon definido).
                        }
                    }
                }
            }
            if(insert_op.size() != 0)
            {
                world->getPlasmidPool()->createPlasmid(insert_op,k);
                insert_op.clear();
            }
            else
            {
                //Error! Plasmido no se crea.
            }
        }
    }
    return new Value ( Value::UNIT );
}

Value * set_action (std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    ASSERT(args->size() == 3 || args->size() == 2);

    Value * prot_list = *i;i++;
    Value * func_name = *i;i++;
    Value * param_list;
    if(args->size() == 3)
    {
        param_list = *i;
    }

    bool regulation = false;
    uint64_t mask = 0;
    uint64_t values = 0;

    std::list<std::string> params;
    std::string protein_name;
    std::string zero("0");
    std::string absorbQS("s_absorb_QS");
    std::string getQS("s_get_QS");

    protein_name.clear();

    for ( i=prot_list->list_value()->begin(); i != prot_list->list_value()->end(); i++ )
    {
        protein_name = (*i)->string_value();
        if(protein_name.at(0) == '-')
        {
            regulation = false;
            protein_name.erase(protein_name.begin());
        }
        else
        {
            regulation = true;
        }

        uint64_t id = world->getPlasmidPool()->getProteinByName(protein_name)->getID();
        uint64_t value = id * regulation;
        mask |= id;
        values |= value;
    }

    if(args->size() == 3)
    {
        for ( i=param_list->list_value()->begin(); i != param_list->list_value()->end(); i++ ) {
            params.push_back((*i)->string_value());
        }
    }
    else
    {
        params.push_back(zero);
    }

    world->add_action_data(func_name->string_value(), mask, values, params);
    world->setNoAction(false);

    if((func_name->string_value().compare(absorbQS) == 0) ||
       (func_name->string_value().compare(getQS) == 0))
    {
        world->add_signal_prot(params.back());
    }

    return new Value ( Value::UNIT );
}

Value * reset_actions (std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    world->remove_all_actions();
    /*world->clear_operon_map();
    world->clear_protein_and_rna_map();
    world->clear_plasmids_map();
    world->clear_riboswitches_map();
    world->clear_molecules_map();
    world->clear_analog_molecules_map();*/
    //world->set_num_actions(0);
    return new Value ( Value::UNIT );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// SIGNALLING
// INTERNAL GRO FUNCTIONS
//

Value * new_signal ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();

  std::list<Value *>::iterator i = args->begin();
  Value * kdi = *i; i++;
  Value * kde = *i;

  int w, h, numx, numy;

  w = world->get_param ( "signal_grid_width" );
  h = world->get_param ( "signal_grid_height" );
  numx = w / world->get_param ( "signal_element_size" );
  numy = h / world->get_param ( "signal_element_size" );

  /*Signal * sig = new Signal (
     cpv ( -w/2, -h/2 ), cpv ( w/2, h/2 ), numx, numy,
     kdi->num_value(), kde->num_value() );*/

  Signal * sig = new Signal (
              ceGetVector2 ( -w/2, -h/2 ), ceGetVector2 ( w/2, h/2 ), numx, numy,
              kdi->num_value(), kde->num_value() );

  world->add_signal ( sig );

  return new Value ( world->num_signals() - 1 );

}

Value * new_cross_feeding_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    std::list<Value *>::iterator i = args->begin();
    Value * kdi = *i; i++;
    Value * kde = *i; i++;
    Value * conc_max = *i;

    int w, h, numx, numy, id;

    w = world->get_param ( "signal_grid_width" );
    h = world->get_param ( "signal_grid_height" );
    numx = w / world->get_param ( "signal_element_size" );
    numy = h / world->get_param ( "signal_element_size" );

    Signal * sig = new Signal (
                ceGetVector2 ( -w/2, -h/2 ), ceGetVector2 ( w/2, h/2 ), numx, numy,
                kdi->num_value(), kde->num_value() );

    world->add_signal ( sig );

    id = world->num_signals() - 1;
    world->add_cross_feeding_max_emit(id,conc_max->num_value());

    return new Value ( world->num_signals() - 1 );
}

Value * add_reaction ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    // args are: reactants, products, rate
    std::list<Value *>::iterator i = args->begin();
    Value * R = *i; i++;
    Value * P = *i; i++;
    Value * k = *i;

    Reaction r (k->num_value());

    for ( i=R->list_value()->begin(); i != R->list_value()->end(); i++ ) {
        if ( (*i)->int_value() < 0 || (*i)->int_value() >= world->num_signals() )
            throw std::string ( "Reaction refers to a non-existant reactant." );
        r.add_reactant( (*i)->int_value() );
    }

    for ( i=P->list_value()->begin(); i != P->list_value()->end(); i++ ) {
        if ( (*i)->int_value() < 0 || (*i)->int_value() >= world->num_signals() )
            throw std::string ( "Reaction refers to a non-existant product." );
        r.add_product( (*i)->int_value() );
    }

    world->add_reaction(r);

    return new Value ( Value::UNIT );

}

Value * set_signal ( std::list<Value *> * args, Scope * s ) {

  // n, x, y, v


  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  ASSERT ( args->size() == 4 );

  Value * n = *i; i++;
  Value * x = *i; i++;
  Value * y = *i; i++;
  Value * val = *i; 

  world->set_signal ( n->int_value(), x->num_value(), y->num_value(), val->num_value() );

  return new Value ( Value::UNIT );

}

Value * set_signal_rect ( std::list<Value *> * args, Scope * s ) {

  // n, x1, y1, x2, y2, v

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  ASSERT ( args->size() == 6 );

  Value * n = *i; i++;

  Value * x1 = *i; i++;
  Value * y1 = *i; i++;
  Value * x2 = *i; i++;
  Value * y2 = *i; i++;

  Value * val = *i;

  world->set_signal_rect ( n->int_value(), x1->num_value(), y1->num_value(), x2->num_value(), y2->num_value(), val->num_value() );

  return new Value ( Value::UNIT );

}

Value * get_signal ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * n = *i;

  if ( current_cell != NULL ) {
    ASSERT ( n->int_value() < world->num_signals() );
    return new Value ( (double) world->get_signal_value ( current_cell, n->int_value() ) );
  } else {
    printf ( "Warning: Tried to get signal value from outside a cell program. No action taken\n" );
    return new Value ( 0.0 );
  }

}

Value * emit_signal ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * n = *i; i++;
  Value * ds = *i;

  if ( current_cell != NULL )
    world->emit_signal ( current_cell, n->int_value(), ds->num_value() );
  else
    printf ( "Warning: Tried to emit signal from outside a cell program. No action taken\n" );

  return new Value ( Value::UNIT );

}

Value * absorb_signal ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * n = *i; i++;
  Value * ds = *i;

  if ( current_cell != NULL )
    world->absorb_signal ( current_cell, n->int_value(), ds->num_value() );
  else
    printf ( "Warning: Tried to absorb signal from outside a cell program. No action taken\n" );

  return new Value ( Value::UNIT );

}

Value * get_signal_matrix ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator a = args->begin();

  int signal_index = (*a)->int_value();
  std::vector< std::vector<float> > * sm = world->get_signal_matrix(signal_index);

  Value * m = new Value();

  int i, j;

  std::list<Value *> * K = new std::list<Value *>;

  for ( i=0; i < sm->size(); i++ ) {
      std::list<Value *> * L = new std::list<Value *>;
      for ( j=0; j != (*sm)[i].size(); j++ ) {
          L->push_back ( new Value ( (*sm)[i][j] ) );
      }
      Value * temp = new Value();
      temp->set(L);
      K->push_back(temp);
  }

  m->set(K);

  return m;

}

/////////////////////////////////////////////////////////////////////////////////////////////
// CELLSIGNALS
// EXTERNAL GRO FUNCTIONS
//

Value * grid( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    std::list<Value *>::iterator i = args->begin();
    Value * grid_type = *i; i++;
    Value * diff_method = *i; i++;
    Value * nCells = *i; i++;
    Value * cellSize = *i; i++;
    Value * neighborhood = *i;

    if (grid_type->string_value() == "continuous" && (world->get_param("signals") == 1.0)) {
        world->handler = new Grid<CSCell>(nCells->int_value(), cellSize->int_value(), neighborhood->int_value());
        world->grid_type = "continuous";
        world->diff_method = diff_method->string_value();
        world->set_param("signals_grid_length", nCells->int_value());
        world->set_param("signals_grid_cell_size", cellSize->int_value());
        world->set_param("signals_grid_neighborhood", neighborhood->int_value());
    } else if (grid_type->string_value() == "digital" && (world->get_param("signals") == 1.0)) {
        world->dhandler = new Grid<DBCell>(nCells->int_value(), cellSize->int_value(), neighborhood->int_value());
        world->grid_type = "digital";
        world->diff_method = diff_method->string_value();
        world->set_param("signals_grid_length", nCells->int_value());
        world->set_param("signals_grid_cell_size", cellSize->int_value());
        world->set_param("signals_grid_neighborhood", neighborhood->int_value());
    }

    return new Value ( Value::UNIT );
}

Value * change_matrix ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    std::list<Value *>::iterator i = args->begin();
    Value * signal_id = *i; i++;
    Value * data = *i;
    std::list<Value*> * global_matrix = NULL;
    std::list<Value*> * temp_matrix = NULL;
    std::list<Value *>::iterator j,l;

    std::vector<std::vector<float> > mdiff(3, std::vector<float>(3));
    bool mdefined = false;

    if ( data->get_type() == Value::RECORD ) {
        if ( data->getField ( "matrix" ) && data->getField( "matrix" )->get_type() == Value::LIST) {
            global_matrix = data->getField( "matrix" )->list_value();
            if (global_matrix != NULL) {
                int row = 0;
                for (j=global_matrix->begin(); j!=global_matrix->end(); j++) {
                    int col = 0;
                    temp_matrix = (*j)->list_value();
                    if(temp_matrix != NULL) {
                        for (l=temp_matrix->begin(); l!=temp_matrix->end(); l++) {
                            mdiff[row][col] = (*l)->real_value();
                            col++;
                        }
                    }
                    row++;
                }
                // Si ha entrado aquí es porque se ha definido la matriz
                mdefined = true;
            }
        }
    }

    if (world->grid_type == "continuous") {
       world->handler->setSignalMatrix(signal_id->int_value(), mdiff);
    } else if (world->grid_type == "digital") {
       world->dhandler->setSignalMatrix(signal_id->int_value(), mdiff);
    }


}

Value * s_add_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    std::list<Value *>::iterator i = args->begin();
    Value * data = *i;
    std::list<Value *>::iterator j,l;

    Value * kdi = NULL;
    Value * kde = NULL;
    Value * ncycles = NULL;
    Value * emit_max = NULL;
    Value * absorb_max = NULL;
    bool mdefined = false;
    std::vector<std::vector<float> > mdiff(3, std::vector<float>(3));

    std::list<Value*> * global_matrix = NULL;
    std::list<Value*> * temp_matrix = NULL;

    if ( data->get_type() == Value::RECORD ) {
        if ( data->getField ( "kdiff" ) && data->getField( "kdiff" )->get_type() == Value::REAL) {
            kdi = data->getField ( "kdiff" );
        }
        if ( data->getField ( "kdeg" ) && data->getField( "kdeg" )->get_type() == Value::REAL) {
            kde = data->getField ( "kdeg" );
        }
        if ( data->getField ( "cycles" ) && data->getField( "cycles" )->get_type() == Value::INTEGER) {
            ncycles = data->getField ( "cycles" );
        }
        if ( data->getField ( "matrix" ) && data->getField( "matrix" )->get_type() == Value::LIST) {
            global_matrix = data->getField( "matrix" )->list_value();
            if (global_matrix != NULL) {
                int row = 0;
                for (j=global_matrix->begin(); j!=global_matrix->end(); j++) {
                    int col = 0;
                    temp_matrix = (*j)->list_value();
                    if(temp_matrix != NULL) {
                        for (l=temp_matrix->begin(); l!=temp_matrix->end(); l++) {
                            mdiff[row][col] = (*l)->real_value();
                            col++;
                        }
                    }
                    row++;
                }
                // Si ha entrado aquí es porque se ha definido la matriz
                mdefined = true;
            }
        }
        if ( data->getField( "emit_max" ) && data->getField( "emit_max" )->get_type() == Value::REAL) {
            emit_max = data->getField( "emit_max" );
        }
        if ( data->getField( "absorb_max" ) && data->getField( "absorb_max" )->get_type() == Value::REAL) {
            absorb_max = data->getField( "absorb_max" );
        }
        if ( data->getField( "digital_prob" ) && data->getField( "digital_prob" )->get_type() == Value::REAL) {
            world->digital_prob = data->getField( "digital_prob" )->real_value();
        }
        if ( data->getField( "digital_prop" ) && data->getField( "digital_prop" )->get_type() == Value::INTEGER) {
            world->digital_prop = data->getField( "digital_prop" )->int_value();
        }
    }

    if (kdi != NULL && kde != NULL && ncycles == NULL && mdefined == false) {
        if (world->grid_type == "continuous") {
           world->handler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value());
        } else if (world->grid_type == "digital") {
           world->dhandler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value());
        }

    } else if (kdi != NULL && kde != NULL && ncycles != NULL && mdefined == false) {
        if (world->grid_type == "continuous") {
            world->handler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value(), ncycles->int_value());
        } else if (world->grid_type == "digital") {
           world->dhandler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value(), ncycles->int_value());
        }

    } else if (kdi != NULL && kde != NULL && mdefined == true && ncycles == NULL) {
        if (world->grid_type == "continuous") {
            world->handler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value(), mdiff);
        } else if (world->grid_type == "digital") {
           world->dhandler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value(), mdiff);
        }

    } else if (kdi != NULL && kde != NULL && mdefined == true && ncycles != NULL) {
        if (world->grid_type == "continuous") {
           world->handler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value(), mdiff, ncycles->int_value());
        } else if (world->grid_type == "digital") {
           world->dhandler->add_signal(world->s_signal_id, kdi->real_value(), kde->real_value(), mdiff, ncycles->int_value());
        }

    } else {
        printf("Wrong parameters specification\n");
    }

    world->signal_concs[world->s_signal_id] = 0;

    if (emit_max != NULL) {
        world->add_cross_feeding_max_emit(world->s_signal_id, emit_max->num_value());
    }

    if (absorb_max != NULL) {
        world->add_cross_feeding_max_absorb(world->s_signal_id, absorb_max->num_value());
    }

    world->s_signal_id++;

    return new Value ( world->s_signal_id - 1 );
}

Value * s_add_reaction ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    // args are: reactants, products, rate
    std::list<Value *>::iterator i = args->begin();
    Value * R = *i; i++;
    Value * P = *i; i++;
    Value * k = *i;

    std::vector<int> reacts, prods;

    for ( i=R->list_value()->begin(); i != R->list_value()->end(); i++ ) {
        reacts.push_back((*i)->int_value());
    }

    for ( i=P->list_value()->begin(); i != P->list_value()->end(); i++ ) {
        prods.push_back((*i)->int_value() );
    }

    world->handler->add_reaction(k->real_value(), reacts, prods);

    return new Value ( Value::UNIT );

}

Value * s_new_cross_feeding_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    std::list<Value *>::iterator i = args->begin();
    Value * kdi = *i; i++;
    Value * kde = *i; i++;
    Value * emit_max = *i; i++;
    Value * absorb_max = *i;


    world->handler->add_signal(world->s_signal_id, kdi->num_value(), kde->num_value());
    world->signal_concs[world->s_signal_id] = 0;
    //printf("%d: %f\n", world->s_signal_id, emit_max->num_value());
    //printf("%d: %f\n", world->s_signal_id, absorb_max->num_value());
    world->add_cross_feeding_max_emit(world->s_signal_id,emit_max->num_value());
    world->add_cross_feeding_max_absorb(world->s_signal_id,absorb_max->num_value());

    /*printf("Add signal - 2:\n");
    for(core = world->handler->sig.begin(); core != world->handler->sig.end(); core++)
    {
        tempGSignal = core->second;
        printf("Id: %d, diffusion: %f, degradation: %f\n",core->first, (core->second).get_kdiff(), (core->second).get_kdeg());

    }
    for(it = world->cross_feeding_max_emit.begin(); it != world->cross_feeding_max_emit.end(); it++)
    {
        printf("EMIT - Key = %d , Value = %f\n", it->first, it->second);
    }
    for(it = world->cross_feeding_max_absorb.begin(); it != world->cross_feeding_max_absorb.end(); it++)
    {
        printf("ABSORB - Key = %d , Value = %f\n", it->first, it->second);
    }*/

    //printf("Add signal - 2: id: %d, diffusion: %f, degradation: %f, emit max: %f, absorb max: %f\n", world->s_signal_id, kdi->num_value(), kde->num_value(), _max->num_value(), absorb_max->num_value());

    /*if(world->handler->sig.empty())
    {
        printf("VACIOOOOOO!!!\n");
    }*/

    world->s_signal_id++;

    return new Value ( world->s_signal_id - 1);
}

Value * s_absorb_signal_area ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++; //int
    Value * conc = *i; //double

    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = current_cell->get_vec_x();
    double y_length = current_cell->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    //printf("Coords_absorb: x = %f, y = %f, len_x = %f, len_y = %f\n", x, y, x_length, y_length);
    if (world->grid_type == "continuous") {
       world->handler->absorb_signal(signal_id->int_value(), conc->real_value(), coords, "random");
    } else if (world->grid_type == "digital") {
        world->dhandler->absorb_signal(signal_id->int_value(), conc->real_value(), coords, "random");
    }


    return new Value ( Value::UNIT );
}

Value * s_absorb_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++; //int
    Value * conc = *i; i++; //double
    Value * absorb_type = *i; //string

    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->absorb_signal(signal_id->int_value(), conc->real_value(), coords, absorb_type->string_value());
    } else if (world->grid_type == "digital") {
        world->dhandler->absorb_signal(signal_id->int_value(), conc->real_value(), coords, absorb_type->string_value());
    }

    //printf("Coords_absorb: x = %f, y = %f, len_x = %f, len_y = %f\n", x, y, x_length, y_length);


    return new Value ( Value::UNIT );
}

Value * s_emit_signal_area ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++; //int
    Value * conc = *i; //double

    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = current_cell->get_vec_x();
    double y_length = current_cell->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->emit_signal(signal_id->int_value(), conc->real_value(), coords, "average");
    } else if (world->grid_type == "digital") {
        world->dhandler->emit_signal(signal_id->int_value(), conc->real_value(), coords, "average");
    }

    //printf("Coords_absorb: x = %f, y = %f, len_x = %f, len_y = %f\n", x, y, x_length, y_length)

    return new Value ( Value::UNIT );
}

Value * s_emit_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++; //int
    Value * conc = *i; i++; //double
    Value * emit_type = *i; //string

    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
       world->handler->emit_signal(signal_id->int_value(), conc->real_value(), coords, emit_type->string_value());
    } else if (world->grid_type == "digital") {
        world->dhandler->emit_signal(signal_id->int_value(), conc->real_value(), coords, emit_type->string_value());
    }

    //printf("Coords_absorb: x = %f, y = %f, len_x = %f, len_y = %f\n", x, y, x_length, y_length);


    return new Value ( Value::UNIT );
}

Value * s_emit_signal_point ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++; //int
    Value * conc = *i; i++; //double
    Value * x = *i; i++;
    Value * y = *i;

    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x->real_value();
    coords[1] = y->real_value();
    coords[2] = x_length;
    coords[3] = y_length;

    //printf("Coords_absorb: x = %f, y = %f, len_x = %f, len_y = %f\n", x, y, x_length, y_length);
    world->handler->emit_signal(signal_id->int_value(), conc->real_value(), coords, "exact");

    return new Value ( Value::UNIT );
}

Value * s_get_signal_area ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; //int

    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = current_cell->get_vec_x();
    double y_length = current_cell->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    double sig_val;
    if (world->grid_type == "continuous") {
        sig_val =  world->handler->getSignalValue(signal_id->int_value(),coords, "area");
    } else if (world->grid_type == "digital") {
        sig_val =  world->dhandler->getSignalValue(signal_id->int_value(),coords, "area");
    }

    if(sig_val != -1)
    {
        world->signal_concs[signal_id->int_value()] = sig_val;
    }

    return new Value ( sig_val );
}

Value * s_get_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++; //int

    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    double sig_val;
    if (world->grid_type == "continuous") {
        sig_val =  world->handler->getSignalValue(signal_id->int_value(),coords, "exact");
    } else if (world->grid_type == "digital") {
        sig_val =  world->dhandler->getSignalValue(signal_id->int_value(),coords, "exact");
    }


    if(sig_val != -1)
    {
        world->signal_concs[signal_id->int_value()] = sig_val;
    }

    return new Value ( sig_val );
}

Value * s_set_signal_rect ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++;
    Value * conc = *i; i++;
    Value * x = *i; i++;
    Value * y = *i; i++;
    Value * x_length = *i; i++;
    Value * y_length = *i;

    double coords[4];
    coords[0] = x->real_value();
    coords[1] = y->real_value();
    coords[2] = x_length->real_value();
    coords[3] = y_length->real_value();

    if (world->grid_type == "continuous") {
        world->handler->setSignalValue(signal_id->int_value(), conc->real_value(), coords, "area");
    } else if (world->grid_type == "digital") {
        world->dhandler->setSignalValue(signal_id->int_value(), conc->real_value(), coords, "area");
    }


    return new Value ( Value::UNIT );
}

Value * s_set_signal ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * signal_id = *i; i++;
    Value * conc = *i; i++;
    Value * x = *i; i++;
    Value * y = *i;
    double x_length = 0.0;
    double y_length = 0.0;

    double coords[4];
    coords[0] = x->real_value();
    coords[1] = y->real_value();
    coords[2] = x_length;
    coords[3] = y_length;

    if (world->grid_type == "continuous") {
        world->handler->setSignalValue(signal_id->int_value(), conc->real_value(), coords, "exact");
    } else if (world->grid_type == "digital") {
        world->dhandler->setSignalValue(signal_id->int_value(), conc->real_value(), coords, "exact");
    }

    return new Value ( Value::UNIT );
}

Value * s_emit_cross_feeding_signal( std::list<Value *> * args, Scope * s )
{
    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i =  args->begin();
    int signal_id = (*i)->int_value(); i++;
    Value * conc = *i; i++;
    std::string emision_type = (*i)->string_value();

    float signal_conc = conc->real_value();
    float dl_max = 0, dl = 0;
    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = current_cell->get_vec_x();
    double y_length = current_cell->get_vec_y();

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;

    dl_max = current_cell->get_param ( "ecoli_growth_rate" ) * current_cell->get_volume() * world->get_sim_dt();

    //dl = current_cell->get_d_vol();
    dl = current_cell->get_d_length();

    current_cell->set_output_cf_coef( dl/dl_max );

    if(current_cell->get_output_cf_coef() > 1.0)
    {
        current_cell->set_output_cf_coef( 1.0 );
    }

    if (world->grid_type == "continuous") {
        world->handler->emit_signal(signal_id,(current_cell->get_output_cf_coef()*signal_conc),coords, emision_type.c_str());
    } else if (world->grid_type == "digital") {
        world->dhandler->emit_signal(signal_id,(current_cell->get_output_cf_coef()*signal_conc),coords, emision_type.c_str());
    }

    return new Value ( Value::UNIT );
}

Value *  s_get_cross_feeding_signal( std::list<Value *> * args, Scope * s )
{
    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    int signal_id = (*i)->int_value(); i++;
    float conc = (*i)->real_value(); i++;
    int benefit = (*i)->int_value();
    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = 0;
    double y_length = 0;

    double coords[4];
    coords[0] = x;
    coords[1] = y;
    coords[2] = x_length;
    coords[3] = y_length;
    float signal_level = world->handler->getSignalValue(signal_id, coords, "exact");

    if(signal_level == -1)
    {
        signal_level = 0;
    }

    switch(benefit)
    {
        case 1:
            current_cell->set_input_cf_coef(signal_level/conc);
            break;
        case -1:
            current_cell->set_input_cf_coef(1 - (signal_level/conc));
            break;
        default:
            current_cell->set_input_cf_coef(1.0);
            break;
    }
    if(current_cell->get_input_cf_coef() > 1)
    {
        current_cell->set_input_cf_coef(1.0);
    }
    else if(current_cell->get_input_cf_coef() < 0)
    {
        current_cell->set_input_cf_coef(0.0);
    }

    //return new Value ( Value::UNIT );

    return new Value ( current_cell->get_input_cf_coef() );

}

Value * s_absorb_cross_feeding_signal( std::list<Value *> * args, Scope * s )
{
    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    int signal_id = (*i)->int_value(); i++;
    float conc = (*i)->real_value(); i++;
    int benefit = (*i)->int_value(); i++;
    std::string absorb_type = (*i)->string_value();
    double x = current_cell->get_x();
    double y = current_cell->get_y();
    double x_length = current_cell->get_vec_x();
    double y_length = current_cell->get_vec_y();

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

    float post_signal_level;
    float dif_level = 0;

    if(pre_signal_level != -1)
    {
        if (world->grid_type == "continuous") {
            world->handler->absorb_signal(signal_id, conc, coords, "exact");
            post_signal_level = world->handler->getSignalValue(signal_id, coords, "exact");
        } else if (world->grid_type == "digital") {
            world->dhandler->absorb_signal(signal_id, conc, coords, absorb_type.c_str());
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
            current_cell->set_input_cf_coef(dif_level/conc);
            break;
        case -1:
            current_cell->set_input_cf_coef(1 - (dif_level/conc));
            break;
        default:
            current_cell->set_input_cf_coef(1.0);
            break;
    }

    if(current_cell->get_input_cf_coef() > 1)
    {
        current_cell->set_input_cf_coef(1.0);
    }
    else if(current_cell->get_input_cf_coef() < 0)
    {
        current_cell->set_input_cf_coef(0.0);
    }

    return new Value ( dif_level );

    //return new Value ( Value::UNIT );
}


/////////////////////////////////////////////////////////////////////////////////////////////
// PHAGES
// EXTERNAL GRO FUNCTIONS (WITH CELLSIGNALS)
//

Value * local_MOI ( std::list<Value *> * args, Scope * s )
{
    std::list<Value *>::iterator i = args->begin();

    Value * id = *i; i++;
    Value * dist = *i;
    float n_bact = 0, sig_conc = 0, local_moi = 0;

    n_bact = current_cell->get_n_cells_d((float)dist->real_value());
    sig_conc = current_cell->area_concentration(id->int_value(),(float)dist->real_value());
    if(n_bact == 0 || sig_conc == -1)
    {
        local_moi = 0;
    }
    else
    {
        local_moi = sig_conc/n_bact;
    }
    //printf("Neighbor bacteria: %d, signal concentration: %f, LOCAL MOI: %f\n", (int)n_bact, sig_conc, local_moi);

    return new Value(local_moi);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// GROWTH
// EXTERNAL GRO FUNCTIONS (WITH CELLNUTRIENT)
//

Value * delta_length ( std::list<Value *> * args, Scope * s )
{
    return new Value ( current_cell->get_d_length() );
}

Value * my_available ( std::list<Value *> * args, Scope * s )
{
    return new Value ( current_cell->get_available());
}


/////////////////////////////////////////////////////////////////////////////////////////////
// CELL SPECIFIC (EXCEPT SIGNALLING)
// INTERNAL GRO FUNCTIONS
//

Value * geometry ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * g = new Value ( Value::RECORD );

  if ( current_cell != NULL ) {

      Value * g = new Value ( Value::RECORD );
      g->addField ( "x",     new Value ( current_cell->get_x()     ) );
      g->addField ( "y",     new Value ( current_cell->get_y()     ) );
      g->addField ( "theta", new Value ( current_cell->get_theta() ) );

      return g;

  } else {

    printf ( "Warning: Tried to get signal value from outside a cell program. No action taken\n" );
    g->addField ( "x",     new Value ( 0.0 ) );
    g->addField ( "y",     new Value ( 0.0 ) );
    g->addField ( "theta", new Value ( 0.0 ) );
    return g;

  }

}


Value * die ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  if ( current_cell != NULL ) 
    current_cell->mark_for_death();
  else
    printf ( "Warning: Called die() from outside a cell program. No action taken\n" );

  return new Value ( Value::UNIT );

}

Value * die_c ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    ASSERT(args->size() == 1);

    float radius = 0;
    radius = (*i)->real_value();


    if (current_cell != NULL)
    {
        float x = current_cell->get_x();
        float y = current_cell->get_y();
        if((x*x + y*y) >= radius*radius)
        {
            current_cell->mark_for_death();
        }
    }
    else
        printf ( "Warning: Called die_c() from outside a cell program. No action taken\n" );

    return new Value ( Value::UNIT );

}

Value * force_divide ( std::list<Value *> * args, Scope * s ) {

  if ( current_cell != NULL ) {

    current_cell->force_divide();

  } else fprintf ( stderr, "Warning: divide() called from outside a cell\n" );

  return new Value ( Value::UNIT );

}

/////////////////////////////////////////////////////////////////////////////////////////////
// SIMULATION CONTROL
// INTERNAL GRO FUNCTIONS
//

Value * reset ( std::list<Value *> * args, Scope * s ) {

  current_gro_program->get_world()->restart();
  return new Value ( Value::UNIT );

}

Value * stop ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  world->set_stop_flag(true);
  return new Value ( Value::UNIT );

}

Value * start ( std::list<Value *> * args, Scope * s ) {

  //run_simulation ( true );
  return new Value ( Value::UNIT );

}



Value * set_param ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * name = *i; i++;
  Value * val = *i;

  if ( current_cell == NULL ) { // This is a global parameter //////////////////////////////////

      if ( ( name->string_value() != "signal_grid_width"
          && name->string_value() != "signal_grid_height"
          && name->string_value() != "signal_element_size" ) || world->num_signals() == 0 ) {

          world->set_param ( name->string_value(), val->num_value() );

      }

  } else { /////////////////////// This is a cell-specific parameter ///////////////////////////

    current_cell->set_param ( name->string_value(), val->num_value() );
    current_cell->compute_parameter_derivatives();

  }

  if ( name->string_value() == "seed" )
  {
        world->getPlasmidPool()->getRandomEngine().seed((unsigned int)val->num_value());
  }

  return new Value ( Value::UNIT );

}

Value * world_stats ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * name = *i;

  if ( name->string_value() == "pop_size" ) {

    return new Value ( world->get_pop_size() );
    
  } else printf ( "unknown statistic %s in call to 'stat'\n", name->string_value().c_str() );

  return new Value ( 0 );

}

Value * gro_time ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();

    return new Value ( world->get_time() );

}

Value * replate ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    ASSERT(args->size() == 1);

    float perc = (*i)->real_value();

    world->replating(perc);

    return new Value ( Value::UNIT );
}

Value * select_random_cell ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    world->select_random_cell();
    return new Value ( Value::UNIT );

}

/////////////////////////////////////////////////////////////////////////////////////////////
// MESSAGES AND IMAGES
// INTERNAL GRO FUNCTIONS
//

Value * message ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * n = *i;
  i++;
  Value * mes = *i;

  world->message ( n->int_value(), mes->string_value() );

  return new Value ( Value::UNIT );

}

Value * clear_messages ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  Value * n = *i;

  world->clear_messages ( n->int_value() );

  return new Value ( Value::UNIT );

}

Value * snapshot ( std::list<Value *> * args, Scope * s ) {

  if ( true /* used to check for whether the gui was instantiated */ ) {
    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    Value * name = *i;
    world->snapshot ( name->string_value().c_str() );
  }

  return new Value ( 0 );

}

Value * zoom ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  float z = (*i)->num_value();

  if ( z > 0 ) {

      world->set_zoom ( z );

  }

  return new Value ( Value::UNIT );

}

Value * set_theme ( std::list<Value *> * args, Scope * s ) {

#ifndef NOGUI

  ASSERT ( args->size() == 1 );

  std::list<Value *>::iterator i = args->begin();
  current_gro_program->get_world()->set_theme ( *i );

#endif

  return new Value ( Value::UNIT );

}

Value * gro_print ( std::list<Value *> * args, Scope * s ) {

  std::list<Value *>::iterator i;
  std::stringstream strm;

  for ( i=args->begin(); i!=args->end(); i++ ) {
      if ( (*i)->get_type() == Value::STRING )
          strm << (*i)->string_value();
      else
        strm << (*i)->tostring();
  }

#ifndef NOGUI
  current_gro_program->get_world()->emit_message(strm.str());
#else
  std::cout << strm.str();
#endif

  return new Value(Value::UNIT);

}

Value * gro_clear ( std::list<Value *> * args, Scope * s ) {

  std::string str = "";
  current_gro_program->get_world()->emit_message(str,true);
  return new Value(Value::UNIT);

}

Value * gro_fopen ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    Value * name = *i; i++;
    Value * mod = *i;

    FILE * f = fopen ( name->string_value().c_str(), mod->string_value().c_str() );

    if ( f ) {

        world->fileio_list.push_back(f);
        return new Value ( (int) world->fileio_list.size() - 1 );

    } else {

        return new Value(-1);

    }

}

Value * gro_fprint ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    int index = (*i)->int_value(); i++;

    if ( 0 <= index && index < world->fileio_list.size() ) {

        std::stringstream strm;

        for ( ; i!=args->end(); i++ ) {
            if ( (*i)->get_type() == Value::STRING )
                strm << (*i)->string_value();
            else
                strm << (*i)->tostring();
        }

        fprintf ( world->fileio_list[index], strm.str().c_str() );
        fflush ( world->fileio_list[index] );
    }

    return new Value ( Value::UNIT );

}

Value * gro_dump ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    int index = (*i)->int_value(); i++;

    if ( 0 <= index && index < world->fileio_list.size() ) {

        world->dump(world->fileio_list[index]);
        fflush ( world->fileio_list[index] );

    }

    return new Value ( Value::UNIT );

}

Value * create_dirs ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    ASSERT(args->size() == 2);

    Value * path = *i;
    i++;
    Value * n = *i;
    world->create_dirs(path->string_value().c_str(), n->int_value());

    return new Value ( Value::UNIT );
}

Value * gro_dump_single (std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    int index = (*i)->int_value(); i++;

    /*std::vector<std::string> names = it->second->promoter.names;
    std::vector<bool> regulations = it->second->promoter.regulation;
    uint64_t mask = 0, mask_unique = 0;
    uint64_t values = 0, values_unique = 0;
    for(unsigned int i = 0; i < names.size(); ++i)
    {
        uint64_t id = world->getPlasmidPool()->getProteinByName(names[i])->getID();
        uint64_t value = id * regulations[i];
        mask |= id;
        values |= value;
        if(i == 0)
        {
           mask_unique |= id;
           values_unique |= value;
        }
    }*/

    std::vector<std::string> prot_names;

    for(auto it : *(world->get_protein_map()))
    {
        prot_names.push_back(it.first);
    }

    //REVISAR ESTO. Mejor quizas hacer un internal proteins global y consultar por ese en la bacteria que estoy mirando.
    //Idea: Hacer un mapa de cada plasmidList de todas las proteinas (activas e inactivas) y luego las misma operacion global.

    if(!world->get_output_started())
    {
        fprintf(world->fileio_list[index], "time, id, x, y, theta, volume, gt_inst, gfp, rfp, yfp, cfp");
        for(auto it : prot_names)
        {
            fprintf(world->fileio_list[index], ", %s", it.c_str());
        }
        fprintf(world->fileio_list[index], "\n");
        world->set_output_started(true);
    }

    if ( 0 <= index && index < world->fileio_list.size() ) {
        current_cell->state_to_file(world->fileio_list[index], prot_names);
        fflush ( world->fileio_list[index] );
    }

    return new Value ( Value::UNIT );
}

Value * gro_dump_single_reduced (std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();
    int index = (*i)->int_value(); i++;

    std::vector<std::string> prot_names;

    for(auto it : *(world->get_protein_map()))
    {
        prot_names.push_back(it.first);
    }

    char time[5];
    char equis[2];

    strcpy(time, "time");
    strcpy(equis, "x");


    if(!world->get_output_started())
    {
        fprintf(world->fileio_list[index], "%8s,%5s", time, equis);
        for(auto it : prot_names)
        {
            fprintf(world->fileio_list[index], ", %6s", it.c_str());
        }
        fprintf(world->fileio_list[index], "\n");
        world->set_output_started(true);
    }

    if ( 0 <= index && index < world->fileio_list.size() ) {
        current_cell->state_to_file_reduced(world->fileio_list[index], prot_names);
        fflush ( world->fileio_list[index] );
    }

    return new Value ( Value::UNIT );
}

Value * gro_dump_multiple(std::list<Value *> * args, Scope * s){

    World * world = current_gro_program->get_world();
    int n_cond = 0, j = 0;
    int *counts;

    uint64_t mask = 0;
    uint64_t values = 0;
    bool regulation = false;
    //std::vector<std::pair<std::string, int>> condition_vector;

    std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator cds;
    std::list<Value *>::iterator l;
    std::string protein_name;
    int index = (*i)->int_value();
    i++;
    cds = i;

    for (i=cds ; i != args->end(); i++ )
    {
        n_cond++;
    }
    counts = new int[n_cond];

    for(j=0; j<n_cond; j++)
    {
        counts[j] = 0;
    }
    j = 0;

    for( i=cds ; i != args->end(); i++ )
    {
        for(l = (*i)->list_value()->begin(); l != (*i)->list_value()->end(); l++)
        {
            protein_name = (*l)->string_value();
            if(protein_name.at(0) == '-')
            {
                regulation = false;
                protein_name.erase(protein_name.begin());
            }
            else
            {
                regulation = true;
            }
            uint64_t id = world->getPlasmidPool()->getProteinByName(protein_name)->getID();
            mask |= id;
            values |= (id * regulation);
         }
        counts[j] += world->check_gen_cond_pop({mask, values});
        j++;
        mask = 0;
        values = 0;
    }

    fprintf(world->fileio_list[index],"%f, ", world->get_time());
    for (j=0; j<n_cond-1; j++)
    {
        fprintf(world->fileio_list[index],"%d, ", counts[j]);
    }
    fprintf(world->fileio_list[index],"%d\n", counts[j]);

    fflush (world->fileio_list[index]);

    delete counts;

    return new Value ( Value::UNIT );
}

Value * gro_dump_multiple_plasmids(std::list<Value *> * args, Scope * s){

    World * world = current_gro_program->get_world();
    int n_cond = 0, j = 0;
    int *counts;

    std::vector<std::pair<std::string, int>> condition_vector;

    std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator cds;
    std::list<Value *>::iterator l;
    std::string plasmid_name;
    int plasmid_value = 0;
    int index = (*i)->int_value();
    i++;
    cds = i;

    for (i=cds ; i != args->end(); i++ )
    {
        n_cond++;
    }
    counts = new int[n_cond];

    for(j=0; j<n_cond; j++)
    {
        counts[j] = 0;
    }
    j = 0;

    for( i=cds ; i != args->end(); i++ )
    {
        for(l = (*i)->list_value()->begin(); l != (*i)->list_value()->end(); l++)
        {
            plasmid_name = (*l)->string_value();
            if(plasmid_name.at(0) == '-')
            {
                plasmid_value = -1;
                plasmid_name.erase(plasmid_name.begin());
            }
            else
            {
                plasmid_value = 1;
            }
            condition_vector.push_back(std::make_pair(plasmid_name,plasmid_value));
        }
        counts[j] += world->check_plasmid_cond_pop(condition_vector);
        j++;
        condition_vector.clear();
    }

    fprintf(world->fileio_list[index],"%f, ", world->get_time());
    for (j=0; j<n_cond-1; j++)
    {
        fprintf(world->fileio_list[index],"%d, ", counts[j]);
    }
    fprintf(world->fileio_list[index],"%d\n", counts[j]);

    fflush (world->fileio_list[index]);

    delete counts;

    return new Value ( Value::UNIT );
}

Value * gro_dump_cond_conjugations(std::list<Value *> * args, Scope * s){

    World * world = current_gro_program->get_world();
    int n = 0, j = 0;
    int *counts;

    std::list<Value *>* plasmid_vector;

    std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator plasmid_name;
    int index = (*i)->int_value();
    i++;
    plasmid_vector = (*i)->list_value();

    n = plasmid_vector->size();
    counts = new int[n];

    for(j=0; j<n; j++)
    {
        counts[j] = 0;
    }
    j = 0;

    for(plasmid_name = plasmid_vector->begin(); plasmid_name != plasmid_vector->end(); plasmid_name++)
    {
        counts[j] = world->get_conj_count(world->getPlasmidPool()->getPlasmidByName((*plasmid_name)->string_value()));
    }

    fprintf(world->fileio_list[index],"%f, ", world->get_time());
    for (j=0; j<n-1; j++)
    {
        fprintf(world->fileio_list[index],"%d, ", counts[j]);
    }
    fprintf(world->fileio_list[index],"%d\n", counts[j]);

    fflush (world->fileio_list[index]);

    delete counts;

    return new Value ( Value::UNIT );


    ////////////////
    /*std::list<Value *>::iterator i = args->begin();
    std::list<Value *>::iterator cds;
    std::list<Value *>::iterator l;
    std::string plasmid_name;
    int plasmid_value = 0;
    int index = (*i)->int_value();
    i++;
    cds = i;
    const std::map<std::string, const Plasmid*> plasmids;
    plasmids = world->getPlasmidPool()->getPlasmids();

    for(auto it : plasmids)
    {

    }*/


}


/////////////////////////////////////////////////////////////////////////////////////////////
// MISC
// INTERNAL GRO FUNCTIONS
//

Value * gro_dump_left ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    float left;

    left = world->dump_left();
    return new Value ( left );

}

Value * gro_dump_right ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    float right;

    right = world->dump_right();
    return new Value ( right );

}

Value * gro_dump_top ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    float top;

    top = world->dump_top();
    return new Value ( top );

}

Value * gro_dump_bottom ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    float bottom;

    bottom = world->dump_bottom();
    return new Value ( bottom );

}

/////////////////////////////////////////////////////////////////////////////////////////////
// MICROFLUIDICS
// INTERNAL GRO FUNCTIONS
//

Value * barrier ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    float x1 = (*i)->num_value();
    i++;
    float y1 = (*i)->num_value();
    i++;
    float x2 = (*i)->num_value();
    i++;
    float y2 = (*i)->num_value();

    //PREGUNTARLE A LUIS
    /*cpShape *shape;
    cpBody *staticBody = &(world->get_space())->staticBody;

    shape = cpSpaceAddShape(world->get_space(), cpSegmentShapeNew(staticBody, cpv(x1,y1), cpv(x2,y2), 5.0f));
    shape->e = 1.0f; shape->u = 0.0f;

    world->add_barrier ( x1, y1, x2, y2 );*/

    return new Value ( Value::UNIT );

}

Value * chemostat ( std::list<Value *> * args, Scope * s ) {

    World * world = current_gro_program->get_world();
    std::list<Value *>::iterator i = args->begin();

    Value * val = *i;

    world->set_chemostat_mode(val->bool_value());

    return new Value ( Value::UNIT );

}

Value * World::map_to_cells ( Expr * e ) {

  std::list<Value *> * L = new std::list<Value *>;
  std::vector<Cell *>::iterator j;

  for ( j=population->begin(); j!=population->end(); j++ ) {
    L->push_back ( (*j)->eval ( e ) );
  }

  return new Value ( L );

}

Value * map_to_cells (  std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();

  std::list<Value *>::iterator i = args->begin();
  Expr * expression = (*i)->func_body();

  while ( expression->get_type() == Expr::CONSTANT ) {
    expression = expression->get_val()->func_body();
  }

  return world->map_to_cells ( expression );

}

/////////////////////////////////////////////////////////////////////////////////////////////
// CHEMOTAXIS
// INTERNAL GRO FUNCTIONS
//

Value * run ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  float dvel = (*i)->num_value();

  if ( current_cell != NULL ) {

    float a = current_cell->get_theta();
    ceBody * body = current_cell->get_body();

    /*cpBody * body = current_cell->get_body();
    cpVect v = cpBodyGetVel ( body );
    cpFloat adot = cpBodyGetAngVel ( body );

    cpBodySetTorque ( body, -adot ); // damp angular rotation

    cpBodyApplyForce ( // apply force
      current_cell->get_shape()->body, 
      cpv ( 
        ( dvel*cos(a) - v.x ) * world->get_sim_dt(),
        ( dvel*sin(a) - v.y ) * world->get_sim_dt()
      ), 
      cpv ( 0, 0 ) );*/

  } else

    printf ( "Warning: Tried to emit signal from outside a cell program. No action taken\n" );

  return new Value ( Value::UNIT );

}

Value * tumble ( std::list<Value *> * args, Scope * s ) {

  World * world = current_gro_program->get_world();
  std::list<Value *>::iterator i = args->begin();

  float vel = (*i)->num_value(); 

  if ( current_cell != NULL ) {

    float a = current_cell->get_theta();
    ceBody * body = current_cell->get_body();
    /*cpBody * body = current_cell->get_body();
    cpVect v = cpBodyGetVel ( body );
    cpFloat adot = cpBodyGetAngVel ( body );

    cpBodySetTorque ( body, vel - adot ); // apply torque

    cpBodyApplyForce ( // damp translation
      current_cell->get_shape()->body, 
      cpv ( 
        - v.x * world->get_sim_dt(),
        - v.y * world->get_sim_dt()
      ), 
      cpv ( 0, 0 ) );*/

  } else

    printf ( "Warning: Tried to emit signal from outside a cell program. No action taken\n" );

  return new Value ( Value::UNIT );

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gro Class Methods
// 
// 

gro_Program::gro_Program ( const char * path, int ac, char ** av ) : MicroProgram(), pathname(path), scope(NULL), argc(ac), argv(av) {

  current_gro_program = this;

}

void gro_Program::add_method ( Value::TYPE t, int num_args, const char * name, EXTERNAL_CCLI_FUNCTION f ) {

  ASSERT ( scope != NULL );

  int i;
  TypeExpr * R = new TypeExpr ( t );
  std::list<TypeExpr *> * Ta = new std::list<TypeExpr *>;
  for ( i=0; i<num_args; i++ )
    Ta->push_back ( new TypeExpr ( true ) ); 
  scope->add ( name, new Value ( f, R, Ta ) ); // Ta and its elements should be deleted when scope is deleted

}

bool init_ccl = false;

void gro_Program::init ( World * w ) {

  world = w;

  scope = new Scope; // deleted in ::destroy

  scope->push ( new SymbolTable ( 100 ) );

  // Initialize Globals : this is in the top scope.
  scope->add ( "dt", new Value ( world->get_sim_dt() ) );
  scope->add ( "just_divided", new Value ( false ) );
  scope->add ( "daughter", new Value ( false ) );
  scope->add ( "selected", new Value ( false ) );
  scope->add ( "volume", new Value ( 0.0000000001 ) );
  scope->add ( "id", new Value ( 0 ) ); // these values should be deleted when scope is deleted

  // command line arguments
  std::list<Value *> * args = new std::list<Value *>;

  int i;
  for ( i=0; i<argc; i++ ) {
      args->push_back ( new Value ( argv[i] ) );
  }

  scope->add ( "ARGC", new Value ( (int) args->size() ) );
  scope->add ( "ARGV", new Value ( args ) );

  parse_error = false;

  current_cell = NULL;

  int parse_ok;

  try {

      parse_ok = readOrganismProgram ( scope, pathname.c_str() );

  }

  catch ( std::string err ) {

      parse_error = true;
      throw err;

  }

  if ( parse_ok < 0 ) {
      parse_error = true;
      throw std::string ( "un-ported parse error" );
  }

  world_update_program = scope->get_program ( "main" );

  if ( world_update_program ) {
    world_update_program->init_params ( scope );
    world_update_program->init ( scope );
  } 

}

Value * gro_Program::eval ( World * world, Cell * cell, Expr * e ) {

  current_cell = cell;

  Program * gro = cell->get_gro_program();

  ASSERT ( gro != NULL );

  SymbolTable * symtab = gro->get_symtab();

  if ( cell->just_divided() ) {

    scope->assign ( "just_divided", new Value ( true ) );    
    cell->set_division_indicator ( false ); 

    if ( cell->is_daughter() ) {
      scope->assign ( "daughter", new Value ( true ) ); 
      cell->set_daughter_indicator ( false ); 
    } else {
      scope->assign ( "daughter", new Value ( false ) ); 
    }

  } else {

    scope->assign ( "just_divided", new Value ( false ) );    
    scope->assign ( "daughter", new Value ( false ) ); 

  }

  scope->assign ( "dt", new Value ( world->get_sim_dt() ) ); 
  scope->assign ( "volume", new Value ( cell->get_volume() ) ); 
  scope->assign ( "selected", new Value ( cell->is_selected() ) );
  scope->assign ( "id", new Value ( cell->get_id() ) );

  scope->push ( gro->get_symtab() );
  Value * v = e->eval ( scope );
  scope->pop();

  return v;

}

void gro_Program::update ( World * world, Cell * cell ) {

  if ( !parse_error ) {

  current_cell = cell;

  Program * gro = cell->get_gro_program();

  ASSERT ( gro != NULL );

  SymbolTable * symtab = gro->get_symtab();

  if ( cell->just_divided() ) {

    scope->assign ( "just_divided", new Value ( true ) );    
    cell->set_division_indicator ( false ); 

    if ( cell->is_daughter() ) {
      scope->assign ( "daughter", new Value ( true ) ); 
      cell->set_daughter_indicator ( false ); 
    } else {
      scope->assign ( "daughter", new Value ( false ) ); 
    }

  } else {

    scope->assign ( "just_divided", new Value ( false ) );    
    scope->assign ( "daughter", new Value ( false ) ); 

  }

  scope->assign ( "dt", new Value ( world->get_sim_dt() ) ); 
  scope->assign ( "volume", new Value ( cell->get_volume() ) ); 
  scope->assign ( "selected", new Value ( cell->is_selected() ) );
  scope->assign ( "id", new Value ( cell->get_id() ) );

  gro->step(scope);

  Value * v = symtab->get ( "gfp" );
  if ( v != NULL )
    cell->set_rep ( GFP, v->real_value() );

  v = symtab->get ( "rfp" );
  if ( v != NULL )
    cell->set_rep ( RFP, v->real_value() );

  v = symtab->get ( "yfp" );
  if ( v != NULL )
    cell->set_rep ( YFP, v->real_value() );

  v = symtab->get ( "cfp" );
  if ( v != NULL )
    cell->set_rep ( CFP, v->real_value() );

  current_cell = NULL;

  }

}

void gro_Program::world_update ( World * world ) {

  if ( !parse_error ) {

  if ( world_update_program ) 
    world_update_program->step ( scope );

  }

}

void gro_Program::destroy ( World * world ) {

  if ( scope != NULL ) {
    delete scope; // this would be the case if this isn't the first time init was called
  }

}

std::string gro_Program::name ( void ) const {

  return pathname;

}


void register_gro_functions ( void ) {

  // Parameters
  register_ccl_function ( "set", set_param );
  register_ccl_function ( "zoom", zoom );

  // Cell types
  register_ccl_function ( "ecoli", new_ecoli );
  register_ccl_function ( "c_ecolis", new_ecolis_random_circle );
  register_ccl_function ( "c_ecolis_growth", new_ecolis_random_circle_growth );
  //register_ccl_function ( "yeast", new_yeast );

  // Signals
  register_ccl_function ( "signal",        new_signal );
  register_ccl_function ( "set_signal",    set_signal );
  register_ccl_function ( "set_signal_rect", set_signal_rect );
  register_ccl_function ( "get_signal",    get_signal );
  register_ccl_function ( "emit_signal",   emit_signal );
  register_ccl_function ( "absorb_signal", absorb_signal );
  register_ccl_function ( "reaction",      add_reaction );
  register_ccl_function ( "get_signal_matrix", get_signal_matrix );

  register_ccl_function ( "cf_signal",        new_cross_feeding_signal );


  // World control
  register_ccl_function ( "reset",          reset );
  register_ccl_function ( "stop",           stop );
  register_ccl_function ( "start",          start );
  register_ccl_function ( "stats",          world_stats );
  register_ccl_function ( "snapshot",       snapshot );
  register_ccl_function ( "message",        message );
  register_ccl_function ( "clear_messages", clear_messages );
  register_ccl_function ( "set_theme",      set_theme );
  register_ccl_function ( "dump",           gro_dump );
  register_ccl_function ( "time",           gro_time );
  register_ccl_function ( "replate",        replate );
  register_ccl_function ( "select_random_cell",           select_random_cell );

  // Cell Specific
  register_ccl_function ( "die", die );
  register_ccl_function ( "die_c", die_c );
  register_ccl_function ( "divide", force_divide );
  register_ccl_function ( "geometry", geometry );
  register_ccl_function ( "run", run );
  register_ccl_function ( "tumble", tumble );

  // Misc
  register_ccl_function ( "map_to_cells", map_to_cells );
  register_ccl_function ( "print", gro_print );
  register_ccl_function ( "clear", gro_clear );
  register_ccl_function ( "chemostat", chemostat );
  register_ccl_function ( "barrier", barrier );
  register_ccl_function ( "dump_left",           gro_dump_left );
  register_ccl_function ( "dump_right",           gro_dump_right );
  register_ccl_function ( "dump_top",           gro_dump_top );
  register_ccl_function ( "dump_bottom",           gro_dump_bottom );

  // File I/O
  register_ccl_function ( "fopen", gro_fopen );
  register_ccl_function ( "fprint", gro_fprint );
  register_ccl_function ( "create_dirs",    create_dirs );

  register_ccl_function ( "dump_single", gro_dump_single );
  register_ccl_function ( "dump_single_reduced", gro_dump_single_reduced );
  register_ccl_function ( "dump_multiple", gro_dump_multiple );
  register_ccl_function ( "dump_multiple_plasmids", gro_dump_multiple_plasmids );
  register_ccl_function ( "dump_cond_conjugations", gro_dump_cond_conjugations );

  // Gene expression
  register_ccl_function ( "genes", new_operon );
  register_ccl_function ( "plasmids_genes", assign_operons_to_plasmids );
  register_ccl_function ( "action",    set_action);
  register_ccl_function ( "reset_actions",    reset_actions);

  // CellSignals
  register_ccl_function ( "s_signal",        s_add_signal );
  register_ccl_function ( "s_cf_signal",        s_new_cross_feeding_signal );
  register_ccl_function ( "grid",            grid );
  register_ccl_function ( "s_set_signal",    s_set_signal );
  register_ccl_function ( "s_set_signal_rect", s_set_signal_rect );
  register_ccl_function ( "s_get_signal",    s_get_signal );
  register_ccl_function ( "s_get_signal_area",    s_get_signal_area );
  register_ccl_function ( "s_emit_signal",   s_emit_signal );
  register_ccl_function ( "s_emit_signal_area",   s_emit_signal_area );
  register_ccl_function ( "s_emit_signal_point",   s_emit_signal_point );
  register_ccl_function ( "s_absorb_signal", s_absorb_signal );
  register_ccl_function ( "s_absorb_signal_area", s_absorb_signal_area );
  register_ccl_function ( "s_add_reaction",      s_add_reaction );

  register_ccl_function ( "s_emit_cf_signal",   s_emit_cross_feeding_signal );
  register_ccl_function ( "s_absorb_cf_signal", s_absorb_cross_feeding_signal );
  register_ccl_function ( "s_get_cf_signal", s_get_cross_feeding_signal );
  // register_ccl_function ( "change_matrix",      change_matrix );
  // register_ccl_function ( "s_absorb_QS",      s_absorb_QS );
  // register_ccl_function ( "s_get_QS",      s_get_QS );


  // Growth
  register_ccl_function ( "my_d_length", delta_length );
  register_ccl_function ( "available", my_available );

  //Phages
  register_ccl_function ( "local_moi", local_MOI );

  //


}
