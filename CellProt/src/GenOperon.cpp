#include "GenOperon.h"
#include "GenCell.h"
#include "GenPlasmid.h"
#include "erreenege.h"
#include "GenPromoter.h"
#include <cstdlib>
#include <random>

/* CONSTRUCTOR */

GenOperon::GenOperon(std::string id, GenPlasmid* parent, GenPlasmid* enviroment_plasmid,GenPromoter* promoter){
    this -> id= id;
    this -> gens = new std::vector<GenCell*>;
    this->enviroment_plasmid = enviroment_plasmid;
    this->free=false;
    this->parent=parent;
	this->promoter=promoter;
	this->tNoise1=0;
	this->tNoise2=0;
    this->noise=false;
    this->onTOoff=0;
	this->offTOon=0;
	this->lGate=-2;

}

GenOperon::GenOperon(std::string id, std::string name, GenPlasmid* parent, GenPlasmid* enviroment_plasmid,GenPromoter* promoter){
    this -> id= id;
    this -> name = name;
    this -> gens =  new std::vector<GenCell*>;
    this->enviroment_plasmid = enviroment_plasmid;
    this->free=false;
    this->parent=parent;
	this->promoter=promoter;
	this->tNoise1=0;
	this->tNoise2=0;
	this->noise=false;
	this->onTOoff=0;
	this->offTOon=0;
	this->lGate=-2;
}

GenOperon::GenOperon(std::string id,GenPromoter* promoter){
    this -> id= id;
    this -> gens = new std::vector<GenCell*>;
    this->enviroment_plasmid = NULL;
    this->free=false;
	this->promoter=promoter;
	this->tNoise1=0;
	this->tNoise2=0;
	this->noise=false;
    this->onTOoff=0;
	this->offTOon=0;
	this->lGate=-2;
}

GenOperon::GenOperon(std::string id, std::string name,GenPromoter* promoter){
    this -> id= id;
    this -> name = name;
    this -> gens =  new std::vector<GenCell*>;
    this->enviroment_plasmid = NULL;
    this->free=false;
    this->parent=NULL;
	this->promoter=promoter;
	this->tNoise1=0;
	this->tNoise2=0;
    this->noise=false;
    this->onTOoff=0;
	this->offTOon=0;
	this->lGate=-2;
}

/* ATRIBUTES */

std::string GenOperon::getId() {
    return(this->id);
}

std::string GenOperon::getName() {
    return(this->name);
}

GenOperon* GenOperon::getLast() {
    return(this->last);
}

unsigned int GenOperon::getSize() {
    return(this->gens->size());
}

int GenOperon::getPos(GenCell* gen ){
    int cont = -1;
    bool encontrado = false;
    GenCell* auxgen = NULL;
    if (gen != NULL && this->getSize()>0){
        cont = 0;
        do{
            auxgen = this->getGen(cont);
            if (auxgen != NULL && (auxgen->getName()).compare(gen->getName()) == 0){
                encontrado=true;
            }else{
                cont++;
            }
        }while(!encontrado && (unsigned int)cont< this->getSize());
    }
    return cont;
}

GenCell* GenOperon::getGen (GenCell* gen ){
    unsigned int cont = 0;
    bool encontrado = false;
    GenCell* auxgen = NULL;
    if (gen != NULL && this->getSize()>0){
        do{
            auxgen = this->getGen(cont);
            if (gen->compareCell(auxgen)){
                encontrado=true;
            }else{
                cont++;
            }
        }while(!encontrado && cont< this ->getSize());
        if(!encontrado){
            auxgen = NULL;
        }
    }
    return auxgen;
}

GenCell* GenOperon::getGen(unsigned int index){
    GenCell* gen = NULL;
    if (this-> getSize() > index){
       gen = this->gens->at(index);
    }
    return gen;
}

GenCell* GenOperon::getGen(std::string name){
    unsigned int cont = 0;
    bool encontrado = false;
    GenCell* gen = NULL;
    if (this->getSize()>0){
        do{
            if ((this ->getGen(cont)-> getName()).compare(name) == 0){
                gen = this ->gens->at(cont);
                encontrado=true;
            }
            cont++;
        }while(!encontrado && cont< this ->getSize());
    }
    return gen;
}

std::vector<GenCell*>* GenOperon::getGens() {
    return(this->gens);
}

std::map<std::string,int> GenOperon::getGensReleased() {
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        if(!(this ->getGen(i) -> isBlocked())){
            genMap.insert(std::pair<std::string, const int>(this ->gens->at(i) -> getName(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenOperon::getGensBlocked() {
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        if((this ->getGen(i) -> isBlocked())){
            genMap.insert(std::pair<std::string, const int>(this ->gens->at(i) -> getName(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenOperon::getGensActive() {
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        if((this ->getGen(i) -> getState()) == 1){
            genMap.insert(std::pair<std::string, const int>(this ->getGen(i) -> getName(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenOperon::getGensDeactive() {
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        if((this ->getGen(i) -> getState()) == 0){
            genMap.insert(std::pair<std::string, const int>(this ->getGen(i) -> getName(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenOperon::getGensGuide() {
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        if((this ->getGen(i) -> guideGen()) == true){
            genMap.insert(std::pair<std::string, const int>(this ->getGen(i)-> getName(), cont));
            cont++;
        }
    }
    return genMap;
}

bool GenOperon::isFree (){
    return(this -> free);
}

 /* CHECK GENS */

bool GenOperon::isGenExist(std::string name){
    bool aux = false;
    if (!isFree()){
        if(this -> getGen(name) != NULL){
            aux = true;
        }
     }
    return aux;
}

bool GenOperon::isGenBlocked(std::string name){
    bool blocked = false;
    if (!isFree()){
        if(this -> getGen(name) != NULL){
            blocked = this -> getGen(name) ->isBlocked();
        }
    }
    return blocked;
}

bool GenOperon::isGenActive(std::string name){
    bool active=false ;
    if (!isFree()){
		
        if(this -> getGen(name) != NULL){
            if (this -> getGen(name)->getState()==1){
                active = true;
            }
        }
    }
    return active;
}

bool GenOperon::isGenDeactive(std::string name){
    bool deactive = false;
    if (!isFree()){
        if(this -> getGen(name) != NULL){
            if (this -> getGen(name) ->getState()==0){
                deactive = true;
            }
        }
    }
    return deactive;
}

bool GenOperon::isGenGuide(std::string name){
    bool aux = false;
    if (!isFree()){
        if(this -> getGen(name) != NULL){
            aux = this -> getGen(name) ->guideGen();
        }
    }
    return aux;
}

bool GenOperon::isGenExist(std::map<std::string,int> sGens){
    bool exist = false;
    if (!isFree()){
        for(map<string,int>::iterator element = sGens.begin(); element != sGens.end(); ++element) {
            std::string gen = element -> first;
            bool aux = this ->isGenExist(gen);
            if (!aux && !exist){
                exist = aux;
            }
        }
    }
    return exist;
}

bool GenOperon::isGenActive(std::map<std::string,int> sGens){
    bool exist = false;
    if (!isFree()){
        for(map<string,int>::iterator element = sGens.begin(); element != sGens.end(); ++element) {
            std::string gen = element -> first;
            bool aux = this ->isGenActive(gen);
            if (!aux && !exist){
                exist = aux;
            }
        }
    }
    return exist;
}

bool GenOperon::isGenBlocked(std::map<std::string,int> sGens){
    bool blocked = false;
    if (!isFree()){
    	for(map<string,int>::iterator element = sGens.begin(); element != sGens.end(); ++element) {
            std::string gen = element -> first;
            bool aux = this ->isGenBlocked(gen);
            /*if (!aux && !blocked){
                blocked = aux;
            }*/
            if (aux){
                blocked = aux;
            }
        }
    }
    return blocked;
}

bool GenOperon::isGenDeactive(std::map<std::string,int> sGens){
    bool exist = false;
    if (!isFree()){
        for(map<string,int>::iterator element = sGens.begin(); element != sGens.end(); ++element) {
            std::string gen = element -> first;
            bool aux = this ->isGenDeactive(gen);
            if (!aux && !exist){
                exist = aux;
            }
        }
    }
    return exist;
}

bool GenOperon::isGenGuide(std::map<std::string,int> sGens){
    bool exist = false;
    if (!isFree()){
        for(map<string,int>::iterator element = sGens.begin(); element != sGens.end(); ++element) {
            std::string gen = element -> first;
            bool aux = this ->isGenGuide(gen);
            if (!aux && !exist){
                exist = aux;
            }
        }
    }
    return exist;
}

bool GenOperon::compareOperon(GenOperon* operon){
    bool oequal = false;
    if (this->getSize()>0 && this->getSize() == operon->getSize()){
        oequal = true;
        unsigned int cont=0;
        do{
            if( this->getGen(cont)->compareCell(operon->getGen(cont)) == false){
                oequal = false;
            }
            cont++;
        }while(oequal && cont < this->getSize());
    }
    return oequal;
}

 /* MANAGE OPERON */

void GenOperon::setFree(){
    if (!isFree()){
        this ->free = true;
    }
}

void GenOperon::insertGens (std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type,float time_act,float time_deact){
    GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
    aux->insertGens(this->getGens());
    this -> insertGens(initGens,type,time_act,time_deact);
    this -> activateGens(activeGens);
    this ->setLast(aux);
}

void GenOperon::insertGens (std::map<std::string,int> initGens, int type,float time_act,float time_deact){
	GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
    aux->insertGens(this->getGens());
    for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
    	std::string gen = element->first;
        GenCell* auxgen = this ->getGen(gen);
        if (auxgen == NULL){
        	GenCell* gennuevo = new GenCell (element->first, type,this, this->enviroment_plasmid, time_act, time_deact);
            if (this->isFree()){
            	gennuevo->setFree();
            }
            this->gens->push_back(gennuevo);
        }
    }
    this ->setLast(aux);
}

void GenOperon::insertGens (std::vector<GenCell*>* initGens){
    if (initGens != NULL){        
        for(unsigned int i=0; i< initGens->size();i++){
			
            this -> insertGens (initGens->at(i));
        }
    }
}

void GenOperon::insertGens (GenCell* initGens){
    if (initGens != NULL){
		
        this ->setLast(this);
        GenCell* auxgen = this ->getGen(initGens->getName());
	
        if (auxgen == NULL){
            GenCell* gennuevo = new GenCell(initGens->getName(),initGens->getState(), initGens->getTarget(), initGens->getType(),this, this->enviroment_plasmid,0,0);
            gennuevo->setTimeAct(initGens->getTimeAct());
            gennuevo->setTimeDeg(initGens->getTimeDeg());
			gennuevo->setTimeActError(initGens->getTimeActError());
            gennuevo->setTimeDegError(initGens->getTimeDegError());
            gennuevo->setBlock(initGens->isBlocked());
            if (this->isFree()){
                gennuevo->setFree();
            }
            this ->gens->push_back(gennuevo);
        }else{
            if (auxgen->getTimeDeg() > initGens->getTimeDeg()){
                GenCell* gen = new GenCell(initGens->getName(), initGens->getState(), initGens->getTarget(), initGens->getType(),this, this->enviroment_plasmid,0,0);
                gen->setTimeAct(initGens->getTimeAct());
                gen->setTimeDeg(initGens->getTimeDeg());
				gen->setTimeActError(initGens->getTimeActError());
				gen->setTimeDegError(initGens->getTimeDegError());
                gen->setBlock(initGens->isBlocked());
                if (initGens->isFree()){
                    gen->setFree();
                }
                unsigned int cont=0;
                bool done= false;
                do{
                    GenCell* gen1 = this->getGen(cont);
                    if (gen1->compareCell(gen)){
                        this->gens->erase(gens->begin()+cont);
                        delete gen1;
                        done=true;
                    }
                    cont++;
                }while(!done && cont<this->getSize());
                this ->gens->push_back(gen);
                delete initGens;
            }
        }
    }
}

void GenOperon::setTargetGen(std::string name, std::string target){
    if (!isFree()){
        this ->setLast(this);
        this ->getGen(name) ->setTarget(target);
    }
}

void GenOperon::setDegTimeGen(std::string name, double time){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->setTimeDeg(time);
        }
    }
}

void GenOperon::setActTimeGen(std::string name, double time){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->setTimeAct(time);
        }
    }
}

void GenOperon::setDegTimeErrorGen(std::string name, double time){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->setTimeDegError(time);
        }
    }
}

void GenOperon::setActTimeErrorGen(std::string name, double time){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->setTimeActError(time);
        }
    }
}

void GenOperon::activateGen(std::string name){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->activate();
        }
    }
}

void GenOperon::deactivateGen(std::string name){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->deactivate();
        }
    }
}

void GenOperon::eraseGen(std::string name){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->erase();
        }
        this->isEmptyErase();
    }
}

void GenOperon::blockGen(std::string name){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->setBlock(true);
        }
    }
}

void GenOperon::releaseGen(std::string name){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(name);
        if(gen != NULL){
            gen->setBlock(false);
        }
    }
}

void GenOperon::eraseGen(int index){
    if (!isFree()){
        this ->setLast(this);
        GenCell* gen = this -> getGen(index);
        if(gen != NULL){
            gen->erase();
        }
        this->isEmptyErase();
    }
}

void GenOperon::mutateGen(std::string name, int probability){
    if (!isFree()){
        this ->setLast(this);
        double randaux = (rand() % 100) + 1;
        // If the probability is equals or bigger than aux, the gen mutates and get erased.
        if(probability >= randaux){
           this -> eraseGen(name);
        }
    }
}

void GenOperon::activateGens(std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
        if (this->getSize()>1){
            aux->insertGens(this->getGens());
        }
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> activateGen(gen);
        }
        aux-> last = NULL;
        this ->last = aux;
    }
}

void GenOperon::deactivateGens(std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
        if (this->getSize()>1){
            aux->insertGens(this->getGens());
        }
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> deactivateGen(gen);
        }
        aux-> last = NULL;
        this ->last = aux;
    }
}

void GenOperon::eraseGens(std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
        if (this->getSize()>1){
            aux->insertGens(this->getGens());
        }
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> eraseGen(gen);
        }
        aux-> last = NULL;
        this ->last = aux;
        this->isEmptyErase();
    }
}

void GenOperon::blockGens(std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
        if (this->getSize()>1){
            aux->insertGens(this->getGens());
        }
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> blockGen(gen);
        }
        aux-> last = NULL;
        this ->last = aux;
    }
}

void GenOperon::releaseGens(std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* aux = new GenOperon(this->id,this->name,this->promoter);
        if (this->getSize()>1){
            aux->insertGens(this->getGens());
        }
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> releaseGen(gen);
        }
        aux-> last = NULL;
        this ->last = aux;
    }
}

void GenOperon::setLast (GenOperon* operon){
    if (!isFree()){
        GenOperon* aux = new GenOperon(operon->id,operon->name,operon->promoter);
        if (operon->getSize()>1){
            aux->insertGens(operon->getGens());
        }
        aux-> last = NULL;
        this ->last = aux;
    }
}

void GenOperon::isEmptyErase(){
     if (!isFree()){
        if(this->getSize() == 0){
            this-> erase();
        }
     }
}

void GenOperon::erase(){
    if(!this->isFree()){
            this -> setFree();
            if(this->getSize() != 0){
                    while (this -> getSize() >0){
                            this -> gens->at(0)->erase();
                    }
            }
    }
    int auxindex=this->parent->getPos(this);
    if (this->parent != NULL && auxindex >=0 ){
       this->parent->getOperons()->erase(this->parent->getOperons()->begin()+auxindex);
    }
    delete this;
}

/*float GenOperon::getonTOoff(){
	return(this->onTOoff);
}

float GenOperon::getoffTOon(){
	return (this->offTOon);
	
}*/

GenPromoter* GenOperon::getPromoter(){
    return(this->promoter);
}

GenPlasmid* GenOperon::getParent(){
    return (this->parent);
}

void GenOperon::sendToActivation(std::vector<GenCell *> cellAct, float currentTime){  
  
    this->parent->insertActList(cellAct, currentTime);
}

void GenOperon::sendToDeact(std::vector<GenCell *> cellDeact, float currentTime){
    
    this->parent->insertDecList(cellDeact, currentTime);
}

void GenOperon:: update(float currentTime){
	
    int gate=(this->promoter)->check_gates();
	
    if( gate==1 && (this->lGate)!=gate ){ //no se manda cuando el estado anterior es igual
            sendToActivation(*(this->getGens()), currentTime); //que se agrega? o son los activos? 
			(this->lGate)=gate;
    }
    if(gate==0 && (this->lGate)!=gate ){
			 
             sendToDeact(*(this->getGens()), currentTime);
            
			 (this->lGate)=gate;
    }
	
}

void GenOperon::changeTrue(){//para ruido permanente 
	(this->promoter)->setGate(1);
	(this->promoter)->eraseListA();//vacias listas de Activadores
	(this->promoter)->eraseListR();//vacias lista de Desactivadores
}

void GenOperon::changeFalse(){//para ruido permanente
	(this->promoter)->setGate(0);
	(this->promoter)->eraseListA();//vacias lista de Activadores
	(this->promoter)->eraseListR();//vacias lista de Desactivadores
}


float GenOperon::calculateNoise(float p){//calcula el ruido en el promotor
	float i;
	std::random_device seed1;
	std::mt19937 generator (seed1());
	std::uniform_real_distribution<double> distribution(0,1);	
	i=(log(distribution(generator))/log(1-p));
    return(i);
}

void GenOperon::setNoise(float currentTime)
{
	this->tNoise1 = currentTime + calculateNoise(this->onTOoff);
	this->tNoise2 = currentTime + calculateNoise(this->offTOon);
}

float* GenOperon:: getTimeNoise(){
	float *noises = new float[2];
	noises[0] = tNoise1;
	noises[1] = tNoise2;
	return(noises);
}

float GenOperon::getonTOoff()
{
	return(this->onTOoff);	
}

float GenOperon::getoffTOon()
{
	return(this->offTOon);
}

bool GenOperon::getNoisef()
{
	return(this->noise);
}
void GenOperon::setPromoter(GenPromoter* prom){
	this->promoter=prom;
}
int GenOperon::getlGate(){
	return (this->lGate);
}
