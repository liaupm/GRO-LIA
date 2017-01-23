#ifndef GENCELL_H

#define GENCELL_H

#include <string>
#include <random>
#include <iostream>

// States
#define ACTIVE 1
#define DEACTIVE 0

// Types
#define PROTEIN 0
#define RNA 1
#define DNA 2

class GenPlasmid;
class GenOperon;
class erreenege;
class GenCell{
	private:
        std::string  name; // name of the gen.
        int state; // state values: 0 (deactive) and 1 (active).
        int type; // type values: 0 (protein), 1 (rna), 2 (dna).
        bool block; // Block control for riboswitch.
        std::string  target; // Target of the gen.
        double time_act; // Time activation.
        double time_deg; // Time degradation
        double sigmaAct;//---AITOR
	    double sigmaDeg;//---AITOR
        //double time_act_error; // Time activation error.
        //double time_deg_error; // Time degradation error
        bool free; // Control for free gens.
        GenPlasmid* environment_plasmid; // Reference to the environment plasmid.
        GenOperon* parent; // Reference to its parent.
		float time;//---AITOR
	    
	    erreenege *rng;
	   
	public:
        /* CONSTRUCTOR */
        /*GenCell(std::string name, int state, int type, GenOperon* parent, GenPlasmid* environment_plasmid, int); // New gen with name and initial state.
        GenCell(std::string name, int type, GenOperon* parent, GenPlasmid* environment_plasmid, int); // New gen with name.
        GenCell(std::string name, int state, std::string target, int type, GenOperon* parent, GenPlasmid* environment_plasmid, int); // New gen with name and initial state.
        GenCell(std::string name, std::string target, int type, GenOperon* parent, GenPlasmid* environment_plasmid, int); // New gen with name.
        GenCell(std::string name, int type, int); // New gen with type.*/
        GenCell(std::string name, int state, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact, float time = -1); // New gen with name and initial state.
        GenCell(std::string name, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact); // New gen with name.
        GenCell(std::string name, int state, std::string target, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact,float time = -1); // New gen with name and initial state.
        GenCell(std::string name, std::string target, int type, GenOperon* parent, GenPlasmid* environment_plasmid,float time_act,float time_deact); // New gen with name.
        GenCell(std::string name, int type, float time_act, float time_deact); // New gen with type.
        GenCell(std::string name, int state, int type, float time_act,float time_deact); // New gen with name and initial state.
        GenCell(std::string name, int state, std::string target, int type, float time_act,float time_deact); // New gen with name and initial state.
        GenCell(std::string name, std::string target, int type,float time_act,float time_deact); // New gen with name.
		/* ATRIBUTES */
        std::string getName(); // Get gen name.
        std::string getTarget(); // Get gen name.
        int getState(); // Get gen state.
        int getType(); // Get gen type.
        GenOperon* getParent();
        double getTimeAct(); // Get gen time activation.
        double getTimeDeg(); // Get gen time degradation.
        //double getTimeActError(); // Get gen time activation error.
        //double getTimeDegError(); // Get gen time degradation error.
        double getSigmaAct ();
        double getSigmaDeg ();
        bool isFree(); // Check if gen is free.
        bool isBlocked(); // Check if gen is blocked.
        bool existGen(); // Check if gen exist.
        bool guideGen(); // Check if gen is guide.
        bool compareCell(GenCell* cell); // Checks if this cell is equal than the one given.
        /* MANAGE GEN */
        void setTimeAct(double time); // Set gen time activation.
        void setTimeDeg(double time); // Set gen time degradation.
        //void setTimeActError(double time); // Set gen time activation error.
        //void setTimeDegError(double time); // Set gen time degradation error.
        void setSigmaAct (double time);
        void setSigmaDeg (double time);
        void setBlock(bool block); // Set gen block.
        void setTarget(std::string target); // Set gen target.
        void activate(); // Activate the gen.
        void deactivate(); // Deactivate the gen.
        void erase(); // Erase the gen.
        void setFree(); // Set gen to free.
		void changeState();//---AITOR
		float getTime();//---AITOR
		void setTime(float currentTime);//---AITOR
		void setRNG(erreenege*); //---MARTIN
		void setEnvPlasmid(GenPlasmid* env);
		GenPlasmid* getEnvPlasmid();
		void setParent(GenOperon*);
};

#endif
