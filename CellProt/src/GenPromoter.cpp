#include "GenCell.h"
#include "GenPlasmid.h"
#include "GenOperon.h"
#include "GenPromoter.h"
#include "GenListPlasmid.h"

GenPromoter::GenPromoter(std::string id,int gate=2){
	this->id=id;
	this->gate=gate;
	//this->TA.clear();
	this->listPlasmid=NULL;
	//this->TD.clear();
	
}

int GenPromoter::check_gates(){
	if((this->listPlasmid)!=NULL){
	switch(this->gate)
	{
		
		case 1:
		return (this->true_gate());
		case -1 :
		return(this->false_gate());
		case 2:
		return(this->yes_gate());
		case 3:
		return(this->not_gate());
		case 4:
		return (this->and_gate());
		case 5:
		return (this->or_gate());
		case 6:
		return (this->xor_gate());
		default:
		return (this->yes_gate()); //A ver si esto sirve...
	}}
	return -1;
	
}
void GenPromoter::setListPlasmid(GenListPlasmid* list){
	
	this->listPlasmid=list;
}

void GenPromoter::setListTA(std::vector<GenCell*>  listTA){
	
	(this->TA)=listTA;
	
}
void GenPromoter::setListTD(std::vector<GenCell*> listTD){
	
	this->TD=listTD;
	
}


int GenPromoter::yes_gate(){
   if(TD.size()==0){

   if(( TA.size()==1 )  && itsInMapOne(this->TA)){
	   
	   return 1;
    }
	return 0;
	}
	return -1;
}

int GenPromoter::not_gate(){
	if(TA.size()==0){
		
	if(( (this->TD).size() == 1 )  && itsInMapOne(this->TD)){
		
	   return 0;
   }
	return 1;
	}
	return -1;
   
}

int GenPromoter::and_gate(){
   if((!itsInMapOne(this->TD) &&  TA.size()==0)|| (!itsInMapOne(this->TD) && itsInMapAll(this->TA)))
   {
	   return 1;
    }
	return 0;
}

int GenPromoter::or_gate(){
    if((itsInMapOne(this->TA))|| !itsInMapOne(this->TD) ){
	   return 1;
    }
	return 0;
   
}	

int GenPromoter::xor_gate(){
	
	//Revisar que la longitud de la suma de las listas sea 2 
	if((itsInMapAll(this->TA) && itsInMapAll(this->TD))|| 
	(itsInMapOne(this->TA) && !itsInMapAll(this->TA))||
	(itsInMapOne(this->TD) && !itsInMapAll(this->TD))||
	(!itsInMapOne(this->TA) && !itsInMapOne(this->TD))){
		return 1;
	}
	return 0;
}

int GenPromoter::true_gate(){
	if(TA.size()!=0 && TA.size()!=0){
		return -1;
	}
	return 1;
}
int GenPromoter::false_gate(){
	if(TA.size()!=0 && TD.size()!=0){
		return 0;
	}
	return -1;
}

bool GenPromoter::itsInMapAll(std::vector<GenCell*> vector){
   std::map<std::string,int> genMap = (this->listPlasmid)->getGensActive();
   if(vector.size()==0){return false;}
   string name;
  	for(std::vector<GenCell*>::iterator i =vector.begin();i!=vector.end();i++){
    name=(*i)->getName();
	if(genMap.find(name) == genMap.end()){
		return false;
	}	
  }
  return true;
}
int GenPromoter::getGate(){
	return(this->gate);
}
bool GenPromoter::itsInMapOne(std::vector<GenCell*> vector){
  std::map<std::string,int> genMap = (this->listPlasmid)->getGensActive();
  if(vector.size()==0){return false;}
  
  string name;
  for(std::vector<GenCell*>::iterator i =vector.begin();i!=vector.end();i++){
    name=(*i)->getName();
	if(genMap.find(name) != genMap.end()){
		return true;
	}
  }
return false;

}

void GenPromoter::setGate(int num){
	this->gate=num;
	this->TA.clear();
	this->TD.clear();
}


/*void GenPromoter::eraseListR(){
	for(std::vector<GenCell*>::iterator i =listTR.begin();i!=listTR.end();i++){
		listTR.erase(i);
	}
}
void GenPromoter::eraseListA(){
	for(std::vector<GenCell*>::iterator i =listTA.begin();i!=listTA.end();i++){
		listTA.erase(i);
	}
}*/
void GenPromoter::eraseListR(){
	for(std::vector<GenCell*>::iterator i =TD.begin();i!=TD.end();i++){
		TD.erase(i);
	}
}
void GenPromoter::eraseListA(){
	for(std::vector<GenCell*>::iterator i =TA.begin();i!=TA.end();i++){
		TA.erase(i);
	}
}