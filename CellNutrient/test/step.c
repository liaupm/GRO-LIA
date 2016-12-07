#include <stdlib.h>
#include <stdio.h>

#include "CECellSignal.h"

int main(void)
{
  signalGrid_t* grid = ceCreateGrid(200, 6, 2);

  float coords[3][4] = {{18.5, -10.2, 1.4, 2.4}, {4.5, 7.8, 2, 5}, {7.9, 3.5, 5, 3}, {16.8, 13.7, 3, 5.7}};

  for (int i = 0; i < 10; i++) {
	float* eaten = ceGridStep(grid, 4, coords);
    
  for (unsigned int i = 0; i <= grid->ncells; i++) {
    vector_t* vrow = vector_get(grid->matrix, i);
    for (unsigned int j = 0; j <= grid->ncells; j++) {
      ceSignalBox_t* cell = vector_get(vrow, j);
      printf("  %f", cell->availableConcentration);
    }
    putchar('\n');
  } 
  
	/*for (int j = 0; j <= 2; j++) {
		printf("Consumido: %f\n", eaten[j]);
	} */
  }
  ceDestroyGrid(grid);
  return 0;
}
