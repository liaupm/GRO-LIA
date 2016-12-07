#ifndef CESIGNAL_GRID_HEADER
#define CESIGNAL_GRID_HEADER

#include "CNvector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus 
extern "C" {
#endif

#define HOMOGENEOUS 0
#define GRADIENT 1
#define RANDOM 2
#define RADIO 192 

/* Struct to represent each box in the grid */
typedef struct csSignalBox
{
  float initialConcentration;
  float availableConcentration;
  float substratumC; //Cantidad critica de nutrientes
  float var; //Variabilidad con respecto al sustrato critico
} csSignalBox_t;

/* Struct to represent a real point*/
typedef struct rp_t
{
  float x;
  float y;
  float lengthX;
  float lengthY;
} rp_t;

/* Struct to represent a virtual point*/
typedef struct vp_t
{
  float x;
  float y;
} vp_t;

/* Struct to represent the virtual point position into the matrix */
typedef struct sector_t
{
  unsigned int row;
  unsigned int col;
} sector_t;

/* Struct to represent an interval of boxes*/
typedef struct inter_t
{
  sector_t top;
  sector_t bottom;
} inter_t;

typedef struct signalGrid
{
  unsigned int ncells;
  //unsigned int cellLength;
  float cellLength;
  vector_t* matrix;
} signalGrid_t;
  
////////////////
// PROTOTYPES //
////////////////

unsigned int _randRange(unsigned int M, unsigned int N);
float _randRangeFloat(float M, float N);  

// Creates a new grid. recibe initial nutrients concentration, number of cells and length of each one */
signalGrid_t* csCreateGrid(float, float, float, unsigned int, float, int, float*);

// Destroys the grid
void csDestroyGrid(signalGrid_t*);

// Gets all boxes's indices related with a body represented by its real point 
inter_t csGetCells(signalGrid_t* grid, rp_t realP);

signalGrid_t* csResize(signalGrid_t* grid, float maxDistance);

/////////////////////////
// AUXILIARY FUNCTIONS //
/////////////////////////

float _LevelOfSignal(signalGrid_t* grid, int rowIndx, int colIndx);

void _Diffusion(signalGrid_t* grid);
  
// Calculates the distance between a pount located out of the grid & the grid
float _OutOfBounds(signalGrid_t* grid, vp_t point);

// Tranforms a real point to a virtual point
vp_t _RealToVirtual(rp_t centre, rp_t realP);

// Gets the cell in which a real point is located
sector_t _getSector(vp_t virtualP, float cellLength);

// Checks if a virtual point is located outside the grid
int _CheckLimits(signalGrid_t* grid, vp_t virtualP);

float _Distance(sector_t center, sector_t top);

float _Reduction(float localDistance);

sector_t _Center(signalGrid_t* grid);
 
#ifdef __cplusplus
}
#endif


#endif //CESIGNAL_GRID_HEADER
