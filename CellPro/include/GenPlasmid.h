
#ifndef GENPLASMID_H

#define GENPLASMID_H


#include <vector>
#include <string>
#include <array>
#include <map>
#include "../include/GenCell.h"
#include "../include/GenListPlasmid.h"
#include "../include/GenOperon.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class GenOperon;
class GenListPlasmid;
class erreenege;

class GenPlasmid {
	private:
        std::string id; // ID of plasmid.
        std::string name; // Name of plasmid.
        std::vector<GenOperon*>* operons; // Vector of the operons.
        int cont_ids; // Number of operon that have been signed to this plasmid, although they don't exist at this moment.

        bool type; // Possible type: 0/False (Original) and 1/True (acquire).
        bool free; // Control if plasmid is free.
        GenPlasmid* environment_plasmid; // Reference to the environment plasmid.
        GenListPlasmid* parent; // Reference to its parent;
		//std::vector<GenPlasmid*> actList;//---AITOR
		//std::vector<GenPlasmid*> decList;//---AITOR
		std::vector<GenCell*> actList;//---AITOR Tipo correcto?
		std::vector<GenCell*> decList;//---AITOR
		std::vector<GenCell*> aux;
	public:
		std::string to_string(int );
        /* CONSTRUCTOR */
        GenPlasmid(std::string id, GenListPlasmid* parent, GenPlasmid* environment_plasmid); // New plasmid with ID.
        GenPlasmid(std::string id, std::string name, GenListPlasmid* parent, GenPlasmid* environment_plasmid);// New plasmid with ID and name.
        GenPlasmid(std::string id); // New plasmid with ID.
        GenPlasmid(std::string id, std::string name);// New plasmid with ID and name.
        /* ATRIBUTES */
        std::string getId(); // Get plasmid ID.
        std::string getName(); // Get plasmid name.
        unsigned int getSize(); // Get plasmid size.
        int getContIds(); // Get the number of operon that have been signed to this plasmid, although they don't exist at this moment.
        bool getType(); // Get the type of the plasmid.
        GenOperon* getOperonById(std::string id); // Get the operon by Id.
        std::vector<GenOperon*>* getOperonByName(std::string name); // Get a list of the operon with that name.
        GenOperon* getOperon(unsigned int index); // Get the operon by index.
        GenOperon* getOperon(GenOperon*  operon); // Get the operon by index.
        std::vector<GenOperon*>* getOperons(); // Get a vector with all operons.
        bool isFree(); // Check if the plasmid is free.
        int getPos(GenOperon* op); // Get operon position.
        /* CHECK GENS AND OPERONS */
        std::map<std::string,int> getGensReleased(); // Returns a map with the released gens.
        std::map<std::string,int> getGensBlocked(); // Returns a map with the blocked gens.
        std::map<std::string,int> getGensActive(); // Returns a map with the active gens.
        std::map<std::string,int> getGensDeactive(); // Returns a map with the deactivedgens.
        std::map<std::string,int> getGensGuide(); // Returns a map with the guided gens.
        std::map<std::string,int> isGenExist(std::string name); // Returns a map with the operon IDs where the gen exists.
        std::map<std::string,int> isGenReleased(std::string name); // Returns a map with the operon IDs where the gen is released.
        std::map<std::string,int> isGenBlocked(std::string name); // Returns a map with the operon IDs where the gen is blocked.
        std::map<std::string,int> isGenActive(std::string name); // Returns a map with the operon IDs where the gen is active.
        std::map<std::string,int> isGenDeactive(std::string name); // Returns a map with the operon IDs where the gen is deactive.
        std::map<std::string,int> isGenGuide(std::string name); // Returns a map with the operon IDs where the gen is guide.
        bool isOperonExistById(std::string operon_id); // Check if the operon exist searched by ID.
        bool isOperonExistByName(std::string operon_name); // Check if the operon exist searched by name.
        bool isOperonExist(unsigned int index); // Check if the operon exist searched by index.
        bool isGenExistByOpId(std::string operon_id, std::string name); // Check if the gen exists in a specific operon searched by operon's ID.
        bool isGenBlockedByOpId(std::string operon_id, std::string name); // Check if the gen is blocked in a specific operon searched by operon's ID.
        bool isGenReleasedByOpId(std::string operon_id, std::string name); // Check if the gen is released in a specific operon searched by operon's ID.
        bool isGenActiveByOpId(std::string operon_id, std::string name); // Check if the gen is active in a specific operon searched by operon's ID.
        bool isGenDeactiveByOpId(std::string operon_id, std::string name); // Check if the gen is deactive in a specific operon searched by operon's ID.
        bool isGenGuideByOpId(std::string operon_id, std::string name); // Check if the gen is guide in a specific operon searched by operon's ID.
        bool isGenExistByOpName(std::string operon_name, std::string name); // Check if the gen exists in a specific operon searched by operon's name.
        bool isGenBlockedByOpName(std::string operon_name, std::string name); // Check if the gen is blocked in a specific operon searched by operon's name.
        bool isGenReleasedByOpName(std::string operon_name, std::string name); // Check if the gen is released in a specific operon searched by operon's name.
        bool isGenActiveByOpName(std::string operon_name, std::string name); // Check if the gen is active in a specific operon searched by operon's name.
        bool isGenDeactiveByOpName(std::string operon_name, std::string name); // Check if the gen is deactive in a specific operon searched by operon's name.
        bool isGenGuideByOpName(std::string operon_name, std::string name); // Check if the gen is guide in a specific operon searched by operon's name.
        bool isGenExist(unsigned int operon_index, std::string name); // Check if the gen exists in a specific operon searched by operon's index.
        bool isGenBlocked(unsigned int operon_index, std::string name); // Check if the gen is blocked in a specific operon searched by operon's index.
        bool isGenReleased(unsigned int operon_index, std::string name); // Check if the gen is released in a specific operon searched by operon's index.
        bool isGenActive(unsigned int operon_index, std::string name); // Check if the gen is active in a specific operon searched by operon's index.
        bool isGenDeactive(unsigned int operon_index, std::string name); // Check if the gen is deactive in a specific operon searched by operon's index.
        bool isGenGuide(unsigned int operon_index, std::string name); // Check if the gen is guide in a specific operon searched by operon's index.
        std::map<std::string,int> isGenExist(std::map<std::string,int> sGens); // Returns a map with the operon IDs where the gens exist.
        std::map<std::string,int> isGenBlocked(std::map<std::string,int> sGens); // Returns a map with the operon IDs where the gens are blocked.
        std::map<std::string,int> isGenReleased(std::map<std::string,int> sGens); // Returns a map with the operon IDs where the gens are released.
        std::map<std::string,int> isGenActive(std::map<std::string,int> sGens); // Returns a map with the operon IDs where the gens are active.
        std::map<std::string,int> isGenDeactive(std::map<std::string,int> sGens); // Returns a map with the operon IDs where the gens are deactive.
        std::map<std::string,int> isGenGuide(std::map<std::string,int> sGens); // Returns a map with the operon IDs where the gens are guide.
        bool isGenExistByOpId(std::string operon_id, std::map<std::string,int> sGens); // Check if the gens exist in a specific operon searched by operon's ID.
        bool isGenBlockedByOpId(std::string operon_id, std::map<std::string,int> sGens); // Check if the gens are blocked in a specific operon searched by operon's ID.
        bool isGenReleasedByOpId(std::string operon_id, std::map<std::string,int> sGens); // Check if the gens are released in a specific operon searched by operon's ID.
        bool isGenActiveByOpId(std::string operon_id, std::map<std::string,int> sGens); // Check if the gens are active in a specific operon searched by operon's ID.
        bool isGenDeactiveByOpId(std::string operon_id, std::map<std::string,int> sGens); // Check if the gens are deactive in a specific operon searched by operon's ID.
        bool isGenGuideByOpId(std::string operon_id, std::map<std::string,int> sGens); // Check if the gens are guide in a specific operon searched by operon's ID.
        bool isGenExistByOpName(std::string operon_name, std::map<std::string,int> sGens); // Check if the gens exist in a specific operon searched by operon's name.
        bool isGenBlockedByOpName(std::string operon_name, std::map<std::string,int> sGens); // Check if the gens are blocked in a specific operon searched by operon's name.
        bool isGenReleasedByOpName(std::string operon_name, std::map<std::string,int> sGens); // Check if the gens are released in a specific operon searched by operon's name.
        bool isGenActiveByOpName(std::string operon_name, std::map<std::string,int> sGens); // Check if the gens are active in a specific operon searched by operon's name.
        bool isGenDeactiveByOpName(std::string operon_name, std::map<std::string,int> sGens); // Check if the gens are deactive in a specific operon searched by operon's name.
        bool isGenGuideByOpName(std::string operon_name, std::map<std::string,int> sGens); // Check if the gens are guide in a specific operon searched by operon's name.
        bool isGenExist(unsigned int operon_index, std::map<std::string,int> sGens); // Check if the gens exist in a specific operon searched by operon's index.
        bool isGenBlocked(unsigned int operon_index, std::map<std::string,int> sGens); // Check if the gens are blocked in a specific operon searched by operon's index.
        bool isGenReleased(unsigned int operon_index, std::map<std::string,int> sGens); // Check if the gens are released in a specific operon searched by operon's index.
        bool isGenActive(unsigned int operon_index, std::map<std::string,int> sGens); // Check if the gens are active in a specific operon searched by operon's index.
        bool isGenDeactive(unsigned int operon_index, std::map<std::string,int> sGens); // Check if the gens are deactive in a specific operon searched by operon's index.
        bool isGenGuide(unsigned int operon_index, std::map<std::string,int> sGens); // Check if the gens are guide in a specific operon searched by operon's index.
        /*bool isGenExist(std::map<std::string,int> sGens); // Check if the gens exist in a specific operon searched by operon's index.
        bool isGenBlocked(std::map<std::string,int> sGens); // Check if the gens are blocked in a specific operon searched by operon's index.
        bool isGenReleased(std::map<std::string,int> sGens); // Check if the gens are released in a specific operon searched by operon's index.
        bool isGenActive(std::map<std::string,int> sGens); // Check if the gens are active in a specific operon searched by operon's index.
        bool isGenDeactive(std::map<std::string,int> sGens); // Check if the gens are deactive in a specific operon searched by operon's index.
        bool isGenGuide(std::map<std::string,int> sGens); // Check if the gens are guide in a specific operon searched by operon's index.*/

        bool comparePlasmid(GenPlasmid* plasmid); // Checks if the plasmids are the same.
        /* MANAGE PLASMIDS */
        void setType(bool type); // Set the type of the plasmid.
        void setFree(); // Set plasmid to free.
        void insertOperon(std::map<std::string,int> initGens, int type, GenPromoter *prom); // Insert operon with maps of gens.
        void insertOperon(GenOperon* operon); // Insert operon with initial operon.
        void insertOperonConj(GenOperon* operon); // Insert operon with initial operon.
        void insertOperon(std::vector<GenOperon*>* initOperons); // Insert operon with initial operons.
        void insertOperonConj(std::vector<GenOperon*>* initOperons); // Insert operon with initial operons.
        void insertOperon(std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom); //Insert operon with maps of gens and map of active gens.
        void insertOperon(std::string name, std::map<std::string,int> initGens, int type, GenPromoter *prom);// Insert operon with name and maps of gens.
        void insertOperon(std::string name, GenOperon* operon);// Insert operon with name and initial operon.
        void insertOperon(std::string name, std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom);//  Insert operon with name, maps of gens and map of active gens.
        void insertOperon(unsigned int index, std::map<std::string,int> initGens, int type, GenPromoter *prom); // Insert operon with maps of gens into an specific place.
        void insertOperon(unsigned int index, GenOperon* operon); // Insert operon with initial operon into an specific place.
        void insertOperon(unsigned int index, std::vector<GenOperon*>* initOperons); // Insert operon with initial operons into an specific place.
        void insertOperon(unsigned int index, std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom); //Insert operon with maps of gens and map of active gens into an specific place.
        void insertOperon(unsigned int index, std::string name, std::map<std::string,int> initGens, int type, GenPromoter *prom);// Insert operon with name and maps of gens into an specific place.
        void insertOperon(unsigned int index, std::string name, GenOperon* operon);// Insert operon with name and initial operon into an specific place.
        void insertOperon(unsigned int index, std::string name, std::map<std::string,int> initGens, std::map<std::string,int> activeGens, int type, GenPromoter *prom);//  Insert operon with name, maps of gens and map of active gens into an specific place.
        void eraseOperon(unsigned int index); // Erase an operon from the operons list by the index.
        void eraseOperonById(std::string id);  // Erase an operon from the operons list by the operon's ID.
        void eraseOperonByName(std::string name); // Erase all the operons from the operons list with that name.
        void setTargetGen(std::string name, std::string target); // Set target of the gen in all the operons.
        void setDegTimeGen(std::string name,double time); // Set degradation time of the gen in all the operons.
        void setActTimeGen(std::string name,double time); // Set activation time of the gen in all the operons.
        void blockGen(std::string name); // Block the gen in all the operons of the plasmid.
        void releaseGen(std::string name); // Release the gen in all the operons of the plasmid.
        void activateGen(std::string name); // Activate the gen in all the operons of the plasmid.
        void deactivateGen(std::string name); // Deactivate the gen in all the operons of the plasmid.
        void eraseGen(std::string name); // Erase the gen in all the operons of the plasmid.
        void mutateGen(std::string name, int probability); // Mutate the gen in all the operons of the plasmid with a probability.
        void blockGenByOpId(std::string operon_id, std::string name); // Block the gen in a specific operon of the plasmid by operon's ID.
        void releaseGenByOpId(std::string operon_id, std::string name); // Release the gen in a specific operon of the plasmid by operon's ID.
        void activateGenByOpId(std::string operon_id, std::string name); // Activate the gen in a specific operon of the plasmid by operon's ID.
        void deactivateGenByOpId(std::string operon_id,std::string name); // Deactivate the gen in a specific operon of the plasmid by operon's ID.
        void eraseGenByOpId(std::string operon_id,std::string name); // Erase the gen in a specific operon of the plasmid by operon's ID.
        void mutateGenByOpId(std::string operon_id,std::string name, int probability); // Mutate the gen in a specific operon of the plasmid by operon's ID with a probability.
        void blockGenByOpName(std::string operon_name, std::string name);// Block the gen in all the operons of the plasmid with that name.
        void releaseGenByOpName(std::string operon_name, std::string name);// Release the gen in all the operons of the plasmid with that name.
        void activateGenByOpName(std::string operon_name, std::string name);// Activate the gen in all the operons of the plasmid with that name.
        void deactivateGenByOpName(std::string operon_name,std::string name);// Deactivate the gen in all the operons of the plasmid with that name.
        void eraseGenByOpName(std::string operon_name,std::string name);// Erase the gen in all the operons of the plasmid with that name.
        void mutateGenByOpName(std::string operon_name,std::string name, int probability);// Mutate the gen in all the operons of the plasmid with that name with a probability.
        void blockGen(unsigned int operon_index, std::string name);// Block the gen in a specific operon of the plasmid by operon's index.
        void releaseGen(unsigned int operon_index, std::string name);// Release the gen in a specific operon of the plasmid by operon's index.
        void activateGen(unsigned int operon_index, std::string name);// Activate the gen in a specific operon of the plasmid by operon's index.
        void deactivateGen(unsigned int operon_index,std::string name);// Deactivate the gen in a specific operon of the plasmid by operon's index.
        void eraseGen(unsigned int operon_index,std::string name);// Erase the gen in a specific operon of the plasmid by operon's index.
        void mutateGen(unsigned int operon_index,std::string name, int probability);// Mutate the gen in a specific operon of the plasmid by operon's index with a probability.
        void blockGens(std::map<std::string,int> initGens); // Activate those gens in all the operons of the plasmid.
        void releaseGens(std::map<std::string,int> initGens); // Activate those gens in all the operons of the plasmid.
        void activateGens(std::map<std::string,int> initGens); // Activate those gens in all the operons of the plasmid.
        void deactivateGens(std::map<std::string,int> initGens); // Deactivate those gens in all the operons of the plasmid.
        void eraseGens(std::map<std::string,int> initGens); // Erase those gens in all the operons of the plasmid.
        void blockGensByOpId(std::string operon_id, std::map<std::string,int> initGens); // Block those gens in a specific operon of the plasmid by operon's ID.
        void releaseGensByOpId(std::string operon_id, std::map<std::string,int> initGens); // Release those gens in a specific operon of the plasmid by operon's ID.
        void activateGensByOpId(std::string operon_id, std::map<std::string,int> initGens); // Activate those gens in a specific operon of the plasmid by operon's ID.
        void deactivateGensByOpId(std::string operon_id, std::map<std::string,int> initGens); // Deactivate those gens in a specific operon of the plasmid by operon's ID.
        void eraseGensByOpId(std::string operon_id, std::map<std::string,int> initGens); // Erase those gens in a specific operon of the plasmid by operon's ID.
        void blockGensByOpName(std::string operon_name, std::map<std::string,int> initGens); // Block those gens in a specific operon of the plasmid by operon's name.
        void releaseGensByOpName(std::string operon_name, std::map<std::string,int> initGens); // Release those gens in a specific operon of the plasmid by operon's name.
        void activateGensByOpName(std::string operon_name, std::map<std::string,int> initGens); // Activate those gens in a specific operon of the plasmid by operon's name.
        void deactivateGensByOpName(std::string operon_name, std::map<std::string,int> initGens); // Deactivate those gens in a specific operon of the plasmid by operon's name.
        void eraseGensByOpName(std::string operon_name, std::map<std::string,int> initGens); // Erase those gens in a specific operon of the plasmid by operon's name.
        void blockGens(unsigned int operon_index, std::map<std::string,int> initGens); // Block those gens in a specific operon of the plasmid by operon's index.
        void releaseGens(unsigned int operon_index, std::map<std::string,int> initGens); // Release those gens in a specific operon of the plasmid by operon's index.
        void activateGens(unsigned int operon_index, std::map<std::string,int> initGens); // Activate those gens in a specific operon of the plasmid by operon's index.
        void deactivateGens(unsigned int operon_index, std::map<std::string,int> initGens); // Deactivate those gens in a specific operon of the plasmid by operon's index.
        void eraseGens(unsigned int operon_index, std::map<std::string,int> initGens); // Erase those gens in a specific operon of the plasmid by operon's index.
        void erase(); // Erase this plasmid.
        void isEmptyErase(); // Erase this plasmid.
		void insertDecList(std::vector<GenCell*> cellDeact, float);//---AITOR	
		void insertActList(std::vector<GenCell*> cellAct, float);//---AITOR
		void checkList(std::vector<GenCell*> list, float time);//---AITOR
		void checkList1(std::vector<GenCell*> list, float time);
		void checkNoises(std::vector<GenOperon*> list, float time);//---AITOR Tipo?
		void macroPlasmid(float time);//---AITOR
		GenListPlasmid* getParent();//---Aitor
        std::vector<GenCell*> getListAct();
        std::vector<GenCell*> getListDec();
        void setActList(std::vector<GenCell*>);
        void setDecList(std::vector<GenCell*>);
		void eraseListAct(int p);
		void eraseListDeact(int p);
		void remakelistTA(std::vector<GenCell*>,std::vector<GenCell*> );
		void remakelistTD(std::vector<GenCell*>,std::vector<GenCell*> );
        void remakelist(std::vector<GenCell*>&);
		void setRNG(erreenege*);
		void setEnvPlasmid(GenPlasmid* env);
		GenPlasmid* getEnvPlasmid();
		void setParent(GenListPlasmid*);
};


#endif
