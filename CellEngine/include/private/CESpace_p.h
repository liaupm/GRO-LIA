#ifndef CESPACE_PRIVATE_HEADER
#define CESPACE_PRIVATE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "CESpace.h"

static inline int _cePointInsideAABB(ceVector2 point, float top, float buttom, float left, float right);

static inline void _ceCheckAndModifySpatialTable(ceSpace* space, ceBody* body);
void _ceResizeAndClearSpatialTable(ceSpace* space, float multiplier);
int _ceCheckBodiesInSpaceLimit(ceSpace* space);

void _ceClearSpatialTable(ceSpace* space);
void _ceClearSpatialTableByPressure(ceSpace* space, unsigned int pressure);

void _ceInsertBodyAuraInSpatialTable(ceSpace* space, ceBody* body);
void _ceInsertBodyInSpatialTable(ceSpace* space, ceBody* body);

static inline void _ceAddBodyToSpace(ceSpace* space, ceBody* body);

void _ceTotalBodiesAuraAction(ceSpace* space, void (*actionBetweenBodies)(ceBody*, ceBody*));
void _ceTotalBodiesAction(ceSpace* space, void (*actionBetweenBodies)(ceBody*, ceBody*));
void _ceCollideBodiesAtRing(ceSpace* space, unsigned int internalRing, unsigned int externalRing);

static inline void _ceResetRingMemory(ceSpace* space);
void _ceFindEdges(ceSpace* space);
void _ceFindRings(ceSpace* space);

void _ceExpandRing(ceSpace* space, unsigned int ring);
void _ceRelaxRings(ceSpace* space, unsigned int internalRing, unsigned int externalRing, unsigned int physicsIterations);
void _ceRelaxEntirePopulation(ceSpace* space, unsigned int physicsIterations);
void _ceRelaxBodiesNotDetected(ceSpace* space);

int* _ceDebugFindLimits(ceSpace* space, unsigned int* size); //its necesary free the array returned

//****************************************************************************
//                          Inline implementations
//****************************************************************************

int _cePointInsideAABB(ceVector2 point, float top, float buttom, float left, float right)
{
    return top <= point.y && point.y <= buttom && left <= point.x && point.x <= right;
}

void _ceAddBodyToSpace(ceSpace* space, ceBody* body)
{
    body->space = space;
    body->next = NULL;
    body->id = space->currentID++;
    if(space->lastBody != NULL)
    {
        space->lastBody->next = body;
        body->previous = space->lastBody;
    }
    else
    {
        space->firstBody = body;
        body->previous = NULL;
    }
    space->lastBody = body;
    space->populationSize++;
}

void _ceResetRingMemory(ceSpace* space)
{
    space->ringsBufferSize = 0;
    if(space->ringsBufferMemory < space->populationSize)
    {
        space->ringsBufferMemory = space->populationSize * 2;
        space->ringsBuffer = (ceBody**)realloc(space->ringsBuffer, sizeof(ceBody*) * space->ringsBufferMemory);
    }

    if(space->ringsIndicatorMemory < space->ringsNumber * 2)
    {
        space->ringsIndicatorMemory = space->ringsNumber * 2;
        space->ringsIndicator = (unsigned int*)realloc(space->ringsIndicator, sizeof(unsigned int) * space->ringsIndicatorMemory);
    }
    space->ringsNumber = 0;
}

void _ceCheckAndModifySpatialTable(ceSpace* space, ceBody* body)
{

    unsigned int x = abs(((int)body->center.x) / ceBoxLength);
    unsigned int y = abs(((int)body->center.y) / ceBoxLength);
    unsigned int middleTableBoxLength = space->tableBoxLength / 2;
    unsigned int max = x > y ? x : y;

    if(max + body->length >= middleTableBoxLength)
        _ceResizeAndClearSpatialTable(space, (max + body->length) / middleTableBoxLength + 1);
}

#ifdef __cplusplus
}
#endif

#endif //CESPACE_PRIVATE_HEADER