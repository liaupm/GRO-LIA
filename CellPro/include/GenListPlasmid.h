#ifndef GENLISTPLASMID_H

#define GENLISTPLASMID_H

#include "../include/GenPlasmid.h"
#include "../include/GenOperon.h"
#include "../include/GenCell.h"
#include "../include/GenPromoter.h"
#include "../include/erreenege.h"
#include <vector>
#include <string>
#include <map>


using namespace std;
class GenPlasmid;
class erreenege;
class GenListPlasmid {
	private:
        std::string id; // Cell id.
        std::vector<GenPlasmid*>* plasmids; // Vector of the plasmids.
        int cont_ids;// Number of plasmids that have been signed to this list of plasmids, although they don't exist at this moment.
        std::vector<GenPlasmid*>* avoidPlasmids; // Plasmids avoid insert in the same list than this plasmid.
        GenPlasmid* environment_plasmid; // Reference to the environment plasmid.
	public:
		
        /* CONSTRUCTOR */
        GenListPlasmid(std::string id, GenPlasmid* environment_plasmid); // Inicial empty list of plasmids with its environment plasmid.
        GenListPlasmid(std::string id); // Inicial empty list of plasmids.
        /* ATRIBUTES */
        std::string getId(); // Get ID of the cell.
        unsigned int getSize(); // Get size of the list of plasmids.
        int getContIds(); // //Get the number of plasmids that have been signed to this list of plasmids, although they don't exist at this moment.
        GenPlasmid* getPlasmid(GenPlasmid*  plasmid); // Get the plasmid by another plasmid.
        GenPlasmid* getPlasmid(unsigned int index); // Get plasmids by index.
        GenPlasmid* getPlasmidById(std::string id); // Get plasmid by ID.
        std::vector<GenPlasmid*>* getPlasmidByName(std::string name); // Get vector of plasmids with that name.
        std::vector<GenPlasmid*>* getPlasmids(); // Get all the plasmids.
        int getPos(GenPlasmid* plasmid); // Get size of the list of plasmids.
        /* CHECK PLASMIDS, OPERONS AND GENS */
        std::map<std::string,int> getGensBlocked(); // Returns a map with the blocked gens.
        std::map<std::string,int> getGensReleased(); // Returns a map with the released gens.
        std::map<std::string,int> getGensActive(); // Returns a map with the active gens.
        std::map<std::string,int> getGensDeactive(); // Returns a map with the deactived gens.
        std::map<std::string,int> getGensGuide(); // Returns a map with the guided gens.
        std::map<std::string,int> isGenExist(std::string name); // Returns a map with the IDs of the plasmids where the gen exist.
        std::map<std::string,int> isGenBlocked(std::string name); // Returns a map with the IDs of the plasmids where the gen is blocked.
        std::map<std::string,int> isGenReleased(std::string name); // Returns a map with the IDs of the plasmids where the gen is released.
        std::map<std::string,int> isGenActive(std::string name); // Returns a map with the IDs of the plasmids where the gen is active.
        std::map<std::string,int> isGenDeactive(std::string name); // Returns a map with the IDs of the plasmids where the gen is active.
        std::map<std::string,int> isGenExist(std::map<std::string,int> sGens); // Returns a map with the IDs of the plasmids where the gen exist.
        std::map<std::string,int> isGenBlocked(std::map<std::string,int> sGens); // Returns a map with the IDs of the plasmids where the gen is blocked.
        std::map<std::string,int> isGenReleased(std::map<std::string,int> sGens); // Returns a map with the IDs of the plasmids where the gen is released.
        std::map<std::string,int> isGenActive(std::map<std::string,int> sGens); // Returns a map with the IDs of the plasmids where the gen is active.
        std::map<std::string,int> isGenDeactive(std::map<std::string,int> sGens); // Returns a map with the IDs of the plasmids where the gen is active.
        std::vector<GenPlasmid*>* getAvoidPlasmids(); // Get a vector with all the avoid plasmids.
        bool isAvoidPlasmid(GenPlasmid* plasmid); // Checks if the plasmid is in the avoid list.
        void insertAvoidPlasmid(GenPlasmid* plasmid); // Insert plasmid in the avoid list.
        void eraseAvoidPlasmid(GenPlasmid* plasmid); // Erase plasmid of the avoid list.
         /* MANAGE LIST PLASMIDS */
        void setPlasmidOriginal(); // Set all plasmids type to original.
        void insertPlasmid(GenPlasmid* genPlasmid); // Insert plasmid into the list.
        void insertPlasmid(std::string name, GenPlasmid* genPlasmid); // Insert plasmid into the list with a name.
        void insertPlasmid(std::vector<GenPlasmid*>* plasmidList); // Insert list of plasmids into the list.
        void insertPlasmidConj(GenPlasmid* genPlasmid); // Insert plasmid into the list.
        void erasePlasmidById(std::string id); // Erase a plasmid from the list by ID.
        void erasePlasmidByName(std::string name); // Erase all the plasmids from the lsit with that name.
        void erasePlasmid(unsigned int index); // Erase plasmid from the list by index.
        void setTargetGen(std::string name, std::string target); // Set target of the gen in all the plasmids.
        void setDegTimeGen(std::string name,double time); // Set degradation time of the gen in all the plasmids.
        void setActTimeGen(std::string name,double time); // Set activation time of the gen in all the plasmids.
        void blockGen(std::string name); // Block the gen in all plasmids.
        void releaseGen(std::string name); // Release the gen in all plasmids.
        void activateGen(std::string name); // Active the gen in all plasmids.
        void deactivateGen(std::string name); // Deactive the gen in all plasmids.
        void eraseGen(std::string name); // Erase the gen in all plasmids.
        void mutateGen(std::string name, int probability); // Mutate the gen in all plasmids with a probability.
        void blockGenById(std::string plasmid_id, std::string name);  // Block the gen in a specific plasmid by ID.
        void releaseGenById(std::string plasmid_id, std::string name);  // Release the gen in a specific plasmid by ID.
        void activateGenById(std::string plasmid_id, std::string name);  // Active the gen in a specific plasmid by ID.
        void deactivateGenById(std::string plasmid_id,std::string name);// Deactive the gen in a specific plasmid by ID.
        void eraseGenById(std::string plasmid_id,std::string name);// Erase the gen in a specific plasmid by ID.
        void mutateGenById(std::string plasmid_id,std::string name, int probability);// Mutate the gen in a specific plasmid by ID with a probability.
        void blockGenByName(std::string plasmid_name, std::string name);// Block the gen in all plasmids with that name.
        void releaseGenByName(std::string plasmid_name, std::string name);// Release the gen in all plasmids with that name.
        void activateGenByName(std::string plasmid_name, std::string name);// Active the gen in all plasmids with that name.
        void deactivateGenByName(std::string plasmid_name,std::string name);// Deactive the gen in all plasmids with that name.
        void eraseGenByName(std::string plasmid_name,std::string name);// Erase the gen in all plasmids with that name.
        void mutateGenByName(std::string plasmid_name,std::string name, int probability);// Mutate the gen in all plasmids with that name with a probability.
        void blockGen(unsigned int plasmid_index, std::string name);// Block the gen in a specific plasmid by index.
        void releaseGen(unsigned int plasmid_index, std::string name);// Release the gen in a specific plasmid by index.
        void activateGen(unsigned int plasmid_index, std::string name);// Active the gen in a specific plasmid by index.
        void deactivateGen(unsigned int plasmid_index,std::string name);// Deactive the gen in a specific plasmid by index.
        void eraseGen(unsigned int plasmid_index,std::string name);// Erase the gen in a specific plasmid by index.
        void mutateGen(unsigned int plasmid_index,std::string name, int probability);// Mutate the gen in a specific plasmid by index with a probability.
        void blockGens(std::map<std::string,int> initGens);// Block those gens in all plasmids.
        void releaseGens(std::map<std::string,int> initGens);// Release those gens in all plasmids.
        void activeGens(std::map<std::string,int> initGens);// Active those gens in all plasmids.
        void deactiveGens(std::map<std::string,int> initGens);// Deactive those gens in all plasmids.
        void eraseGens(std::map<std::string,int> initGens);// Erase those gens in all plasmids.
        void blockGensById(std::string plasmid_id, std::map<std::string,int> initGens);// Block those gens in a specific plasmid by ID.
        void releaseGensById(std::string plasmid_id, std::map<std::string,int> initGens);// Release those gens in a specific plasmid by ID.
        void activeGensById(std::string plasmid_id, std::map<std::string,int> initGens);// Active those gens in a specific plasmid by ID.
        void deactiveGensById(std::string plasmid_id, std::map<std::string,int> initGens);// Deactive those gens in a specific plasmid by ID.
        void eraseGensById(std::string plasmid_id, std::map<std::string,int> initGens);// Erase those gens in a specific plasmid by ID.
        void blockGensByName(std::string plasmid_name, std::map<std::string,int> initGens);// Block those gens in all plasmids with that name.
        void releaseGensByName(std::string plasmid_name, std::map<std::string,int> initGens);// Release those gens in all plasmids with that name.
        void activeGensByName(std::string plasmid_name, std::map<std::string,int> initGens);// Active those gens in all plasmids with that name.
        void deactiveGensByName(std::string plasmid_name, std::map<std::string,int> initGens);// Deactive those gens in all plasmids with that name.
        void eraseGensByName(std::string plasmid_name, std::map<std::string,int> initGens);// Erase those gens in all plasmids with that name.
        void blockGens(unsigned int plasmid_index, std::map<std::string,int> initGens);// Block those gens in a specific plasmid by index.
        void releaseGens(unsigned int plasmid_index, std::map<std::string,int> initGens);// Release those gens in a specific plasmid by index.
        void activeGens(unsigned int plasmid_index, std::map<std::string,int> initGens);// Active those gens in a specific plasmid by index.
        void deactiveGens(unsigned int plasmid_index, std::map<std::string,int> initGens);// Deactive those gens in a specific plasmid by index.
        void eraseGens(unsigned int plasmid_index, std::map<std::string,int> initGens);// Erase those gens in a specific plasmid by index.
        void erase(); // Erase all the list.
		void macroPlasmid(float currentTime);
		void setRNG(erreenege* rng);
		void setEnvPlasmid(GenPlasmid* env);
		GenPlasmid* getEnvPlasmid();
		
};

#endif
