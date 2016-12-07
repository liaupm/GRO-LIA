#include <stdio.h>
#include <stdlib.h>

#include "CEGrid.h"

int main(void)
{
  signalGrid_t* grid = ceCreateGrid(14.4, 10, 2);
  rp_t* realP = (rp_t*)malloc(sizeof(rp_t));
  realP->x = 7.0; realP->y = 4.3;
  realP->lengthX = 1.3; realP->lengthY = 3.2;

  inter_t* cells = ceGetCells(grid, realP);

  printf("Top: (%d,%d)\n",  cells->top->col, cells->top->row);
  printf("Bottom: (%d,%d)\n", cells->bottom->col, cells->bottom->row);

  return 0;
}
