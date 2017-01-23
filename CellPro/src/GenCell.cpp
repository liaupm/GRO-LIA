#include "../include/GenCell.h"
#include "../include/GenPlasmid.h"
#include "../include/GenOperon.h"
#include "../include/erreenege.h"
#include <iostream>

GenCell::GenCell(std::string name, int state, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact, float time){
    this->name = name;
    this->environment_plasmid = environment_plasmid;
    this->state=state; this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=parent;
    this->time=time;
	this->sigmaAct=0;
	this->sigmaDeg=0;
	
	//std::random_device seed1;
	//std::mt19937 generator (seed1);
	//generator = std::mt19937(seed1);
	
} // New gen with name and initial state.

GenCell::GenCell(std::string name, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact){
    this->name = name;
    this->state=0;
    this->environment_plasmid = environment_plasmid;
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=parent;
	this->time=-1;
    this->sigmaAct=0;
    this->sigmaDeg=0;
	//std::random_device seed1;
	//std::mt19937 generator (seed1);
	
} // New gen with name.

GenCell::GenCell(std::string name, int state, std::string target, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact, float time){
    this->name = name;
    this->environment_plasmid = environment_plasmid;
    this->state=state;
    this->target = target;
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=parent;
    this->time=time;
	this->sigmaAct=0;
	this->sigmaDeg=0;
	//std::random_device seed1;
	//std::mt19937 generator (seed1);
	
} // New gen with name and initial state.

GenCell::GenCell(std::string name, std::string target, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact){
    this->name = name;
    this->environment_plasmid = environment_plasmid;
    this->state=0; //-1?
    this->target = target;
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=parent;
	this->time=-1;
	this->sigmaAct=0;
	this->sigmaDeg=0;
	//std::random_device seed1;
	//std::mt19937 generator (seed1);

} // New gen with name.

GenCell::GenCell(std::string name, int state, int type, float time_act,float time_deact){
    this->name = name;
    this->environment_plasmid = NULL;
    this->state=state; 
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=NULL;
    this->time=-1;
    this->sigmaAct=0;
    this->sigmaDeg=0;
    //std::random_device seed1;
    //std::mt19937 generator (seed1);
    //generator = std::mt19937(seed1);

} // New gen with name and initial state.

GenCell::GenCell(std::string name, int state, std::string target, int type, float time_act,float time_deact){
    this->name = name;
    this->environment_plasmid = NULL;
    this->state=state;
    this->target = target;
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=NULL;
    this->time=-1;
    this->sigmaAct=0;
    this->sigmaDeg=0;
    //std::random_device seed1;
    //std::mt19937 generator (seed1);

} // New gen with name and initial state.

GenCell::GenCell(std::string name, std::string target, int type, float time_act, float time_deact){
    this->name = name;
    this->environment_plasmid = NULL;
    this->state=0; //-1?
    this->target = target;
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=NULL;
    this->time=-1;
    this->sigmaAct=0;
    this->sigmaDeg=0;
    //std::random_device seed1;
    //std::mt19937 generator (seed1);

} // New gen with name.

GenCell::GenCell(std::string name, int type,float time_act,float time_deact){
    this->name = name;
    this->environment_plasmid = NULL;
    this->state=0; //-1?
    this->target = "";
    this->type=type;
    this->block=false;
    this->time_act=time_act;
    this->time_deg=time_deact;
    this->free=false;
    this->parent=NULL;
	this->time=-1;
	this->sigmaAct=0;
	this->sigmaDeg=0;
	//std::random_device seed1;
	//std::mt19937 generator (seed1);
} // New gen with name.

std::string GenCell::getName(){
    return(this-> name);
}
GenOperon* GenCell ::getParent(){
	return(this->parent);
}

std::string GenCell::getTarget(){
    return(this-> target);
}

int GenCell::getState(){
    return(this-> state);
}

int GenCell::getType(){
    return(this-> type);
}

double GenCell::getTimeAct(){
    return(this-> time_act);
}

double GenCell::getTimeDeg(){
    return(this-> time_deg);
}

/*double GenCell::getTimeActError()
{
	return(this->time_act_error);
}

double GenCell::getTimeDegError()
{
	return(this->time_deg_error);
}*/

double GenCell::getSigmaAct ()
{
	return(this->sigmaAct);
}

double GenCell::getSigmaDeg ()
{
	return(this->sigmaDeg);
}

bool GenCell::isBlocked(){
    return(this -> block);
}

bool GenCell::isFree(){
    return(this -> free);
}

bool GenCell::guideGen(){
    bool guide= false;
    if (!this->isFree()){
        if (this-> target !=""){
            guide= true;
        }
    }
    return guide;
}

bool GenCell::compareCell(GenCell* cell){
    bool cequal = true;
    if(this->name != cell->getName() || this->type != cell->getType())
    {
            cequal = false;
    }
    return cequal;
}

void GenCell::setTimeAct(double time){
    this-> time_act=time;
}

void GenCell::setTimeDeg(double time){
    this-> time_deg=time;
}

/*void GenCell::setTimeActError(double time){
    this-> time_act_error=time;
}

void GenCell::setTimeDegError(double time){
    this-> time_deg_error=time;
}*/

void GenCell::setSigmaAct (double time)
{
	this->sigmaAct = time;
}
        
void GenCell::setSigmaDeg (double time)
{
	this->sigmaDeg = time;
}

void GenCell::setTarget (std::string name){
    if (!this->isFree()){
        this-> target = name;
    }
}

void GenCell::setBlock (bool block){
    if (!this->isFree()){
        this-> block = block;
    }
}

void GenCell::setFree (){
    this-> free = true;
}

void GenCell::activate(){
    if (!this->isBlocked() && !this->isFree()){
		this->state=1;
		this->time=-1;

    }
}

void GenCell::deactivate(){
    if (!this->isFree()){
        this->state=0;
		this->time=-1;
    }
}

void GenCell::erase(){
	if(!this->isFree()){
		this->setFree();
		if(this->state == 1){
			if (this->environment_plasmid != NULL){
				this->environment_plasmid->getOperon((unsigned int)0)->insertGens(this);
			}
		}
	}
	int auxindex = this->parent->getPos(this);
	if (this->parent != NULL && auxindex>=0){
		this->parent->getGens()->erase(this->parent->getGens()->begin()+auxindex);
	}
	delete this;
}

void GenCell::setTime(float currentTime){
	
	if(((this->parent)->getPromoter()->check_gates()== 1 )){ //seteo del timepo de activacion en el timestep correspondiente 
        this->time=rng->normal_float(time_act, sigmaAct)+currentTime;
		 
	}
	if(((this->parent)->getPromoter()->check_gates() == 0 )){
		this->time=rng->normal_float(time_deg, sigmaDeg)+currentTime;
	}
}

float GenCell::getTime(){//retorna el tiempo
	return (this->time);
}


void GenCell::changeState(){// cambia el estado de las proteinas cuando se produce el ruido.
	if(this->state==1){
		this->state=0;
	}
	else{
		this->state=1;
	}
		
}

void GenCell::setRNG(erreenege* rng)
{
	this->rng = rng;
}

void GenCell::setEnvPlasmid(GenPlasmid* env)
{
	this->environment_plasmid = env;
}
		
GenPlasmid* GenCell::getEnvPlasmid()
{
	return(this->environment_plasmid);
}

void GenCell::setParent(GenOperon* p)
{
	this->parent = p;
}
