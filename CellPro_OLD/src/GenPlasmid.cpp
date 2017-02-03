#include "../include/GenPlasmid.h"


 /* CONSTRUCTOR */

GenPlasmid::GenPlasmid(std::string id, GenListPlasmid* parent, GenPlasmid* environment_plasmid){
	this-> id = id;
    this -> cont_ids=0;
    this->type=true;
    this->environment_plasmid = environment_plasmid;
    this->free=false;
    this->parent=parent;
    this->operons = new std::vector<GenOperon*>;

	//this->actList= new std::vector<GenCell*>;
	//this->decList= new std::vector<GenCell*>;
}

GenPlasmid::GenPlasmid(std::string id, std::string name, GenListPlasmid* parent, GenPlasmid* environment_plasmid){
    this-> id = id;
    this-> name = name;
    this -> cont_ids=0;
    this->type=true;
    this->environment_plasmid = environment_plasmid;
    this->free=false;
    this->parent=parent;
    this->operons = new std::vector<GenOperon*>;
    //this->actList= new std::vector<GenCell*>;
    //this->decList= new std::vector<GenCell*>;
}

GenPlasmid::GenPlasmid(std::string id){
    this-> id = id;
    this -> cont_ids=0;
    this->type=true;
    this->environment_plasmid = NULL;
    this->free=false;
    this->operons = new std::vector<GenOperon*>;
    this->parent=NULL;
	//this->actList= new std::vector<GenCell*>;
	//this->decList= new std::vector<GenCell*>;
}

GenPlasmid::GenPlasmid(std::string id, std::string name){
    this-> id = id;
    this-> name = name;
    this -> cont_ids=0;
    this->type=true;
    this->environment_plasmid = NULL;
    this->free=false;
    this->operons = new std::vector<GenOperon*>;
    this->parent=NULL;
	//this->actList= new std::vector<GenCell*>;
	//this->decList= new std::vector<GenCell*>;
}

 /* ATRIBUTES */

unsigned int GenPlasmid::getSize() {
    return(this->operons->size());
}

bool GenPlasmid::getType() {
    return(this->type);
}

std::string GenPlasmid::getId() {
    return this -> id;
}

std::string GenPlasmid::getName() {
    return(this -> name);
}

int GenPlasmid::getContIds() {
    return(this->cont_ids);
}

GenOperon* GenPlasmid::getOperon(GenOperon* op ){
    unsigned int cont = 0;
    bool encontrado = false;
    GenOperon* auxop = NULL;
    if (op != NULL && this->getSize()>0){
        do{
            auxop = this->getOperon(cont);
            if (op->compareOperon(auxop)){
                encontrado=true;
            }else{
                cont++;
            }
        }while(!encontrado && cont< this ->getSize());
        if(!encontrado){
            auxop = NULL;
        }
    }
    return auxop;
}

GenOperon* GenPlasmid::getOperonById(std::string id){
    unsigned int cont = 0;
    bool encontrado = false;
    GenOperon* operon = NULL;
    do{
    	GenOperon* auxop = this -> getOperon(cont);
        if (auxop != NULL && (auxop-> getId()).compare(id) == 0){
        	operon = auxop;
            encontrado=true;
        }
        cont++;
    }while(!encontrado && cont< this ->getSize());
    return operon;
}

std::vector<GenOperon*>* GenPlasmid::getOperonByName(std::string name){
    unsigned int cont = 0;
    std::vector<GenOperon*>* aux = new std::vector<GenOperon*>;
    do{
    	GenOperon* auxop = this -> getOperon(cont);
        if (auxop != NULL && (auxop-> getName()).compare(name) == 0){
        	aux->push_back(auxop);
        }
        cont++;
    }while(cont< this ->getSize());
    return aux;
}

GenOperon* GenPlasmid::getOperon(unsigned int index){
    GenOperon* operon = NULL;
    if ((this-> getSize() > index)){
    	operon = this->operons->at(index);
    }
    return operon;
}

int GenPlasmid::getPos(GenOperon* op){
    int cont = -1;
    bool encontrado = false;
    if (op != NULL){
        cont = 0;
        do{
            GenOperon* auxop = this -> getOperon(cont);
            if (auxop != NULL && (auxop->getName()).compare(op->getName()) == 0){
                encontrado=true;
            }else{
                cont++;
            }
        }while(!encontrado && (unsigned int)cont< this ->getSize());
    }
    return cont;
}

std::vector<GenOperon*>* GenPlasmid::getOperons(){
    return(this -> operons);
}



bool GenPlasmid::isFree(){
    return(this ->free);
}

 /* CHECK GENS AND OPERONS */

std::map<std::string,int> GenPlasmid::getGensBlocked(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL){
            std::map<std::string, int>  auxGenMap = auxop -> getGensBlocked();
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

std::map<std::string,int> GenPlasmid::getGensReleased(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL){
            std::map<std::string, int>  auxGenMap = auxop -> getGensReleased();
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


std::map<std::string,int> GenPlasmid::getGensActive(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL){
            std::map<std::string, int>  auxGenMap = auxop -> getGensActive();
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

std::map<std::string,int> GenPlasmid::getGensDeactive(){
    std::map<std::string, int>  genMap;
    std::map<std::string, int>  activeGenMap = this->getGensActive();
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL){
            std::map<std::string, int>  auxGenMap = auxop -> getGensDeactive();
            for(map<string,int>::iterator element = auxGenMap.begin(); element != auxGenMap.end(); ++element) {
                std::string gen = element->first;
                if (  (genMap.size()==0 || genMap.find(gen) == genMap.end()) && activeGenMap.find(gen) == activeGenMap.end() ) {
                    genMap.insert(std::pair<std::string, const int>(gen, cont));
                    cont++;
                }
            }
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::getGensGuide(){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL){
            std::map<std::string, int>  auxGenMap = auxop -> getGensGuide();
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

std::map<std::string,int> GenPlasmid::isGenExist(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenExist(name)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenBlocked(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenBlocked(name)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenReleased(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenBlocked(name)) == false){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenActive(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this ->getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenActive(name)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenDeactive(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenDeactive(name)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}


std::map<std::string,int> GenPlasmid::isGenGuide(std::string name){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenGuide(name)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

bool GenPlasmid::isOperonExistById(std::string operon_id){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = true;
    }
    return result;
}

bool GenPlasmid::isOperonExistByName(std::string operon_name){
    bool result= false;
    if(this -> getOperonByName(operon_name)->size() > 0){
        result = true;
    }
    return result;
}

bool GenPlasmid::isOperonExist(unsigned int index){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = true;
    }
    return result;
}

bool GenPlasmid::isGenExist(unsigned int index, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenExist(name);
    }
    return result;
}

bool GenPlasmid::isGenExistByOpId(std::string operon_id, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenExist(name);
    }
    return result;
}

bool GenPlasmid::isGenExistByOpName(std::string operon_name, std::string name){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenExist(name)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenBlocked(unsigned int index, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenBlocked(name);
    }
    return result;
}

bool GenPlasmid::isGenBlockedByOpId(std::string operon_id, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenBlocked(name);
    }
    return result;
}

bool GenPlasmid::isGenBlockedByOpName(std::string operon_name, std::string name){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenBlocked(name)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}


bool GenPlasmid::isGenReleased(unsigned int index, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        if( op -> isGenBlocked(name)){
            result= false;
        }else{
            result = true;
        }
    }
    return result;
}

bool GenPlasmid::isGenReleasedByOpId(std::string operon_id, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        if( op -> isGenBlocked(name)){
            result= false;
        }else{
            result = true;
        }
    }
    return result;
}

bool GenPlasmid::isGenReleasedByOpName(std::string operon_name, std::string name){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (!aux->at(cont)-> isGenBlocked(name)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenGuide(unsigned int index, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenGuide(name);
    }
    return result;
}

bool GenPlasmid::isGenGuideByOpId(std::string operon_id, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenGuide(name);
    }
    return result;
}

bool GenPlasmid::isGenGuideByOpName(std::string operon_name, std::string name){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenGuide(name)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}


bool GenPlasmid::isGenActive(unsigned int index, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenActive(name);
    }
    return result;
}

bool GenPlasmid::isGenActiveByOpId(std::string operon_id, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenActive(name);
    }
    return result;
}

bool GenPlasmid::isGenActiveByOpName(std::string operon_name, std::string name){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenActive(name)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenDeactive(unsigned int index, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenDeactive(name);
    }
    return result;
}

bool GenPlasmid::isGenDeactiveByOpId(std::string operon_id, std::string name){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenDeactive(name);
    }
    return result;
}

bool GenPlasmid::isGenDeactiveByOpName(std::string operon_name, std::string name){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenDeactive(name)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::comparePlasmid(GenPlasmid* plasmid){
    bool pequal = false;
    if (this->getSize() == plasmid->getSize()){
        pequal = true;
        unsigned int cont=0;
        do{
            if(this->getOperon(cont)->compareOperon(plasmid->getOperon(cont)) == false){
                pequal = false;
            }
            cont++;
        }while(pequal && cont < this->getSize());
    }
    return pequal;
}

// CHECK A GROUP OF GENS

std::map<std::string,int> GenPlasmid::isGenExist(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenExist(sGens)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenBlocked(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenBlocked(sGens)) == true){ //Que quisiste decir aqui?
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenReleased(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenBlocked(sGens)) == false){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenGuide(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenGuide(sGens)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenActive(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenActive(sGens)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

std::map<std::string,int> GenPlasmid::isGenDeactive(std::map<std::string,int> sGens){
    std::map<std::string, int>  genMap;
    int cont=0;
    for (unsigned int i=0; i < (this -> getSize()); i++){
        GenOperon* auxop = this -> getOperon(i);
        if(auxop != NULL && (auxop -> isGenDeactive(sGens)) == true){
            genMap.insert(std::pair<std::string, const int>(auxop -> getId(), cont));
            cont++;
        }
    }
    return genMap;
}

bool GenPlasmid::isGenExist(unsigned int index, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenExist(sGens);
    }
    return result;
}

bool GenPlasmid::isGenExistByOpId(std::string operon_id, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenExist(sGens);
    }
    return result;
}

bool GenPlasmid::isGenExistByOpName(std::string operon_name, std::map<std::string,int> sGens){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenExist(sGens)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenBlocked(unsigned int index, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenBlocked(sGens);
    }
    return result;
}

bool GenPlasmid::isGenBlockedByOpId(std::string operon_id, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenBlocked(sGens);
    }
    return result;
}

bool GenPlasmid::isGenBlockedByOpName(std::string operon_name, std::map<std::string,int> sGens){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenBlocked(sGens)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenReleased(unsigned int index, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        if(op -> isGenBlocked(sGens)){
            result =false;
        }else{
            result= true;
        }
    }
    return result;
}

bool GenPlasmid::isGenReleasedByOpId(std::string operon_id, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        if(op -> isGenBlocked(sGens)){
            result =false;
        }else{
            result= true;
        }
    }
    return result;
}

bool GenPlasmid::isGenReleasedByOpName(std::string operon_name, std::map<std::string,int> sGens){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenBlocked(sGens)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}


bool GenPlasmid::isGenGuide(unsigned int index, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenGuide(sGens);
    }
    return result;
}

bool GenPlasmid::isGenGuideByOpId(std::string operon_id, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenGuide(sGens);
    }
    return result;
}

bool GenPlasmid::isGenGuideByOpName(std::string operon_name, std::map<std::string,int> sGens){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenGuide(sGens)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenActive(unsigned int index, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenActive(sGens);
    }
    return result;
}

bool GenPlasmid::isGenActiveByOpId(std::string operon_id, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenActive(sGens);
    }
    return result;
}

bool GenPlasmid::isGenActiveByOpName(std::string operon_name, std::map<std::string,int> sGens){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenActive(sGens)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

bool GenPlasmid::isGenDeactive(unsigned int index, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperon(index);
    if(op != NULL){
        result = op -> isGenDeactive(sGens);
    }
    return result;
}

bool GenPlasmid::isGenDeactiveByOpId(std::string operon_id, std::map<std::string,int> sGens){
    bool result= false;
    GenOperon* op = this -> getOperonById(operon_id);
    if(op != NULL){
        result = op -> isGenDeactive(sGens);
    }
    return result;
}

bool GenPlasmid::isGenDeactiveByOpName(std::string operon_name, std::map<std::string,int> sGens){
    bool result = false;
    std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
    if(aux->size() > 0){
        unsigned int cont=0;
        do{
            if (aux->at(cont)-> isGenDeactive(sGens)){
                result = true;
            }
            cont++;
        }while(!result && cont< aux->size());
    }
    return result;
}

 /* MANAGE PLASMIDS */

void GenPlasmid::setType(bool type) {
    if (!isFree()){
        this->type = type;
    }
}

void GenPlasmid::setFree(){
    if (!isFree()){
        this ->free = true;
    }
}

void GenPlasmid::setTargetGen(std::string name, std::string target){
    if (!isFree()){
        for(unsigned int i=0;i<this -> getSize();i++){
            GenOperon* auxop = this -> getOperon(i);
            if (auxop != NULL){
                auxop -> setTargetGen(name,target);
            }
        }
    }
}

void GenPlasmid::setDegTimeGen(std::string name, double time){
    if (!isFree()){
        for(unsigned int i=0;i<this -> getSize();i++){
            GenOperon* auxop = this -> getOperon(i);
            if (auxop != NULL){
                auxop -> setDegTimeGen(name,time);
            }
        }
    }
}

void GenPlasmid::setActTimeGen(std::string name,double time){
    if (!isFree()){
        for(unsigned int i=0;i<this -> getSize();i++){
            GenOperon* auxop = this -> getOperon(i);
            if (auxop != NULL){
                auxop -> setActTimeGen(name,time);
            }
        }
    }
}

// INSERT OPERONS AT THE END.
void GenPlasmid::insertOperon(std::map<std::string,int> initGens, int type, GenPromoter *prom){
    if(initGens.size()>0){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id, this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,type,0.0f,0.0f);
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom){
    if(initGens.size()>0 && activeGens.size()>0 ){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id, this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,activeGens,type,0.0f,0.0f);
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(std::string name, std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom){
    if(initGens.size()>0 && activeGens.size()>0 ){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id, name, this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,activeGens,type,0.0f,0.0f);
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(std::string name, std::map<std::string,int> initGens, int type, GenPromoter *prom){
    if(initGens.size()>0 ){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id, name, this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,type,0.0f,0.0f);
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(GenOperon* operon){
    if (operon != NULL){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenPromoter * p_copy = new GenPromoter(operon->getPromoter()->getId(), operon->getPromoter()->getGate());
        p_copy->setListTA(operon->getPromoter()->getListTA());
        p_copy->setListTD(operon->getPromoter()->getListTD());
        //GenOperon * op = new GenOperon (operon_id, operon->getName(), this, this->environment_plasmid, operon->getPromoter());
        GenOperon * op = new GenOperon (operon_id, operon->getName(), this, this->environment_plasmid, p_copy);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(operon->getGens());
        op->setNoisef(operon->getNoisef());
        op->setoffTOon(operon->getoffTOon());
        op->setonTOoff(operon->getonTOoff());
        op->setllGate(operon->getllGate());
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperonConj(GenOperon* operon){
    if (operon != NULL){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenPromoter * p_copy = new GenPromoter(operon->getPromoter()->getId(), operon->getPromoter()->getGate());
        p_copy->setListTA(operon->getPromoter()->getListTA());
        p_copy->setListTD(operon->getPromoter()->getListTD());
        GenOperon * op = new GenOperon (operon_id, operon->getName(), this, this->environment_plasmid, p_copy);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(operon->getGens());
        op->setNoisef(operon->getNoisef());
        op->setoffTOon(operon->getoffTOon());
        op->setonTOoff(operon->getonTOoff());
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(std::string name, GenOperon* operon){
    if (operon != NULL){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenPromoter * p_copy = new GenPromoter(operon->getPromoter()->getId(), operon->getPromoter()->getGate());
        p_copy->setListTA(operon->getPromoter()->getListTA());
        p_copy->setListTD(operon->getPromoter()->getListTD());
        //GenOperon * op = new GenOperon (operon_id, name, this, this->environment_plasmid, operon->getPromoter());
        GenOperon * op = new GenOperon (operon_id, name, this, this->environment_plasmid, p_copy);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(operon->getGens());
        op->setNoisef(operon->getNoisef());
		op->setoffTOon(operon->getoffTOon());
        op->setonTOoff(operon->getonTOoff());
        op->setllGate(operon->getllGate());
        this -> operons->push_back(op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(std::vector<GenOperon*>* initOperons){
    for(unsigned int i=0;i<initOperons->size();i++){
        this ->insertOperon(initOperons->at(i));
    }
}

void GenPlasmid::insertOperonConj(std::vector<GenOperon*>* initOperons){
    for(unsigned int i=0;i<initOperons->size();i++){
        this ->insertOperonConj(initOperons->at(i));
    }
}

// INSERT OPERONS IN A POSITION.

void GenPlasmid::insertOperon(unsigned int index, std::map<std::string,int> initGens, int type, GenPromoter *prom){
   if(initGens.size()>0 && index <= getSize()){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id,this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,type,0.0f,0.0f);
        this -> operons->insert(this->operons->begin()+index,op);
        this-> cont_ids++;
   }
}
GenListPlasmid* GenPlasmid::getParent(){
    return (this->parent);
}
void GenPlasmid::insertOperon(unsigned int index, std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom){
    if(initGens.size()>0 && activeGens.size()>0 && index <= this->getSize()){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id,this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,activeGens,type,0.0f,0.0f);
        this -> operons->insert(this->operons->begin()+index,op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(unsigned int index, std::string name, std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom){
    if(initGens.size()>0 && activeGens.size()>0 && index <=this->getSize()){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id, name,this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,activeGens,type,0.0f,0.0f);
        this -> operons->insert(this->operons->begin()+index,op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(unsigned int index, std::string name, std::map<std::string,int> initGens, int type, GenPromoter *prom){
    if(initGens.size()>0 && index <= this->getSize()){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        GenOperon * op = new GenOperon (operon_id, name,this, this->environment_plasmid, prom);
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(initGens,type,0.0f,0.0f);
        this -> operons->insert(this->operons->begin()+index,op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(unsigned int index, GenOperon* operon){
    if (operon && index <= this->getSize()){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        //GenPromoter * p_copy = new GenPromoter(operon->getPromoter()->getId(), operon->getPromoter()->getGate());
        GenOperon * op = new GenOperon (operon_id, operon->getName(),this, this->environment_plasmid, operon->getPromoter());
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(operon->getGens());
        op->setNoisef(operon->getNoisef());
		op->setoffTOon(operon->getoffTOon());
        op->setonTOoff(operon->getonTOoff());
        op->setllGate(operon->getllGate());
        this -> operons->insert(this->operons->begin()+index,op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(unsigned int index, std::string name, GenOperon* operon){
    if (operon && index <=this->getSize()){
        std::string operon_id= id+"O"+std::to_string(this-> cont_ids);
        //GenPromoter * p_copy = new GenPromoter(operon->getPromoter()->getId(), operon->getPromoter()->getGate());
        GenOperon * op = new GenOperon (operon_id, name,this, this->environment_plasmid, operon->getPromoter());
        if (this->isFree()){
            op->setFree();
        }
        op->insertGens(operon->getGens());
        op->setNoisef(operon->getNoisef());
		op->setoffTOon(operon->getoffTOon());
        op->setonTOoff(operon->getonTOoff());
        op->setllGate(operon->getllGate());
        this -> operons->insert(this->operons->begin()+index,op);
        this-> cont_ids++;
    }
}

void GenPlasmid::insertOperon(unsigned int index, std::vector<GenOperon*>* initOperons){
    for(unsigned int i=0;i<initOperons->size();i++){
        this ->insertOperon((index+i),initOperons->at(i));
    }
}

// ERASE OPERONS

void GenPlasmid::eraseOperon(unsigned int index){
    if (!isFree()){
        if ((this-> getSize() > index)){
            GenOperon * auxop = this -> getOperon(index);
            auxop->erase();
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::eraseOperonById(std::string operon_id){
    unsigned int cont = 0;
    bool encontrado = false;
    if (!isFree()){
        do{
            if (this ->getSize() > 0 && (this ->getOperon(cont)-> getId()).compare(operon_id) == 0){
                GenOperon * auxop = this -> getOperon(cont);
                auxop->erase();
                encontrado=true;
            }else{
                cont++;
            }
        }while(!encontrado && cont< this -> getSize());
        this->isEmptyErase();
    }
}

void GenPlasmid::eraseOperonByName(std::string operon_name){
    unsigned int cont = 0;
    if (!isFree()){
        do{
            if (this ->getSize()>0 && (this ->getOperon(cont)-> getName()).compare(operon_name) == 0){
                GenOperon * auxop = this -> getOperon(cont);
                auxop->erase();
            }else{
                cont++;
            }

        }while( cont< this -> getSize());
        this->isEmptyErase();
    }
}

void GenPlasmid::blockGen(std::string name){
    if (!isFree()){
        for (unsigned int i=0; i < (this -> getSize()); i++){
            GenOperon* auxop = this->getOperon(i);
            if(auxop != NULL){
                auxop -> blockGen(name);
            }
        }
    }
}

void GenPlasmid::releaseGen(std::string name){
    if (!isFree()){
        for (unsigned int i=0; i < (this -> getSize()); i++){
            GenOperon* auxop = this->getOperon(i);
            if(auxop != NULL){
                auxop -> releaseGen(name);
            }
        }
    }
}

void GenPlasmid::activateGen(std::string name){
    if (!isFree()){
        for (unsigned int i=0; i < (this -> getSize()); i++){
            GenOperon* auxop = this->getOperon(i);
            if(auxop != NULL){
                auxop -> activateGen(name);
            }
        }
    }
}

void GenPlasmid::deactivateGen(std::string name){
    if (!isFree()){
        for (unsigned int i=0; i < (this ->getSize()); i++){
            GenOperon* auxop = this->getOperon(i);
            if(auxop != NULL){
               auxop -> deactivateGen(name);
            }
        }
    }
}

void GenPlasmid::eraseGen(std::string name){
    if (!isFree()){
        for (unsigned int i=0; i < (this -> getSize()); i++){
            GenOperon* auxop = this->getOperon(i);
            if(auxop != NULL){
                 auxop -> eraseGen(name);
            }
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::mutateGen(std::string name, int probability){
    if (!isFree()){
        for (unsigned int i=0; i < (this -> getSize()); i++){
            GenOperon* auxop = this->getOperon(i);
            if(auxop != NULL){
                auxop -> mutateGen(name,probability);
            }
        }
    }
}

void GenPlasmid::blockGenByOpId(std::string operon_id, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> blockGen(name);
        }
    }
}

void GenPlasmid::blockGenByOpName(std::string operon_name, std::string name){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> blockGen(name);
        }
    }
}

void GenPlasmid::releaseGenByOpId(std::string operon_id, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> releaseGen(name);
        }
    }
}

void GenPlasmid::releaseGenByOpName(std::string operon_name, std::string name){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> releaseGen(name);
        }
    }
}

void GenPlasmid::activateGenByOpId(std::string operon_id, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> activateGen(name);
        }
    }
}

void GenPlasmid::activateGenByOpName(std::string operon_name, std::string name){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> activateGen(name);
        }
    }
}

void GenPlasmid::deactivateGenByOpId(std::string operon_id, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> deactivateGen(name);
        }
    }
}

void GenPlasmid::deactivateGenByOpName(std::string operon_name, std::string name){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> deactivateGen(name);
        }
    }
}

void GenPlasmid::eraseGenByOpId(std::string operon_id, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> eraseGen(name);
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::mutateGenByOpId(std::string operon_id, std::string name, int probability){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> mutateGen(name,probability);
        }
    }
}

void GenPlasmid::eraseGenByOpName(std::string operon_name, std::string name){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> eraseGen(name);
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::mutateGenByOpName(std::string operon_name, std::string name, int probability){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> mutateGen(name,probability);
        }
    }
}

void GenPlasmid::activateGen(unsigned int operon_index, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> activateGen(name);
        }
    }
}

void GenPlasmid::blockGen(unsigned int operon_index, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> blockGen(name);
        }
    }
}

void GenPlasmid::releaseGen(unsigned int operon_index, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> releaseGen(name);
        }
    }
}

void GenPlasmid::deactivateGen(unsigned int operon_index, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> deactivateGen(name);
        }
    }
}

void GenPlasmid::eraseGen(unsigned int operon_index, std::string name){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> eraseGen(name);
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::mutateGen(unsigned int operon_index, std::string name, int probability){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> mutateGen(name,probability);
        }
    }
}

void GenPlasmid::blockGens(std::map<std::string,int> initGens){
    if (!isFree()){
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> blockGen(gen);
        }
    }
}

void GenPlasmid::releaseGens(std::map<std::string,int> initGens){
    if (!isFree()){
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> releaseGen(gen);
        }
    }
}

void GenPlasmid::activateGens(std::map<std::string,int> initGens){
    if (!isFree()){
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> activateGen(gen);
        }
    }
}

void GenPlasmid::deactivateGens(std::map<std::string,int> initGens){
    if (!isFree()){
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> deactivateGen(gen);
        }
    }
}

void GenPlasmid::eraseGens(std::map<std::string,int> initGens){
    if (!isFree()){
        for(map<string,int>::iterator element = initGens.begin(); element != initGens.end(); ++element) {
            std::string gen = element -> first;
            this -> eraseGen(gen);
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::blockGensByOpId(std::string operon_id, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> blockGens(initGens);
        }
    }
}

void GenPlasmid::blockGensByOpName(std::string operon_name, std::map<std::string,int> initGens){
    if (!isFree()){
       std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> blockGens(initGens);
        }
    }
}

void GenPlasmid::releaseGensByOpId(std::string operon_id, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> releaseGens(initGens);
        }
    }
}

void GenPlasmid::releaseGensByOpName(std::string operon_name, std::map<std::string,int> initGens){
    if (!isFree()){
       std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> releaseGens(initGens);
        }
    }
}

void GenPlasmid::activateGensByOpId(std::string operon_id, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> activateGens(initGens);
        }
    }
}

void GenPlasmid::activateGensByOpName(std::string operon_name, std::map<std::string,int> initGens){
    if (!isFree()){
       std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
        for(unsigned int i=0;i<aux->size();i++){
            aux->at(i) -> activateGens(initGens);
        }
    }
}

void GenPlasmid::deactivateGensByOpId(std::string operon_id, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> deactivateGens(initGens);
        }
    }
}

void GenPlasmid::deactivateGensByOpName(std::string operon_name, std::map<std::string,int> initGens){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
         for(unsigned int i=0;i<aux->size();i++){
             aux->at(i) -> deactivateGens(initGens);
         }
    }
}

void GenPlasmid::eraseGensByOpId(std::string operon_id, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperonById(operon_id);
        if(op != NULL){
            op -> eraseGens(initGens);
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::eraseGensByOpName(std::string operon_name, std::map<std::string,int> initGens){
    if (!isFree()){
        std::vector<GenOperon*>* aux = this -> getOperonByName(operon_name);
         for(unsigned int i=0;i<aux->size();i++){
             aux->at(i) ->eraseGens(initGens);
         }
         this->isEmptyErase();
    }
}

void GenPlasmid::activateGens(unsigned int operon_index, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> activateGens(initGens);
        }
    }
}

void GenPlasmid::blockGens(unsigned int operon_index, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> blockGens(initGens);
        }
    }
}

void GenPlasmid::releaseGens(unsigned int operon_index, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> releaseGens(initGens);
        }
    }
}

void GenPlasmid::deactivateGens(unsigned int operon_index, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> deactivateGens(initGens);
        }
    }
}

void GenPlasmid::eraseGens(unsigned int operon_index, std::map<std::string,int> initGens){
    if (!isFree()){
        GenOperon* op = this -> getOperon(operon_index);
        if(op != NULL){
            op -> eraseGens(initGens);
        }
        this->isEmptyErase();
    }
}

void GenPlasmid::isEmptyErase(){
     if (!isFree()){
        if(this->getSize() == 0){
            this-> erase();
        }
     }
}

void GenPlasmid::erase(){
    if(!this-> isFree()){
            this -> setFree();
            if(this->getSize() != 0){
               while(this -> getOperon((unsigned int)0) != NULL){
                            this -> getOperon((unsigned int)0)->erase();
                    }
            }
    }
    int auxindex=this->parent->getPos(this);
    if (this->parent != NULL && auxindex >=0 ){
       this->parent->getPlasmids()->erase(this->parent->getPlasmids()->begin()+auxindex-1);
    }
    delete this;
}

void GenPlasmid::insertDecList(std::vector<GenCell*> cellDeact, float currentTime){
    for(std::vector<GenCell*>::iterator j =cellDeact.begin();j!=cellDeact.end();j++){
            //(*j)->setTime(currentTime);
			if((*j)->getState()!=0){
                (*j)->setTime(currentTime);
				(this->decList).insert(decList.end(),*j);//mete el gen cellDeact en la lista de deact
				(this->aux).push_back(*j);
			}
	
    }
    remakelist(actList);
}

void GenPlasmid::insertActList(std::vector<GenCell*> cellAct, float currentTime){
    for(std::vector<GenCell*>::iterator j =cellAct.begin();j!=cellAct.end();j++){
            //(*j)->setTime(currentTime);
			if((*j)->getState()!=1){
                (*j)->setTime(currentTime);
				(this->actList).insert(actList.end(),*j);//mete el gen cellDeact en la lista de act
				(this->aux).push_back(*j);
				}
			}
    remakelist(decList);
}

void GenPlasmid::setActList(std::vector<GenCell*> cellAct){
    this->actList = cellAct;//mete el gen cellDeact en la lista de act
}

void GenPlasmid::setDecList(std::vector<GenCell*> cellAct){
    this->decList = cellAct;//mete el gen cellDeact en la lista de dec
}


void GenPlasmid::checkList(std::vector<GenCell*> list, float time){
	
    for(std::vector<GenCell*>::iterator i =list.begin();i!=list.end();i++){
			
			float timeP=(*i)->getTime();
			
            if( (time >= timeP)&& (timeP!=-1) && (*i)->getState()==1 && ((*i)->getParent()->getPromoter()->check_gates() == 0) ){
                     //std::cout<< "Proteina " << (*i)->getName() <<"  De plasmido "<< this->getId() << " De operon " << (*i)->getParent()->getId()<< " Se desactiva en tiempo   " <<time <<'\n';
					(*i)->deactivate();
					(this->aux).push_back(*i);
					//actList.erase(i); AITOR
            }
			if((*i)->getParent()->getPromoter()->check_gates() == 1){
			  (this->aux).push_back(*i);
			}
			
    }
}
void GenPlasmid::checkList1(std::vector<GenCell*> list, float time){
	
	for(std::vector<GenCell*>::iterator i =list.begin();i!=list.end();i++){
	float timeP=(*i)->getTime();
            if( (time >= timeP) && (timeP!=-1) && (*i)->getState()==0 && ((*i)->getParent()->getPromoter()->check_gates() == 1) ){
					  //std::cout<< "Proteina " << (*i)->getName() <<"  De plasmido "<< this->getId() << " De operon " << (*i)->getParent()->getId()<< " Se activaen tiempo   " <<time <<'\n';
                    (*i)->activate();	
					(this->aux).push_back(*i);
					
            }
		if((*i)->getParent()->getPromoter()->check_gates() == 0){
			  (this->aux).push_back(*i);
		}
	}
}

void GenPlasmid::checkNoises(std::vector<GenOperon*> list, float time){
    //float *noises;
    std::array<float,2> noises;
	for(std::vector<GenOperon*>::iterator i =list.begin();i!=list.end();i++){
		if(!((*i)->getNoisef()))//flag de ruido
		{
			noises = (*i)->getTimeNoise();
            //if(time >= noises[0]){ <-- breakpoint
            if(time >= noises.at(0)){
				(*i)->changeFalse();
                (*i)->setNoisef(true);
			}
            //if(time >= noises[1]){ <-- breakpoint
            if(time >= noises.at(1)){
				(*i)->changeTrue();
                (*i)->setNoisef(true);
            }

		}
	}	
}

void GenPlasmid:: macroPlasmid(float time){
	//std::cout <<"TIMEPO DE EXPERIMENTO  " << time << '\n';
	checkList(decList,time);
    remakelist(decList);
	checkList1(actList,time);
    remakelist(actList);
	checkNoises((*operons),time);
	for(std::vector<GenOperon*>::iterator i =operons->begin();i!=operons->end();i++){
			(*i)->update(time);
	}
	
}

std::vector<GenCell*> GenPlasmid::getListAct(){
    return (this->actList);
}

std::vector<GenCell*> GenPlasmid::getListDec(){
    return (this->decList);
}


/*void GenPlasmid::remakelistTA(std::vector<GenCell*> listA,std::vector<GenCell*> listT){
	std::vector<GenCell*> aa;
    bool found = false;
	if(listT.size()!=0){
    for(std::vector<GenCell*>::iterator i =listA.begin();i!=listA.end();i++){
        for(std::vector<GenCell*>::iterator j =listT.begin();j!=listT.end();j++){
            if((*i)==(*j)){
                found = true;
            }
            if(found)
            {
                aa.push_back(*i);
                found = false;
            }
		}

	}
	(this->actList).clear();
	for(std::vector<GenCell*>::iterator i =aa.begin();i!=aa.end();i++){
		(this->actList).push_back(*i);
	}
	aux.clear();
	}
}*/

void GenPlasmid::remakelist(std::vector<GenCell*>& list){
    if(aux.size()!=0){
    for(std::vector<GenCell*>::iterator i =aux.begin();i!=aux.end();i++){
        for(std::vector<GenCell*>::iterator j =list.begin();j!=list.end();){
            if((*i)==(*j)){
                list.erase(j);
            }
            else
            {
                j++;
            }
        }
    }
    aux.clear();
    }
}


/*void GenPlasmid::remakelistTD(std::vector<GenCell*> listA,std::vector<GenCell*> listT){
	std::vector<GenCell*> aa;
    bool found = false;
	if(listT.size()!=0){
	for(std::vector<GenCell*>::iterator i =listA.begin();i!=listA.end();i++){
		for(std::vector<GenCell*>::iterator j =listT.begin();j!=listT.end();j++){
            if((*i)==(*j)){
                found = true;
            }
            if(found)
            {
                aa.push_back(*i);
                found = false;
            }
		}
	}
	(this->decList).clear();
	
	for(std::vector<GenCell*>::iterator i =aa.begin();i!=aa.end();i++){
		(this->decList).push_back(*i);
		}
		aux.clear();
	}
	
}*/

void GenPlasmid::setRNG(erreenege* rng)
{
	for (unsigned int j = 0; j < this->getSize(); j++) {
            this->getOperon(j)->setRNG(rng);
    }
}

void GenPlasmid::setEnvPlasmid(GenPlasmid* env)
{
	this->environment_plasmid = env;
	for (unsigned int i=0; i < (this -> getSize()); i++){
    	this->getOperon(i)->setEnvPlasmid(env);
    }
}
		
GenPlasmid* GenPlasmid::getEnvPlasmid()
{
	return(this->environment_plasmid);
}

void GenPlasmid::setParent(GenListPlasmid* p)
{
	this->parent = p;
}

