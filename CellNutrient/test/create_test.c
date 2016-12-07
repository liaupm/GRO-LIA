#include <stdlib.h>
#include <stdio.h>

#include "CEGrid.h"

int main(void)
{
  signalGrid_t* grid = ceCreateGrid(24.89677, 10, 2);

  for (unsigned int i = 0; i < grid->ncells; i++) {
    vector_t* vrow = vector_get(grid->matrix, i);
    for (unsigned int j = 0; j < grid->ncells; j++) {
      ceSignalBox_t* cell = vector_get(vrow, j);
      printf("  %f", cell->availableConcentration);
    }
    putchar('\n');
  }
  ceDestroyGrid(grid);
  return 0;
}
