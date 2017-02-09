#include "CellNutrient.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

float* csGridStep(signalGrid_t* grid, unsigned int size, float coords[][4])
{
  float* result = (float*)malloc(size*sizeof(float));

  // Do actions for each coord (bacteria)
  for (unsigned int i = 0; i < size; i++) {
    // Extract the individual real point
    struct rp_t realP;
    realP.x = coords[i][0]; realP.y = coords[i][1];
    realP.lengthX = coords[i][2]; realP.lengthY = coords[i][3];

    struct rp_t rTop;
    rTop.x = realP.x - realP.lengthX;
    rTop.y = realP.y + realP.lengthY;
    rTop.lengthX = 0; rTop.lengthY = 0;

    struct rp_t rBottom;
    rBottom.x = realP.x + realP.lengthX;
    rBottom.y = realP.y - realP.lengthY;
    rBottom.lengthX = 0; rBottom.lengthY = 0;

    // Extract centre
    struct rp_t center;
    center.x = (grid->ncells*grid->cellLength)/2;
    center.y = (grid->ncells*grid->cellLength)/2;

    vp_t vTop =  _RealToVirtual(center, rTop);
    vp_t vBottom =  _RealToVirtual(center, rBottom);
    
    // Check if the bacteria it's outside
    if (_CheckLimits(grid, vTop) || _CheckLimits(grid, vBottom)) {
     /*float maxDistTop = _OutOfBounds(grid, vTop);
      float maxDistBottom = _OutOfBounds(grid, vBottom);
      float maxDist = 0;

      if ((maxDistTop < 0) || (maxDistBottom < 0)) {
	maxDist = MIN(maxDistTop, maxDistBottom);
	maxDist = -maxDist; // Length should be positive
      } else {
	maxDist = MAX(maxDistTop, maxDistBottom);
      } */

      // Indicate number of cells needed to resize
      //unsigned int numberCells = (unsigned int)ceil(maxDist/grid->cellLength);
      unsigned int numberCells = 2; //Add always 2 cells -> temporal fix
      grid = csResize(grid, numberCells);
    } 

    // Get cells & uptake
    inter_t interval = csGetCells(grid, realP);
    float consum = csNutrientUptake(grid, interval);
    result[i] = consum;
  }

  //Aplicar difusión si se indico
  _Diffusion(grid);
  
  return result;
}
