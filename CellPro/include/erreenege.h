#ifndef ERREENEGE_H

#define ERREENEGE_H

#include <random>

class erreenege
{
	public:
		erreenege(bool, unsigned int);
		double normal_float(double, double);
        std::mt19937& get_generator();
	private:
		unsigned int seed;//---MARTIN
		std::mt19937 generator;//---MARTIN	
};

#endif
