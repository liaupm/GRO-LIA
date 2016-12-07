#ifndef GENPROMOTER_H

#define GENPROMOTER_H

#include <string>
#include <map>
#include <iostream>

class GenPlasmid;
class GenOperon;
class GenCell;
class GenListPlasmid;
class GenPromoter{
	private:
	  std::string id;
	  std::vector<GenCell*> TD;
	  std::vector<GenCell*> TA;
	  GenListPlasmid* listPlasmid;
	  
	  int gate;
        
	public:
        GenPromoter(std::string id,int gate);//New promoter with id,contitutive state,and TF list
		
		
		
		void eraseListR();
		void eraseListA();
		void setGate(int);
		void setListPlasmid(GenListPlasmid *);
		void setListTA(std::vector<GenCell*> );
		void setListTD(std::vector<GenCell*> );
		int getGate();
		
		
		int check_gates();// Select the gate type and returns if they are on or off
        int yes_gate();  //Evaluate a yes gate type
        int not_gate();  //Evaluate a not gate type
        int and_gate();  //Evaluate a and gate type
        int or_gate();   //Evaluate a or gate type
		int xor_gate();
		int true_gate();
		int false_gate();
		
		bool itsInMapAll(std::vector<GenCell*> vector); // check if all GenCells in the vector are actives.
		bool itsInMapOne(std::vector<GenCell*> vector); //check if one GenCell in the vector is active.
		
}; 

#endif

