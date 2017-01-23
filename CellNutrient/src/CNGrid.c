#include "CNGrid.h"
#include <string.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

float totalN;
int type;
float kdiff;
float kdeg;

//////////////////// AUXILIARY FUNCTIONS ////////////////////////

float _LevelOfSignal(signalGrid_t* grid, int rowIndx, int colIndx)
{
  vector_t* row = (vector_t*)(vector_get(grid->matrix, rowIndx));
  csSignalBox_t* cell = (csSignalBox_t*)(vector_get(row, colIndx));

  return cell->substratumC;
}

void _Diffusion(signalGrid_t* grid)
{
  if (kdiff != 0) {
    float nutrient;
    // Eliminamos primera fila y ultima fila
    vector_t* dsig = vector_create(grid->ncells-2);
    for (int i = 1; i < (int)(grid->ncells-1); i++) {
      // Eliminamos primera columna y ultima columna
      vector_t* dsigRow = vector_create(grid->ncells-2);
      for (int j = 1; j < (int)(grid->ncells-1); j++) {
	nutrient = _LevelOfSignal(grid, i, j);

	float value = (-6*kdiff*nutrient - kdeg*nutrient) +
	  (kdiff * (0.5*_LevelOfSignal(grid,i+1,j-1) + _LevelOfSignal(grid,i+1,j) + 0.5*_LevelOfSignal(grid,i+1,j+1)
		    + _LevelOfSignal(grid,i,j-1) + _LevelOfSignal(grid,i,j+1)
		    + 0.5*_LevelOfSignal(grid,i-1,j-1) + _LevelOfSignal(grid,i-1,j) + 0.5*_LevelOfSignal(grid,i-1,j+1)));

	vector_append(dsigRow, &value);
      }
      vector_append(dsig, dsigRow);
    }

    for (int i = 1; i < (int)(grid->ncells-1); i++) {
      vector_t* vrow = (vector_t*)(vector_get(grid->matrix, i));
      vector_t* dsigRow = (vector_t*)(vector_get(dsig, i));
      for (int j = 1; j < (int)(grid->ncells-1); j++) {
	csSignalBox_t* cell = (csSignalBox_t*)(vector_get(vrow, j));
	float dsigValue = *((float*)vector_get(dsigRow, j));
	cell->substratumC += 0.1 * dsigValue;
	if ((cell->substratumC) < 0.0)
	  cell->substratumC = 0.0;
      }
    }
  }
}

float _OutOfBounds(signalGrid_t* grid, vp_t point)
{
  float dists[2];
  float distance;
  unsigned int len = grid->ncells*grid->cellLength;
  
  if (point.x >= len) {
    dists[0] = point.x-len;
  } else if (point.x < 0) {
    dists[0] = point.x;
  } else { // If point is into the grid
    dists[0] = 0.0;
  }

  if (point.y >= len) {
    dists[1] = point.y-len;
  } else if (point.y < 0) {
    dists[1] = point.y;
  } else { // If point is into the grid
    dists[1] = 0.0;
  }

  if (dists[0] <= 0 || dists[1] <= 0) {
    distance = MIN(dists[0],dists[1]);
  } else {
    distance = MAX(dists[0], dists[1]);
  }
  
  return distance;
}

vp_t _RealToVirtual(rp_t centre, rp_t realP)
{
  struct vp_t virtualP;
  virtualP.x = centre.x+realP.x;
  virtualP.y = centre.y-realP.y;

  return virtualP;
}

sector_t _getSector(vp_t virtualP, float cellLength)
{
  struct sector_t sector;  

  sector.row = (unsigned int)floor(virtualP.x/cellLength);
  sector.col = (unsigned int)floor(virtualP.y/cellLength);

  return sector;
}

int _CheckLimits(signalGrid_t* grid, vp_t virtualP)
{
  // Check if the bacteria it's outside
    if ((virtualP.x <= 0)) {
	return 1;
    }
    if (virtualP.x >= (grid->ncells*grid->cellLength)) {
	return 1;
    }
    if ((virtualP.y <= 0)) {
	return 1;
    }
    if (virtualP.y >= (grid->ncells*grid->cellLength)) {
	return 1;
    } 

    return 0;
}

// Devuelve la cantidad que hay que introducir en el cuadro
float _Reduction(float localDistance)
{
    float percent;

    switch (type) {
     case 0:
        if (localDistance <= RADIO) {
        return totalN;
        } else {
          return totalN;
        }
     case 1:
        // FORMULA => y = Nutr/distance * <dist_each_bacteria> + Nutr
        return ((-(totalN/RADIO))*localDistance)+totalN;
     case 2:
       percent = (50*totalN)/100;
       return _randRangeFloat((totalN-percent), totalN);
    
  }
  return 0;
}

float _Distance(sector_t center, sector_t newCell)
{
  struct sector_t diff;
  diff.row = (center.row-newCell.row);
  diff.col = (center.col-newCell.col);
  float dist = sqrt((diff.row*diff.row)+(diff.col*diff.col));

  return _Reduction(dist);
}

sector_t _Center(signalGrid_t* grid)
{
  struct vp_t center;
  center.x = (grid->ncells*grid->cellLength)/2;
  center.y = (grid->ncells*grid->cellLength)/2;
  struct sector_t centerSector = _getSector(center, grid->cellLength);
  return centerSector;
}

unsigned int _randRange(unsigned int M, unsigned int N)
{
    /*struct timespec ts;

    #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;

    #else
    clock_gettime(CLOCK_REALTIME, &ts);
    #endif*/

    return rand()%((N+1)-M)+M;
}

float _randRangeFloat(float M, float N) {
   float random = ((float)rand())/(float)RAND_MAX;
   float diff = N-M;
   float r = random * diff;
   return M+r;
}

signalGrid_t* csResize(signalGrid_t* grid, float max)
{
 
  vector_t* localVector = (vector_t*)(vector_get(grid->matrix, 0));
  csSignalBox_t* localCell = (csSignalBox_t*)(vector_get(localVector, 0));
  struct sector_t centerSector; struct sector_t cellSector;
  float nutrients; //Nutrientes calculados sengun la distancia
 
  // Add max number of columns in the existing rows
  grid->ncells += max; // Se añaden max columnas a la derecha
  for (unsigned int row = 0; row < grid->ncells; row++) {
    vector_t* vrow = (vector_t*)(vector_get(grid->matrix, row));
    for (unsigned int newcol = 0; newcol < max; newcol++) {  
      csSignalBox_t* cell = (csSignalBox_t*)malloc(sizeof(csSignalBox_t));
      centerSector = _Center(grid);
      cellSector.row = row; cellSector.col = vector_len(vrow)+1;
      nutrients = _Distance(centerSector, cellSector);
      cell->initialConcentration = nutrients;
      cell->availableConcentration = cell->initialConcentration;
      cell->substratumC = localCell->substratumC;
      cell->var = localCell->var;
      vector_append(vrow, cell);
    }
  }
 
  grid->ncells += max; //Se añaden max columnas a la izquierda
  for (unsigned int row = 0; row < grid->ncells; row++) {
    vector_t* vrow = (vector_t*)(vector_get(grid->matrix, row));
    for(unsigned int newcol = 0; newcol < max; newcol++) {
      csSignalBox_t* cell = (csSignalBox_t*)malloc(sizeof(csSignalBox_t));
      centerSector = _Center(grid);
      cellSector.row = row; cellSector.col = newcol;
      nutrients = _Distance(centerSector, cellSector);
      cell->initialConcentration = nutrients;
      cell->availableConcentration = cell->initialConcentration;
      cell->substratumC = localCell->substratumC;
      cell->var = localCell->var;
      vector_prepend(vrow, cell);
    }   
  }
 
  for (unsigned int newrow = 0; newrow < max; newrow++) {
    vector_t* vrow = vector_create(grid->ncells);
    for (unsigned int col = 0; col < grid->ncells; col++) {
      csSignalBox_t* cell = (csSignalBox_t*)malloc(sizeof(csSignalBox_t));
      centerSector = _Center(grid);
      cellSector.row = newrow; cellSector.col = col;
      nutrients = _Distance(centerSector, cellSector);
      cell->initialConcentration = nutrients;
      cell->availableConcentration = cell->initialConcentration;
      cell->substratumC = localCell->substratumC;
      cell->var = localCell->var;
      vector_append(vrow, cell);
   }
   vector_prepend(grid->matrix, vrow);
 } 

  for (unsigned int newrow = 0; newrow < max; newrow++) {
    vector_t* vrow = vector_create(grid->ncells);
    int rowLen = vector_len(grid->matrix)+1;
    for (unsigned int col = 0; col < grid->ncells; col++) {
      csSignalBox_t* cell = (csSignalBox_t*)malloc(sizeof(csSignalBox_t));
      centerSector = _Center(grid);
      cellSector.row = rowLen; cellSector.col = col;
      nutrients = _Distance(centerSector, cellSector);
      cell->initialConcentration = nutrients;
      cell->availableConcentration = cell->initialConcentration;
      cell->substratumC = localCell->substratumC;
      cell->var = localCell->var;
      vector_append(vrow, cell);
    }
    vector_append(grid->matrix, vrow);
  }
 
  return grid;
}

/////////////////////////////////////////////////////////////////

signalGrid_t* csCreateGrid(float initialConcentration, float tasa, float variability, unsigned int gridLength, float eachCellLength, int simtype, float *diffusion)
{
  signalGrid_t* grid = (signalGrid_t*)malloc(sizeof(signalGrid_t));
  grid->ncells = gridLength;
  grid->cellLength = eachCellLength;
  grid->matrix = vector_create(grid->ncells);

  totalN = initialConcentration;
  type = simtype;
  struct sector_t centerSector; struct sector_t cellSector;  
  float nutrients; //Nutrientes calculados sengun la distancia
  centerSector = _Center(grid);
  
 // Code to initialize the grid referred to the distante to the center
  for (unsigned int i = 0; i < grid->ncells; i++) {
    vector_t* vrow = vector_create(grid->ncells);
    for (unsigned int j = 0; j < grid->ncells; j++) {
      csSignalBox_t* cell = (csSignalBox_t*)malloc(sizeof(csSignalBox_t));
      cellSector.row = i; cellSector.col = j;
      nutrients = _Distance(centerSector, cellSector);
      cell->initialConcentration = nutrients;
      cell->availableConcentration = cell->initialConcentration;
      cell->substratumC = tasa;
      cell->var = variability;
      vector_append(vrow, cell);
    }
    vector_append(grid->matrix, vrow);
  }

  if (diffusion[1] != 0)
    kdiff = diffusion[0];
  else
    kdiff = 0;

  if (diffusion[3] != 0)
    kdeg = diffusion[2];
  else
    kdeg = 0;
    

 return grid;
}

void csDestroyGrid(signalGrid_t* grid)
{
  if(grid != NULL)
  {
	for (unsigned int i = 0; i < grid->ncells; i++) {
    		vector_t* vrow = (vector_t*)(vector_get(grid->matrix, i));
    		vector_destroy(vrow, 1);
  	}
  	vector_destroy(grid->matrix, 0);
  	free(grid);
  }
}

inter_t csGetCells(signalGrid_t* grid, rp_t realP)
{
  // Obtain center
  struct rp_t center;
  center.x = (grid->ncells*grid->cellLength)/2;
  center.y = (grid->ncells*grid->cellLength)/2;

  /* Obtain the virtual point associated 
  with the top & bottom points */
  struct rp_t rTop;
  rTop.x = realP.x - realP.lengthX;
  rTop.y = realP.y + realP.lengthY;
  rTop.lengthX = 0; rTop.lengthY = 0;

  struct rp_t rBottom;
  rBottom.x = realP.x + realP.lengthX;
  rBottom.y = realP.y - realP.lengthY;
  rBottom.lengthX = 0; rBottom.lengthY = 0;

  // Obtain virtual points of top & bottom
  vp_t vTop =  _RealToVirtual(center, rTop);
  vp_t vBottom =  _RealToVirtual(center, rBottom);

  // Obtain sector related with top & bottom points
  sector_t sectorTop = _getSector(vTop, grid->cellLength);
  sector_t sectorBottom = _getSector(vBottom, grid->cellLength);

  // Create & fill the interval with both sectors
  struct inter_t interval;
  interval.top = sectorTop;
  interval.bottom = sectorBottom;

  return interval;
}
