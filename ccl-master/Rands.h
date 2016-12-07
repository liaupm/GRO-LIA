#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef RANDS_H
#define RANDS_H

#ifdef _WIN32

void _dorand48(unsigned short [3]);

double erand48(unsigned short [3]);

double drand48();

void srand48(long);

#endif // _WIN32

double fRand(double fMin, double fMax);

#endif // RANDS_H
