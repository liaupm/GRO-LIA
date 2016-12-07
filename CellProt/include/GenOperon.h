#ifndef GENOPERON_H

#define GENOPERON_H

#include <vector>
#include <string>
#include <map>
#include <iostream>


using namespace std;

class GenPlasmid;
class GenCell;
class GenPromoter;

class GenOperon {
	private:
        std::string id; // ID of the operon.
        std::string name; // Name of the operon.
        std::vector<GenCell*>* gens; // Vector of the gens.
        GenOperon* last; // Previuos step of the operon.
        bool free; // control free operons.
        GenPlasmid* enviroment_plasmid; // Reference to the enviroment plasmid.
        GenPlasmid* parent; // Reference to the parent.
		float tNoise1;//---AITOR
		float tNoise2;//---AITOR
		float onTOoff;//---AITOR
		float offTOon;//---AITOR
		bool noise;//---AITOR
		GenPromoter* promoter;//MARTIN
		int lGate;
	public:
        /* CONSTRUCTOR */
        GenOperon(std::string id, GenPlasmid* parent, GenPlasmid* enviroment_plasmid,GenPromoter* promoter); // New operon with ID and map of thte gens.
        GenOperon(std::string id, std::string name, GenPlasmid* parent, GenPlasmid* enviroment_plasmid,GenPromoter* promoter); // New operon with ID, name and map of gens.
        GenOperon(std::string id,GenPromoter* promoter); // New operon with ID and map of thte gens.
        GenOperon(std::string id, std::string name,GenPromoter* promoter); // New operon with ID, name and map of gens.
        /* ATRIBUTES */
        std::string getId(); // Get the id of operon.
        std::string getName(); // Get the name of operon.
        unsigned int getSize(); // Get the size of operon.
        GenCell* getGen(std::string name); // Get the gen by name.
        GenCell* getGen(unsigned int index); // Get the gen by index.
        GenCell* getGen(GenCell* gen); // Get the gen by name.
		GenPromoter* getPromoter();  //Get the id of promoter. ---->AITOR Cambio el tipo
		GenPlasmid* getParent();    //Get the parent of operon ----->AITOR
        std::vector<GenCell*>* getGens(); // Returns a vector with all the gens.
        GenOperon* getLast(); // Get the previous step of the operon.
        bool isFree(); // Check if the operon is free.
        int getPos( GenCell* gen); // Returns the position of the gen.
        /* CHECKS */
        std::map<std::string,int>  getGensReleased(); // Returns a map with the names of the released gens in the operon.
        std::map<std::string,int>  getGensBlocked(); // Returns a map with the names of the bloked gens in the operon.
        std::map<std::string,int>  getGensActive(); // Returns a map with the names of the active gens in the operon.
        std::map<std::string,int>  getGensDeactive(); // Returns a map with the names of the deactive gens in the operon.
        std::map<std::string,int>  getGensGuide(); // Returns a map with the names of the guide gens in the operon.
		/*---------------*/
        bool isGenExist(std::string name); // Check if the gen exists.
        bool isGenBlocked(std::string name); // Check if the gen is blocked.
        bool isGenGuide(std::string name); // Check if the gen is guide.
        bool isGenActive(std::string name); // Check if the gen is active.
        bool isGenDeactive(std::string name); // Check if the gen is deactive.
        bool isGenExist(std::map<std::string,int> sGens);// Check if those gens exist.
        bool isGenGuide(std::map<std::string,int> sGens);// Check if those gens are guide.
        bool isGenBlocked(std::map<std::string,int> sGens);// Check if those gens are blocked.
        bool isGenActive(std::map<std::string,int> sGens);// Check if those gens are active.
        bool isGenDeactive(std::map<std::string,int> sGens);// Check if those gens are deactive.
        bool compareOperon(GenOperon* operon); // Compare this operon with the operon given.
        /* MANAGE OPERON */
        void insertGens(std::vector<GenCell*> initGens); // Insert gen into the operon.
        void insertGens(std::map<std::string,int> initGens, map<string,int> activeGens, int type,float time_act,float time_deg); //  Insert gen into the operon.
        void insertGens(std::map<std::string,int> initGens, int type,float time_act,float time_deg); //  Insert gen into the operon.
        void insertGens(std::vector<GenCell*>* initGens); //  Insert gens into the operon.
        void insertGens(GenCell* initGens); //  Insert gen into the operon.
        void setTargetGen(std::string name, std::string target); // Set target to the gen.
        void setDegTimeGen(std::string name,double time); // Set degradation time of the gen.
        void setActTimeGen(std::string name,double time); // Set activation time of the gen.
        void setDegTimeErrorGen(std::string name,double time); // Set degradation time error of the gen.
        void setActTimeErrorGen(std::string name,double time); // Set activation time error of the gen.
        void activateGen(std::string name); // Activate the gen.
        void deactivateGen(std::string name); // Deactivate the gen.
        void eraseGen(std::string name); // Erase the gen.
        void blockGen(std::string name); // Block the gen.
        void releaseGen(std::string name); // Realise the gen.
        void eraseGen(int index); // Erase the gen.
        void mutateGen(std::string name, int probability); // Mutation of gen with a probability.
        void activateGens(std::map<std::string,int> initGens); // Activate those gens.
        void deactivateGens(std::map<std::string,int> initGens); // Deactivate those gens.
        void eraseGens(std::map<std::string,int> initGens); // Erase those gens.
        void blockGens(std::map<std::string,int> initGens); // Block those gens.
        void releaseGens(std::map<std::string,int> initGens); // Release those gens.
        void setLast(GenOperon* operon); // Save previuos step of the operon.
        void setFree(); // Set operon to free.
        void erase(); // Erase this operon.
        void isEmptyErase(); // If this operon is empty, erase it.
		void sendToDeact(std::vector<GenCell *> cellDeact, float);//inserts the gen into a plamid listActivation ---AITOR
		void sendToActivation(std::vector<GenCell *> cellAtc, float);//inserts the gen into a plamid listDeactivation ---AITOR
		void update(float);//---AITOR
		void changeTrue();//---AITOR
		void changeFalse();//---AITOR
		float calculateNoise(float);//---AITOR
		void setNoise(float);//---AITOR
		float* getTimeNoise();//---AITOR
		float getonTOoff();//---MARTIN
		float getoffTOon();//---MARTIN
		bool getNoisef();//---AITOR
		void setPromoter(GenPromoter* );
		int getlGate();
};

#endif
