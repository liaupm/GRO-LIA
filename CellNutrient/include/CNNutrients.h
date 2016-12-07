#ifndef CENUTRIENTS_HEADER
#define CENUTRIENTS_HEADER

#include "CNGrid.h"
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus 
extern "C" {
#endif

// Uptakes a random quantity of nutrients
float csNutrientUptake(signalGrid_t* grid, inter_t interval);

// Checks the nutrients concentration available
float _csCheckNutrients(csSignalBox_t* cell);

#ifdef __cplusplus
}
#endif


#endif
