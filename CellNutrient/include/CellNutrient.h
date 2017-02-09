#include "CNGrid.h"
#include "CNNutrients.h"

// Proceses all coordinates & uptakes nutrients depending on their position inside the grid.
// Returns the quantities uptaked
// float* ceGridStep(signalGrid_t* grid, unsigned int size, float coords[size][4]);

#ifdef __cplusplus 
extern "C" {
#endif

float* csGridStep(signalGrid_t*, unsigned int, float[][4]);

#ifdef __cplusplus
}
#endif
