#include <stdlib.h>
#include <stdio.h>

#include "CEGrid.h"
#include "CENutrients.h"

int main(void)
{
  signalGrid_t* grid = ceCreateGrid(16.78, 10, 2);
  inter_t* interval = (inter_t*)malloc(sizeof(inter_t));
  sector_t* top = (sector_t*)malloc(sizeof(sector_t));
  sector_t* bottom = (sector_t*)malloc(sizeof(sector_t));
  top->row = 1; top->col = 3;
  bottom->row = 5; bottom->col = 9;
  interval->top = top; interval->bottom = bottom;

  float cantidad = ceNutrientUptake(grid, interval);

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
