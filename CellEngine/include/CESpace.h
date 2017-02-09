#ifndef CESPACE_HEADER
#define CESPACE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "CEVector2.h"
#include "CEBody.h"

#include <stdlib.h>
#include <math.h>

/* Struct used for internal compute. */
typedef struct ceBox
{
    unsigned short size;
    ceBody** bodies;
} ceBox;

/* Struct for simulate the space. All bacterials created will need
attached to a space for simulate his life. */
typedef struct ceSpace
{
    unsigned int tableBoxLength;
    unsigned int middleTableLength;
    unsigned int limit;
    ceBox* spatialTable;

    unsigned int populationSize;
    ceBody* firstBody;
    ceBody* lastBody;
    unsigned int currentID;

    ceBody** ringsBuffer;
    unsigned int ringsBufferMemory;
    unsigned int ringsBufferSize;

    unsigned int* ringsIndicator;
    unsigned int ringsIndicatorMemory;
    unsigned int ringsNumber;
} ceSpace;

/* Create a new space to contains a population. */
ceSpace* ceCreateSpace();

/* Destroy the space and all bodies associated, it's not necessary
to call to ceDestroyBody() for each body after this method. */
void ceDestroySpace(ceSpace* space);

/* Returns a body if position is inside it or
NULL if there is no body there. */
ceBody* ceGetBody(const ceSpace* space, ceVector2 position);

/* Returns a list of bodies belong to a rectangle of the space.
The rectangle is given by the position and dimension parameters.
Size, is a pointer to will contain the size of the list returned.
It's necessary free the vector. */
ceBody** ceGetBodies(const ceSpace* space, unsigned int* size, ceVector2 position, ceVector2 dimension);

/* The main function of the engine. A call to this method will be
compute an entire frame over the space. */
int ceStep(ceSpace* space);

#ifdef __cplusplus
}
#endif

#endif //CESPACE_HEADER