#include "CNNutrients.h"
#include "CNGrid.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

/* unsigned int _randRange(unsigned int M, unsigned int N) */
/* { */
/*    timespec ts; */
/*    clock_gettime(CLOCK_REALTIME, &ts); */
/*    return rand()%((N+1)-M)+M; */
/* } */

/* float _randRangeFloat(float M, float N) { */
/*    float random = ((float)rand())/(float)RAND_MAX; */
/*    float diff = N-M; */
/*    float r = random * diff; */
/*    return M+r; */
/* } */

float csNutrientUptake(signalGrid_t* grid, inter_t interval)
{
  unsigned int choosenRow = _randRange(interval.top.row, interval.bottom.row);
  unsigned int choosenCol = _randRange(interval.top.col, interval.bottom.col);

  vector_t* vrow = (vector_t*)(vector_get(grid->matrix, choosenRow));
  csSignalBox_t* choosenCell = (csSignalBox_t*)(vector_get(vrow, choosenCol));
  float substratum = choosenCell->substratumC;
  float percent = choosenCell->var;
  float variability = (1-percent)*substratum;
  float quantity;

  if (choosenCell == NULL) {
    choosenCell = (csSignalBox_t*)(vector_get(vrow, choosenCol-1));
    printf("Tamaño de la malla: %d\n", grid->ncells);
    printf("Rango filas: %d, %d\n", interval.top.row, interval.bottom.row);
    printf("Rango de colum: %d,%d\n", interval.top.col, interval.bottom.col);
    printf("ChoosenRow: %d\n", choosenRow);
    printf("ChoosenCol: %d\n", choosenCol);
  }
    
  if (choosenCell->availableConcentration <= 0) {
	quantity = 0;
    choosenCell->availableConcentration = 0;
  } else if (choosenCell->availableConcentration > substratum) {
	quantity = (float)_randRangeFloat((substratum-variability), substratum);
        choosenCell->availableConcentration -= quantity;
  } else {
	quantity = (float)_randRangeFloat(0, choosenCell->availableConcentration);
        choosenCell->availableConcentration -= quantity;
  }
  
  return choosenCell->availableConcentration;
}

float _csCheckNutrients(csSignalBox_t* cell)
{
  return cell->availableConcentration;
} 

