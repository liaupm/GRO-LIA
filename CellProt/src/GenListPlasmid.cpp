#include "GenListPlasmid.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

// States
#define ACTIVE 1
#define DEACTIVE 0
#define NOTEXIST -1

// Types
#define PROTEIN 0
#define RNA 1
#define DNA 2

/* CONSTRUCTOR */
GenListPlasmid::GenListPlasmid(std::string id, GenPlasmid* enviroment_plasmid){
    this -> id = id;
    this -> cont_ids=0;
    this -> enviroment_plasmid = enviroment_plasmid;
    this->plasmids = new std::vector<GenPlasmid*>;
}

GenListPlasmid::GenListPlasmid(std::string id){
    this -> id = id;
    this -> cont_ids=0;
    this -> enviroment_plasmid = NULL;
    this->plasmids = new std::vector<GenPlasmid*>;
}

/* ATRIBUTES */

std::string GenListPlasmid::getId() {
    return(this->id);
}

unsigned int GenListPlasmid::getSize() {
    return(this->plasmids->size());
}

int GenListPlasmid::getContIds() {
    return(this->cont_ids);
}

GenPlasmid* GenListPlasmid::getPlasmid(unsigned int index) {
    GenPlasmid* plasmid = NULL;
    if (this-> getSize() > index){
    	plasmid = this -> plasmids->at(index);
    }
    return plasmid;
}

GenPlasmid* GenListPlasmid::getPlasmid(GenPlasmid* plasmid ){
    unsigned int cont = 0;
    bool encontrado = false;
    GenPlasmid* auxplasm = NULL;
    if (plasmid != NULL && this->getSize()>0){
        do{
            auxplasm = this->getPlasmid(cont);
            if (plasmid->comparePlasmid(auxplasm)){
                encontrado=true;
            }else{
                cont++;
            }
        }while(!encontrado && cont< this ->getSize());
        if(!encontrado){
            auxplasm = NULL;
        }
    }
    return auxplasm;
}

GenPlasmid* GenListPlasmid::getPlasmidById(std::string id) {
    unsigned int cont = 0;
    bool encontrado = false;
    GenPlasmid* genplasmid = NULL;
    do{
        GenPlasmid* auxplasm = this -> getPlasmid(cont);
        if (auxplasm != NULL && (auxplasm-> getId()).compare(id) == 0){
        	genplasmid = auxplasm;
            encontrado=true;
        }
        cont++;
    }while(!encontrado && cont< this -> getSize());
    return genplasmid;
}

std::vector<GenPlasmid*>* GenListPlasmid::getPlasmidByName(std::string name) {
    unsigned int cont = 0;
    std::vector<GenPlasmid*>* aux = new std::vector<GenPlasmid*>;
    do{
    	GenPlasmid* auxplasm = this -> getPlasmid(cont);
        if (auxplasm != NULL && (auxplasm-> getName()).compare(name) == 0){
        	aux->push_back(auxplasm);
        }
        cont++;
    }while(cont< this ->getSize());
    return aux;
}

int GenListPlasmid::getPos(GenPlasmid* plasm){
    int cont = -1;
    bool encontrado = false;
    if (plasm != NULL){
        unsigned int conta = 0;
        do{
            GenPlasmid* auxplasm = this -> getPlasmid(cont);
            if (auxplasm != NULL && (auxplasm->getName()).compare(plasm->getName()) == 0){
                encontrado=true;
            }else{
                conta++;
            }
        }while(!encontrado && conta< this ->getSize());
        cont = conta;
    }
    return cont;
}

std::vector<GenPlasmid*>* GenListPlasmid::getPlasmids() {
    return(this -> plasmids);
}

/* CHECK PLASMIDS, OPERONS AND GENS */

std::map<std::string,int> GenListPlasmid::getGensBlocked(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
    	GenPlasmid* auxplasm = this -> getPlasmid(i);
        if(auxplasm != NULL){
        	std::map<std::string, int>  auxGenMap = auxplasm -> getGensBlocked();
            for(map<string,int>::iterator element = auxGenMap.begin(); element != auxGenMap.end(); ++element) {
            	std::string gen = element->first;
                if ( genMap.size()==0 || genMap.find(gen) == genMap.end() ) {
                	genMap.insert(std::pair<std::string, const int>(gen, cont));
                    cont++;
                }
            }
        }
    }
    return genMap;
}


std::map<std::string,int> GenListPlasmid::getGensReleased(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
    	GenPlasmid* auxplasm = this -> getPlasmid(i);
        if(auxplasm != NULL){
    		std::map<std::string, int>  auxGenMap = auxplasm -> getGensReleased();
            for(map<string,int>::iterator element = auxGenMap.begin(); element != auxGenMap.end(); ++element) {
            	std::string gen = element->first;
                if ( genMap.size()==0 || genMap.find(gen) == genMap.end() ) {
                	genMap.insert(std::pair<std::string, const int>(gen, cont));
                    cont++;
                }
            }
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::getGensActive(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
    	GenPlasmid* auxplasm = this -> getPlasmid(i);
        if(auxplasm != NULL){
        	std::map<std::string, int>  auxGenMap = auxplasm -> getGensActive();
            for(map<string,int>::iterator element = auxGenMap.begin(); element != auxGenMap.end(); ++element) {
            	std::string gen = element->first;
                if ( genMap.size()==0 || genMap.find(gen) == genMap.end() ) {
                	genMap.insert(std::pair<std::string, const int>(gen, cont));
                    cont++;
                }
            }
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::getGensDeactive(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
    	GenPlasmid* auxplasm = this -> getPlasmid(i);
        if(auxplasm != NULL){
        	std::map<std::string, int>  auxGenMap = auxplasm -> getGensDeactive();
            for(map<string,int>::iterator element = auxGenMap.begin(); element != auxGenMap.end(); ++element) {
            	std::string gen = element->first;
                if (  genMap.size()==0 || genMap.find(gen) == genMap.end() ) {
                	genMap.insert(std::pair<std::string, const int>(gen, cont));
                    cont++;
                }
            }
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::getGensGuide(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
    	GenPlasmid* auxplasm = this -> getPlasmid(i);
        if(auxplasm != NULL){
        	std::map<std::string, int>  auxGenMap = auxplasm -> getGensGuide();
            for(map<string,int>::iterator element = auxGenMap.begin(); element != auxGenMap.end(); ++element) {
            	std::string gen = element->first;
                if (  genMap.size()==0 || genMap.find(gen) == genMap.end() ) {
                	genMap.insert(std::pair<std::string, const int>(gen, cont));
                    cont++;
                }
            }
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenBlocked(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenBlocked(name);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenReleased(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenReleased(name);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenActive(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenActive(name);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenDeactive(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenDeactive(name);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
	}
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenExist(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenExist(name);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenBlocked(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenBlocked(sGens);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenReleased(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
	    std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenReleased(sGens);
        if(!aux.empty()){
    	    genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenActive(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenActive(sGens);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenDeactive(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenDeactive(sGens);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenListPlasmid::isGenExist(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
    	std::map<std::string, int>  aux = this ->getPlasmid(i) -> isGenExist(sGens);
        if(!aux.empty()){
        	genMap.insert(std::pair<std::string, const int>(this ->getPlasmid(i) -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

bool GenListPlasmid::isAvoidPlasmid(GenPlasmid* plasmid){
    bool result=false;
	unsigned int cont = 0;
	do{
		if (this->getPlasmid(cont)->isAvoidPlasmid(plasmid)){
			result=true;
		}
		cont++;
	}while(!result && cont < this->getSize());
	return result;
}

/* MANAGE LIST PLASMIDS */

void GenListPlasmid::setPlasmidOriginal(){
	for(unsigned int i=0;i<this -> getSize();i++){
    	this ->getPlasmid(i) -> setType(false);
    }
}

void GenListPlasmid::setTargetGen(std::string name, std::string target){
	for(unsigned int i=0;i<this -> getSize();i++){
    	this ->getPlasmid(i) -> setTargetGen(name,target);
    }
}

void GenListPlasmid::setDegTimeGen(std::string name, double time){
	for(unsigned int i=0;i<this -> getSize();i++){
    	this ->getPlasmid(i) -> setDegTimeGen(name,time);
    }
}

void GenListPlasmid::setActTimeGen(std::string name, double time){
    for(unsigned int i=0;i<this -> getSize();i++){
        this ->getPlasmid(i) -> setActTimeGen(name,time);
    }
}

void GenListPlasmid::insertPlasmid(GenPlasmid* genPlasmid) {
	if(genPlasmid != NULL){
    	if(!this-> isAvoidPlasmid(genPlasmid)){
        	std::string plasmid_id= this->id+"P"+to_string(this-> cont_ids);
            GenPlasmid * plasm = new GenPlasmid (plasmid_id,this, this->enviroment_plasmid);
            plasm->insertOperon(genPlasmid->getOperons());
            this ->plasmids->push_back(plasm);
            this -> cont_ids += 1;
        }
    }
}

void GenListPlasmid::insertPlasmid(std::vector<GenPlasmid*>* plasmidList) {
	for(unsigned int i=0;i<plasmidList->size();i++){
    	this->insertPlasmid(plasmidList->at(i));
    }
}

void GenListPlasmid::insertPlasmid(std::string name, GenPlasmid* genPlasmid) {
	if(genPlasmid != NULL){
    	if(!this-> isAvoidPlasmid(genPlasmid)){
        	std::string plasmid_id= this->id+"P"+to_string(this-> cont_ids);
            GenPlasmid * plasm = new GenPlasmid (plasmid_id, name,this, this->enviroment_plasmid);
            plasm->insertOperon(genPlasmid->getOperons());
            this ->plasmids->push_back(plasm);
            this -> cont_ids += 1;
        }
   	}
}

void GenListPlasmid::erasePlasmidById(std::string id) {
    unsigned int cont = 0;
	bool encontrado = false;
    do{
    	GenPlasmid* auxplasm = this ->getPlasmid(cont);
        if (auxplasm != NULL && (auxplasm-> getId()).compare(id) == 0){
        	auxplasm ->erase();
            encontrado=true;
        }else{
        	cont++;
        }
    }while(!encontrado && cont< this -> getSize());
}

void GenListPlasmid::erasePlasmidByName(std::string name) {
    unsigned int cont = 0;
    do{
    	GenPlasmid* auxplasm = this ->getPlasmid(cont);
        if (auxplasm != NULL && (auxplasm-> getName()).compare(name) == 0){
        	auxplasm -> erase();
        }else{
        	cont++;
        }
    }while(cont< this -> getSize());
}

void GenListPlasmid::erasePlasmid(unsigned int index) {
	if (this-> getSize() > index){
    	GenPlasmid* auxplasm = this ->getPlasmid(index);
        if(auxplasm != NULL){
        	auxplasm->erase();
        }
    }
}

void GenListPlasmid::blockGen(std::string name){
	for (unsigned int i=0; i < (this -> getSize()); i++){
    	GenPlasmid* auxplasm = this ->getPlasmid(i);
        	if(auxplasm != NULL){
            	auxplasm ->blockGen(name);
            }
    }
}

void GenListPlasmid::releaseGen(std::string name){
	for (unsigned int i=0; i < (this -> getSize()); i++){
    	GenPlasmid* auxplasm = this ->getPlasmid(i);
        if(auxplasm != NULL){
        	auxplasm -> releaseGen(name);
        }
    }
}

void GenListPlasmid::activateGen(std::string name){
	for (unsigned int i=0; i < (this -> getSize()); i++){
    	GenPlasmid* auxplasm = this ->getPlasmid(i);
        if(auxplasm != NULL){
        	auxplasm -> activateGen(name);
        }        
    }
}

void GenListPlasmid::deactivateGen(std::string name){
	for (unsigned int i=0; i < (this ->getSize()); i++){
    	GenPlasmid* auxplasm = this ->getPlasmid(i);
        if(auxplasm != NULL){
        	auxplasm -> deactivateGen(name);
        }
    }
}

void GenListPlasmid::eraseGen(std::string name){
	for (unsigned int i=0; i < (this -> getSize()); i++){
    	GenPlasmid* auxplasm = this ->getPlasmid(i);
        if(auxplasm != NULL){
        	auxplasm -> eraseGen(name);
        }
    }
}

void GenListPlasmid::mutateGen(std::string name, int probability){
	for (unsigned int i=0; i < (this -> getSize()); i++){
    	GenPlasmid* auxplasm = this ->getPlasmid(i);
        if(auxplasm != NULL){
        	auxplasm -> mutateGen(name,probability);
        }
    }
}

void GenListPlasmid::blockGenById(std::string plasmid_id, std::string name){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> blockGen(name);
    }
}

void GenListPlasmid::blockGenByName(std::string plasmid_name, std::string name){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> blockGen(name);
    }
}

void GenListPlasmid::releaseGenById(std::string plasmid_id, std::string name){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> releaseGen(name);
    }
}

void GenListPlasmid::releaseGenByName(std::string plasmid_name, std::string name){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> releaseGen(name);
    }
}

void GenListPlasmid::activateGenById(std::string plasmid_id, std::string name){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> activateGen(name);
    }
}

void GenListPlasmid::activateGenByName(std::string plasmid_name, std::string name){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> activateGen(name);
    }
}

void GenListPlasmid::deactivateGenById(std::string plasmid_id, std::string name){
    GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
        plasmid -> deactivateGen(name);
    }
}

void GenListPlasmid::deactivateGenByName(std::string plasmid_name, std::string name){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> deactivateGen(name);
    }
}

void GenListPlasmid::eraseGenById(std::string plasmid_id, std::string name){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> eraseGen(name);
    }
}

void GenListPlasmid::mutateGenById(std::string plasmid_id, std::string name, int probability){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> mutateGen(name,probability);
    }
}

void GenListPlasmid::eraseGenByName(std::string plasmid_name, std::string name){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> eraseGen(name);
    }
}

void GenListPlasmid::mutateGenByName(std::string plasmid_name, std::string name, int probability){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> mutateGen(name,probability);
    }
}

void GenListPlasmid::blockGen(unsigned int plasmid_index, std::string name){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> blockGen(name);
    }
}

void GenListPlasmid::releaseGen(unsigned int plasmid_index, std::string name){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> releaseGen(name);
    }
}

void GenListPlasmid::activateGen(unsigned int plasmid_index, std::string name){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> activateGen(name);
    }
}

void GenListPlasmid::deactivateGen(unsigned int plasmid_index, std::string name){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> deactivateGen(name);
    }
}

void GenListPlasmid::eraseGen(unsigned int plasmid_index, std::string name){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> eraseGen(name);
    }
}

void GenListPlasmid::mutateGen(unsigned int plasmid_index, std::string name, int probability){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> mutateGen(name,probability);
    }
}

void GenListPlasmid::blockGens(std::map<std::string,int> initGens){
	for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
    	std::string gen = element -> first;
        this -> blockGen(gen);
    }
}

void GenListPlasmid::releaseGens(std::map<std::string,int> initGens){
	for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
    	std::string gen = element -> first;
        this -> releaseGen(gen);
    }
}

void GenListPlasmid::activeGens(std::map<std::string,int> initGens){
	for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
    	std::string gen = element -> first;
        this -> activateGen(gen);
    }
}

void GenListPlasmid::deactiveGens(std::map<std::string,int> initGens){
	for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
    	std::string gen = element -> first;
        this -> deactivateGen(gen);
    }    
}

void GenListPlasmid::eraseGens(std::map<std::string,int> initGens){
	for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
    	std::string gen = element -> first;
        this -> eraseGen(gen);
    }
}

void GenListPlasmid::blockGensById(std::string plasmid_id, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> blockGens(initGens);
    }
}

void GenListPlasmid::blockGensByName(std::string plasmid_name, std::map<std::string,int> initGens){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> blockGens(initGens);
    }
}

void GenListPlasmid::releaseGensById(std::string plasmid_id, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> releaseGens(initGens);
    }
}

void GenListPlasmid::releaseGensByName(std::string plasmid_name, std::map<std::string,int> initGens){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> releaseGens(initGens);
    }
}

void GenListPlasmid::activeGensById(std::string plasmid_id, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> activateGens(initGens);
    }
}

void GenListPlasmid::activeGensByName(std::string plasmid_name, std::map<std::string,int> initGens){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> activateGens(initGens);
    }
}


void GenListPlasmid::deactiveGensById(std::string plasmid_id, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> deactivateGens(initGens);
    }
}

void GenListPlasmid::deactiveGensByName(std::string plasmid_name, std::map<std::string,int> initGens){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i)-> deactivateGens(initGens);
    }
}

void GenListPlasmid::eraseGensById(std::string plasmid_id, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmidById(plasmid_id);
    if(plasmid != NULL){
    	plasmid -> eraseGens(initGens);
    }
}

void GenListPlasmid::eraseGensByName(std::string plasmid_name, std::map<std::string,int> initGens){
	std::vector<GenPlasmid*>* plasmid = this -> getPlasmidByName(plasmid_name);
    for(unsigned int i=0; i< plasmid->size();i++){
    	plasmid->at(i) -> eraseGens(initGens);
    }
}

void GenListPlasmid::blockGens(unsigned int plasmid_index, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> blockGens(initGens);
    }
}

void GenListPlasmid::releaseGens(unsigned int plasmid_index, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> releaseGens(initGens);
    }
}

void GenListPlasmid::activeGens(unsigned int plasmid_index, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> activateGens(initGens);
    }
}

void GenListPlasmid::deactiveGens(unsigned int plasmid_index, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> deactivateGens(initGens);
    }
}

void GenListPlasmid::eraseGens(unsigned int plasmid_index, std::map<std::string,int> initGens){
	GenPlasmid* plasmid = this -> getPlasmid(plasmid_index);
    if(plasmid != NULL){
    	plasmid -> eraseGens(initGens);
    }
}

void GenListPlasmid::erase(){
	if(this->getSize() != 0){
    	while(this -> getPlasmid((unsigned int)0) != NULL){
    		this -> getPlasmid((unsigned int)0)->erase();
    	}
    }
    delete this;
}
void GenListPlasmid::macroPlasmid(float currentTime){
	for(std::vector<GenPlasmid*>::iterator i =plasmids->begin();i!=plasmids->end();i++){
			(*i)->macroPlasmid(currentTime);
			
	}
}

/* TEST MAIN */



/*int main()
{
	erreenege *rng = new erreenege(false, 10);
	GenPlasmid *Plas_env = new GenPlasmid("0","Plas_env");
	GenListPlasmid *plasmidlist =new GenListPlasmid("listP", Plas_env);
	//plasmido
	GenPlasmid *p = new GenPlasmid("1","Plas",plasmidlist,Plas_env);
    //promotores
	GenPromoter *prom0 = new GenPromoter("prom",1);//true
	GenPromoter *prom1 = new GenPromoter("prom",3);//not1
	GenPromoter *prom3 = new GenPromoter("prom",3);//not2
	GenPromoter *prom2 = new GenPromoter("prom",5);//or
	GenPromoter *prom = new GenPromoter("prom",3);//default
	//operon
    GenOperon *o=new GenOperon("1","operon",p,Plas_env,prom);
	
    
	//proteinas
    GenCell *pt1 = new GenCell("Prot1",0,0,40.0f,20.0f);
    GenCell *pt2 = new GenCell("Prot2",0,0,61.0f,20.3f);
    GenCell *pt3 = new GenCell("Prot3",0,0,20.0f,20.0f);
	GenCell *pt4 = new GenCell("Prot4",0,0,10.0f,20.0f);
	GenCell *pt5 = new GenCell("Prot5",0,0,0.1f,0.1f);
    //Creacion de la estructura
    
    
    p->insertOperon(o);
	p->insertOperon(o);
	p->insertOperon(o);
	p->insertOperon(o);
	p->insertOperon(o);
   
	plasmidlist->insertPlasmid(p);
   
	//Logica de las puertas
	std::vector<GenCell*> TA1;std::vector<GenCell*> TA2;std::vector<GenCell*> TA3;std::vector<GenCell*> TA4;
	TA1.push_back(pt1);//not1
	TA2.push_back(pt2);//not2
	TA3.push_back(pt4);//rep or 
	TA4.push_back(pt3);//act or
	prom1->setListTD(TA1);
	prom3->setListTD(TA2);
	prom2->setListTD(TA3);prom2->setListTA(TA4);
	
	
	
	//Relacion promotor Operon---SE NECESITAN LOS PUNTEROS REALES DEL SISTEMA
	unsigned int k =0;
	
	
	GenPlasmid* pp1=plasmidlist->getPlasmid(k);
	GenOperon* pp1o0=pp1->getOperon(k);
	GenOperon* pp1o1=pp1->getOperon(1);
	GenOperon* pp1o2=pp1->getOperon(2);
	GenOperon* pp1o3=pp1->getOperon(3);
	GenOperon* pp1o4=pp1->getOperon(4);
	
	pp1o0->insertGens(pt1);
	pp1o1->insertGens(pt2);
	pp1o2->insertGens(pt3);
	pp1o3->insertGens(pt4);
	pp1o4->insertGens(pt5);
	
	//seteo de los promotores sobre el operon
	pp1o0->setPromoter(prom0);
	pp1o1->setPromoter(prom0);
	pp1o2->setPromoter(prom1);
	pp1o3->setPromoter(prom3);
	pp1o4->setPromoter(prom2);
	
	//seteo promotor operon
	prom0->setListPlasmid(plasmidlist);//relacion puertas operon
	prom1->setListPlasmid(plasmidlist);
	prom2->setListPlasmid(plasmidlist);
	prom3->setListPlasmid(plasmidlist);
	
	
	//PROTEINA, ACTIVAREMOS A DEDO
	GenCell* pp1o1PT1=pp1o1->getGen(k);GenCell* pp1o1PT2=pp1o1->getGen(1);GenCell* pp1o1PT3=pp1o1->getGen(2);
	GenCell* pp1o2PT1=pp1o2->getGen(k);GenCell* pp1o2PT2=pp1o2->getGen(1);GenCell* pp1o2PT3=pp1o2->getGen(2);
	GenCell* pp2o1PT1=pp2o1->getGen(k);GenCell* pp2o1PT2=pp2o1->getGen(1);GenCell* pp2o1PT3=pp2o1->getGen(2);
	GenCell* pp2o2PT1=pp2o2->getGen(k);GenCell* pp2o2PT2=pp2o2->getGen(1);GenCell* pp2o2PT3=pp2o2->getGen(2);
	GenCell* pp3o1PT1=pp3o1->getGen(k);GenCell* pp3o1PT2=pp3o1->getGen(1);GenCell* pp3o1PT3=pp3o1->getGen(2);
	GenCell* pp3o2PT1=pp3o2->getGen(k);GenCell* pp3o2PT2=pp3o2->getGen(1);GenCell* pp3o2PT3=pp3o2->getGen(2);
	
	
	//ACTIVACION
	//pp1o1PT1->activate();
	//pp3o1PT2->activate();
	
	
	
    std::cout << "ListPlasmids => Id: " << plasmidlist -> getId() << "; Size: " << plasmidlist->getSize() << '\n';
    for (unsigned int k = 0; k < plasmidlist->getSize(); k++) {
        p = plasmidlist->getPlasmid(k);
        std::cout << "Plasmid => Id: " << p -> getId() << "; Name: " << p->getName() <<"; Size: " << p->getSize() << "; Free: " << p->isFree() << '\n';
        for (unsigned int j = 0; j < p->getSize(); j++) {
            o = p -> getOperon(j);
            std::cout << "\tOperon => Id: " << o -> getId() << "; Name: " << o->getName() <<"; Size: " << o->getSize() << "; Gate: " << o->getPromoter()->getGate() << '\n';
            for (unsigned int i = 0; i < o->getSize(); i++) {
				 o->getGen(i)->setRNG(rng);
                std::cout << "\t\tGen (" << o->getGen(i)->getName() << ") => Type: " <<o->getGen(i)->getType() << " => State: " <<o->getGen(i)->getState() << " => TimeDeg: " <<o->getGen(i)->getTimeDeg()<< " => TimeAct: " <<o->getGen(i)->getTimeAct() << " => TIME: " << o->getGen(i)->getTime() << '\n';
            }
        }
    }
	
	for (float i=0;i<100;i=i+0.1){
		
		plasmidlist->macroPlasmid(i);
	}
}*/





