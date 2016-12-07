#ifndef NORMAL_H
#define NORMAL_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <list>
#include <vector>
#include <string>
#include <map>

#define PI 3.1415927

//float exponential ( float );
float cdf ( float );
float cdf2 ( float );
float cdf3 ( float, float, float);
float pdf ( float, float, float );
float gaussian_random ( float, float );
float uneven_gaussian ( float, float, float );

#endif // NORMAL_H
