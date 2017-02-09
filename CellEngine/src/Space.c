#include "private/CellEngine_p.h"
#include "private/CEBody_p.h"
#include "CESpace.h"
#include "private/CESpace_p.h"


//**************************************************************************
//                              Public functions
//**************************************************************************

ceSpace* ceCreateSpace()
{
    ceSpace* space = (ceSpace*) malloc(sizeof(ceSpace));

    space->tableBoxLength = _CE_INITIAL_TABLE_BOX_LENGTH;
    space->middleTableLength = (space->tableBoxLength * ceBoxLength) / 2;
    space->limit = space->tableBoxLength * _CE_DEFAULT_LIMIT_VALUE;

    space->spatialTable = (ceBox*) malloc(sizeof(ceBox) * space->tableBoxLength * space->tableBoxLength);
    for(unsigned int i = 0; i < space->tableBoxLength * space->tableBoxLength; i++)
    {
        space->spatialTable[i].bodies = (ceBody**) malloc(sizeof(ceBody*) * ceMaxBodiesPerBox);
        space->spatialTable[i].size = 0;
    }

    space->populationSize = 0;
    space->firstBody = NULL;
    space->lastBody = NULL;
    space->currentID = 0;

    space->ringsBuffer = NULL;
    space->ringsBufferMemory = 0;

    space->ringsNumber = 0;
    space->ringsIndicatorMemory = _CE_INITIAL_RINGS_INDICATOR_SIZE;
    space->ringsIndicator = (unsigned int*)malloc(sizeof(unsigned int) * space->ringsIndicatorMemory);

    return space;
}

void ceDestroySpace(ceSpace* space)
{
    ceBody* body = space->firstBody;
    while(body != NULL)
    {
        ceBody* nextAux = body->next;
        free(body->auraContacts);
        free(body);
        body = nextAux;
    }
    for(unsigned int i = 0; i < space->tableBoxLength * space->tableBoxLength; i++)
        free(space->spatialTable[i].bodies);
    free(space->spatialTable);
    free(space->ringsBuffer);
    free(space->ringsIndicator);
    free(space);
}

ceBody* ceGetBody(const ceSpace* space, ceVector2 position)
{
    int middleTableLength = space->middleTableLength;
    if(-middleTableLength <= position.x && position.x < middleTableLength && -middleTableLength <= position.y && position.y < middleTableLength)
    {
        unsigned int x = (middleTableLength + (int)position.x) / ceBoxLength;
        unsigned int y = (middleTableLength + (int)position.y) / ceBoxLength;
        unsigned int index = y * space->tableBoxLength + x;
        for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
        {
            ceBody* body = space->spatialTable[index].bodies[i];
            if(ceTestBodyPoint(body, position))
                return body;
        }
    }
    return NULL;
}

ceBody** ceGetBodies(const ceSpace* space, unsigned int* size, ceVector2 position, ceVector2 dimension)
{
    /* Find a square belong to the spatial table */
    *size = 0;
    int middleTableLength = space->middleTableLength;
    ceVector2 position2 = ceGetAddVectors2(position, dimension);

    if(position.x > position2.x)
    {
        float aux = position.x;
        position.x = position2.x;
        position2.x = aux;
    }
    if(position.y > position2.y)
    {
        float aux = position.y;
        position.y = position2.y;
        position2.y = aux;
    }

    float top = (position.y > -middleTableLength) ? position.y : -middleTableLength;
    float buttom = (position2.y < middleTableLength) ? position2.y : middleTableLength - 1;
    float left = (position.x > -middleTableLength) ? position.x : -middleTableLength;
    float right = (position2.x < middleTableLength) ? position2.x : middleTableLength - 1;

    if(buttom - top <= 0 || right - left <= 0)
        return NULL;

    /* Find the ceBoxes of the selection */
    unsigned int topIndex = (middleTableLength + (int)top) / ceBoxLength;
    unsigned int rightIndex = (middleTableLength + (int)right) / ceBoxLength;
    unsigned int buttomIndex = (middleTableLength + (int)buttom) / ceBoxLength;
    unsigned int leftIndex = (middleTableLength + (int)left) / ceBoxLength;
    ceBody** bodies = (ceBody**) malloc(sizeof(ceBody*) * ceMaxBodiesPerBox * (buttomIndex - topIndex + 1) * (rightIndex - leftIndex + 1));

    /* Choose the correct Bodies */
    for(unsigned int y = topIndex; y <= buttomIndex; y++)
        for(unsigned int x = leftIndex; x <= rightIndex; x++)
        {
            unsigned int index = y * space->tableBoxLength + x;
            for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
            {
                ceBody* body = space->spatialTable[index].bodies[i];
                if(body->stamp != _CE_STAMP_SELECTED)
                {
                    if(y == topIndex || y == buttomIndex || x == leftIndex || x == rightIndex)
                    {
                        if(_cePointInsideAABB(body->p1, top, buttom, left, right) || _cePointInsideAABB(body->p2, top, buttom, left, right))
                        {
                            bodies[(*size)++] = body;
                            body->stamp = _CE_STAMP_SELECTED;
                        }
                        else
                        {
                            if(y == topIndex && ceTestBodySegment(body, ceGetVector2(left, top), ceGetVector2(right, top)))
                            {
                                bodies[(*size)++] = body;
                                body->stamp = _CE_STAMP_SELECTED;
                            }
                            if(body->stamp != _CE_STAMP_SELECTED && y == buttomIndex && ceTestBodySegment(body, ceGetVector2(left, buttom), ceGetVector2(right, buttom)))
                            {
                                bodies[(*size)++] = body;
                                body->stamp = _CE_STAMP_SELECTED;
                            }
                            if(body->stamp != _CE_STAMP_SELECTED && x == leftIndex && ceTestBodySegment(body, ceGetVector2(left, top), ceGetVector2(left, buttom)))
                            {
                                bodies[(*size)++] = body;
                                body->stamp = _CE_STAMP_SELECTED;
                            }
                            if(body->stamp != _CE_STAMP_SELECTED && x == rightIndex && ceTestBodySegment(body, ceGetVector2(right, top), ceGetVector2(right, buttom)))
                            {
                                bodies[(*size)++] = body;
                                body->stamp = _CE_STAMP_SELECTED;
                            }
                        }
                    }
                    else
                    {
                        bodies[(*size)++] = body;
                        body->stamp = _CE_STAMP_SELECTED;
                    }
                }
            }
        }

    if(*size == 0)
    {
        free(bodies);
        return NULL;
    }
    return bodies;
}

int ceStep(ceSpace* space)
{
    /*** Prepare step ***/
    /* Check limit and clear spatial table. */
    if(_ceCheckBodiesInSpaceLimit(space))
        _ceResizeAndClearSpatialTable(space, _CE_RESIZE_MULTIPLIER);
    else
        _ceClearSpatialTable(space);

    /* Per body... */
    ceBody* body = space->firstBody;
    while(body != NULL)
    {
        /* Clear previous contacts. */
        body->auraContactsSize = 0;

        /* Reset pressure indicator */
        body->pressure = _CE_PRESSURE_CLEAR;

        /* Reset the stamps */
        body->stamp = _CE_STAMP_CLEAR;

        /* The previous positions are the same. */
        body->centerPreStep = body->center;

        //This reset could be avoided. but without it, the program crash by a NaN in totalTorque. Fix it.
        _ceResetForces(body);

        /* Recalculate AABB Aura. */
        _ceComputeAABBAura(body);

        /* Insert again the body in spatial table. */
        _ceInsertBodyAuraInSpatialTable(space, body);

        body = body->next;
    }

    /* Create all contacts. */
    _ceTotalBodiesAuraAction(space, _ceCreateContact);

    /* Create rings. */
    _ceResetRingMemory(space);
    _ceFindEdges(space);
    _ceFindRings(space);

    /*** Expansion and relaxation process. ***/
    int externalRing = space->ringsNumber - ceCombinedRing;
    if(externalRing > 0)
    {
        /* Compute the iterations needed. */
        int physicsIterations = cePhysicsIterationsWithoutRingSystem / (space->ringsNumber - ceCombinedRing + 1);
        if(physicsIterations < cePhysicsIterationsPerCombinedRing)
            physicsIterations = cePhysicsIterationsPerCombinedRing;

        /* Create combined rings */
        for(; externalRing >= 0; externalRing--)
        {
            /* Expansion process. */
            _ceExpandRing(space, externalRing);

            /* Relaxation process. */
            _ceRelaxRings(space, externalRing + ceCombinedRing - 1, externalRing, physicsIterations);
        }

        /* Compute bodies not detected by the rings algorithm. */
        _ceRelaxBodiesNotDetected(space);
    }
    else
    {
        /* Relax the entire population. */
        _ceRelaxEntirePopulation(space, cePhysicsIterationsWithoutRingSystem);
    }

    /* Default return value. */
    return 0;
}

//**************************************************************************
//                             Private functions
//**************************************************************************

void _ceResizeAndClearSpatialTable(ceSpace* space, float multiplier)
{
    for(unsigned int i = 0; i < space->tableBoxLength * space->tableBoxLength; i++)
        free(space->spatialTable[i].bodies);

    /* After resize, tableBoxLength have to be a pair number. */
    space->tableBoxLength = space->tableBoxLength * multiplier;
    if(space->tableBoxLength & 1)
        space->tableBoxLength++;

    space->middleTableLength = ceBoxLength * space->tableBoxLength / 2;
    space->limit = space->tableBoxLength * _CE_DEFAULT_LIMIT_VALUE;

    space->spatialTable = (ceBox*) realloc(space->spatialTable, sizeof(ceBox) * space->tableBoxLength * space->tableBoxLength);
    for(unsigned int i = 0; i < space->tableBoxLength * space->tableBoxLength; i++)
    {
        space->spatialTable[i].bodies = (ceBody**) malloc(sizeof(ceBody*) * ceMaxBodiesPerBox);
        space->spatialTable[i].size = 0;
    }
}

void _ceInsertBodyAuraInSpatialTable(ceSpace* space, ceBody* body)
{
    /* It could alloc a body outside of the spatial table, in the positive X axis,
    but this function is NEVER called with a body outside of the spatial table. */
    unsigned int top = (space->middleTableLength + (int)(body->center.y - body->halfDimensionAABBAura.y)) / ceBoxLength;
    unsigned int right = (space->middleTableLength + (int)(body->center.x + body->halfDimensionAABBAura.x)) / ceBoxLength;
    unsigned int bottom = (space->middleTableLength + (int)(body->center.y + body->halfDimensionAABBAura.y)) / ceBoxLength;
    unsigned int left = (space->middleTableLength + (int)(body->center.x - body->halfDimensionAABBAura.x)) / ceBoxLength;
    for(unsigned int y = top; y <= bottom; y++)
        for(unsigned int x = left; x <= right; x++)
        {
            unsigned int index = y * space->tableBoxLength + x;
            //if(index >= space->tableBoxLength * space->tableBoxLength) //DEBUG
            //    printf("**** SpatialTableError (Aura) ****\n");
            if(space->spatialTable[index].size < ceMaxBodiesPerBox)
                space->spatialTable[index].bodies[space->spatialTable[index].size++] = body;
            //else //DEBUG
            //    printf("**** SpatialTableLimit (Aura) ****\n");
        }
}

void _ceInsertBodyInSpatialTable(ceSpace* space, ceBody* body)
{
    /* It could alloc a body outside of spatial table, in the positive axis X,
    but this function is NEVER called with a body outside of the spatial table. */
    unsigned int top = (space->middleTableLength + (int)(body->center.y - body->halfDimensionAABB.y)) / ceBoxLength;
    unsigned int right = (space->middleTableLength + (int)(body->center.x + body->halfDimensionAABB.x)) / ceBoxLength;
    unsigned int bottom = (space->middleTableLength + (int)(body->center.y + body->halfDimensionAABB.y)) / ceBoxLength;
    unsigned int left = (space->middleTableLength + (int)(body->center.x - body->halfDimensionAABB.x)) / ceBoxLength;
    for(unsigned int y = top; y <= bottom; y++)
        for(unsigned int x = left; x <= right; x++)
        {
            unsigned int index = y * space->tableBoxLength + x;
            //if(index >= space->tableBoxLength * space->tableBoxLength) //DEBUG
            //    printf("**** SpatialTableError ****\n");
            if(space->spatialTable[index].size < ceMaxBodiesPerBox)
                space->spatialTable[index].bodies[space->spatialTable[index].size++] = body;
            //else //DEBUG
            //    printf("**** SpatialTableLimit ****\n");
        }
}

void _ceClearSpatialTable(ceSpace* space)
{
    for(unsigned int i = 0; i < space->tableBoxLength * space->tableBoxLength; i++)
        space->spatialTable[i].size = 0;
}

//Improve the logic around this function to increase its performance. It is necessary avoid the extra lineal dependence.
void _ceClearSpatialTableByPressure(ceSpace* space, unsigned int pressure)
{
    for(unsigned int index = 0; index < space->tableBoxLength * space->tableBoxLength; index++)
    {
        for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
        {
            if(space->spatialTable[index].bodies[i]->pressure <= pressure)
                space->spatialTable[index].size = i;
        }
    }
}

int _ceCheckBodiesInSpaceLimit(ceSpace* space)
{
    unsigned int tableBoxLength = space->tableBoxLength;
    unsigned int limit = space->limit;
    unsigned int limit2 = tableBoxLength - limit;
    unsigned int increment = tableBoxLength - 2 * limit - 1;

    unsigned int i;

    /* Top limit line. */
    for(i = tableBoxLength * limit + limit; i < tableBoxLength * limit + limit2; i++)
        if(space->spatialTable[i].size > 0)
            return 1;

    /* Left and right limit lines. */
    for(i += limit * 2; i < tableBoxLength * (limit2 - 1) + limit; i += tableBoxLength)
    {
        if(space->spatialTable[i].size > 0)
            return 1;
        if(space->spatialTable[i + increment].size > 0)
            return 1;
    }

    /* Bottom limit line. */
    for(; i < tableBoxLength * (limit2 - 1) + limit2; i++)
        if(space->spatialTable[i].size > 0)
            return 1;

    return 0;
}

void _ceTotalBodiesAuraAction(ceSpace* space, void (*actionBetweenBodies)(ceBody*, ceBody*))
{
    ceBody* b1 = space->firstBody;
    while(b1 != NULL)
    {
        unsigned int top = (space->middleTableLength + (int)(b1->center.y - b1->halfDimensionAABBAura.y)) / ceBoxLength;
        unsigned int right = (space->middleTableLength + (int)(b1->center.x + b1->halfDimensionAABBAura.x)) / ceBoxLength;
        unsigned int bottom = (space->middleTableLength + (int)(b1->center.y + b1->halfDimensionAABBAura.y)) / ceBoxLength;
        unsigned int left = (space->middleTableLength + (int)(b1->center.x - b1->halfDimensionAABBAura.x)) / ceBoxLength;
        for(unsigned int y = top; y <= bottom; y++)
            for(unsigned int x = left; x <= right; x++)
            {
                unsigned int index = y * space->tableBoxLength + x;
                for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
                {
                    ceBody* b2 = space->spatialTable[index].bodies[i];
                    if(b1->id < b2->id)
                    {
                        if(b2->stamp != b1->id)
                        {
                            (*actionBetweenBodies)(b1, b2);
                            b2->stamp = b1->id;
                        }
                    }
                }
            }
        b1 = b1->next;
    }
}

void _ceTotalBodiesAction(ceSpace* space, void (*actionBetweenBodies)(ceBody*, ceBody*))
{
    ceBody* b1 = space->firstBody;
    while(b1 != NULL)
    {
        unsigned int top = (space->middleTableLength + (int)(b1->center.y - b1->halfDimensionAABB.y)) / ceBoxLength;
        unsigned int right = (space->middleTableLength + (int)(b1->center.x + b1->halfDimensionAABB.x)) / ceBoxLength;
        unsigned int bottom = (space->middleTableLength + (int)(b1->center.y + b1->halfDimensionAABB.y)) / ceBoxLength;
        unsigned int left = (space->middleTableLength + (int)(b1->center.x - b1->halfDimensionAABB.x)) / ceBoxLength;
        for(unsigned int y = top; y <= bottom; y++)
            for(unsigned int x = left; x <= right; x++)
            {
                unsigned int index = y * space->tableBoxLength + x;
                for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
                {
                    ceBody* b2 = space->spatialTable[index].bodies[i];
                    if(b1->id < b2->id)
                    {
                        if(b2->stamp != b1->id)
                        {
                            (*actionBetweenBodies)(b1, b2);
                            b2->stamp = b1->id;
                        }
                    }
                }
            }
        b1 = b1->next;
    }
}

void _ceCollideBodiesAtRing(ceSpace* space, unsigned int internalRing, unsigned int externalRing)
{
    unsigned int internalRingExpand = (internalRing + 2 < (int)space->ringsNumber) ? internalRing + 2 : internalRing + 1;
    unsigned int externalRingExpand = externalRing;

    int internalBody = space->ringsIndicator[internalRingExpand] - 1;
    int externalBody = space->ringsIndicator[externalRingExpand];
    for(int i = internalBody; i >= externalBody; i--)
    {
        ceBody* b1 = space->ringsBuffer[i];
        unsigned int top = (space->middleTableLength + (int)(b1->center.y - b1->halfDimensionAABB.y)) / ceBoxLength;
        unsigned int right = (space->middleTableLength + (int)(b1->center.x + b1->halfDimensionAABB.x)) / ceBoxLength;
        unsigned int bottom = (space->middleTableLength + (int)(b1->center.y + b1->halfDimensionAABB.y)) / ceBoxLength;
        unsigned int left = (space->middleTableLength + (int)(b1->center.x - b1->halfDimensionAABB.x)) / ceBoxLength;
        for(unsigned int y = top; y <= bottom; y++)
            for(unsigned int x = left; x <= right; x++)
            {
                unsigned int index = y * space->tableBoxLength + x;
                for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
                {
                    ceBody* b2 = space->spatialTable[index].bodies[i];
                    if(b1->id < b2->id)
                    {
                        if(b2->stamp != b1->id)
                        {
                            if(b2->pressure >= externalRing)
                                _ceCreateForcesOutward(b1, b2, internalRing, externalRing);
                            b2->stamp = b1->id;
                        }
                    }
                }
            }
    }
}

void _ceFindEdges(ceSpace* space)
{
    for(unsigned int i = 0; i < space->tableBoxLength * space->tableBoxLength; i++)
    {
        unsigned int size = space->spatialTable[i].size;
        if(size <= (ceMaxBodiesPerBox) / _CE_ACCURACY_EDGE)
        {
            for(unsigned int b = 0; b < size; b++)
            {
                ceBody* body = space->spatialTable[i].bodies[b];
                if(body->pressure == _CE_PRESSURE_CLEAR)
                {
                    body->pressure = 0;
                    space->ringsBuffer[space->ringsBufferSize++] = body;
                }
            }
        }
    }
}

void _ceFindRings(ceSpace* space)
{
    if(space->ringsBufferSize > 0)
    {
        space->ringsNumber = 1;
        space->ringsIndicator[0] = 0;

        for(unsigned int i = 0; i < space->ringsBufferSize; i++)
        {
            ceBody* body = space->ringsBuffer[i];

            if(body->pressure == space->ringsNumber - 1)
                space->ringsIndicator[space->ringsNumber++] = space->ringsBufferSize;

            for(unsigned int c = 0; c < body->auraContactsSize; c++)
            {
                ceBody* nextBody = body->auraContacts[c].body;
                if(nextBody->pressure == _CE_PRESSURE_CLEAR)
                {
                    nextBody->pressure = body->pressure + 1;
                    space->ringsBuffer[space->ringsBufferSize++] = nextBody;
                }
            }
        }

        space->ringsNumber--;
    }

    /* Bodies not detected. */
    ceBody* body = space->firstBody;
    while(body != NULL)
    {
        if(body->pressure == _CE_PRESSURE_CLEAR)
            space->ringsBuffer[space->ringsBufferSize++] = body;

        body = body->next;
    }

    space->ringsIndicator[space->ringsNumber + 1] = space->populationSize;
}

void _ceExpandRing(ceSpace* space, unsigned int ring)
{
    for(int i = space->ringsIndicator[ring + 1] - 1; i >= (int)space->ringsIndicator[ring]; i--)
    {
        ceBody* body = space->ringsBuffer[i];
        ceVector2 totalDisplacement = ceGetVector2(0, 0);
        unsigned int accumulator = 0;
        for(unsigned int c = 0; c < body->auraContactsSize; c++)
        {
            ceBody* closeBody = body->auraContacts[c].body;
            if (closeBody->pressure >= body->pressure)
            {
                ceVector2 displacement = ceGetSubVectors2(closeBody->center, closeBody->centerPreStep);
                if(displacement.x != 0 || displacement.y != 0)
                {
                    totalDisplacement = ceGetAddVectors2(totalDisplacement, displacement);
                    accumulator++;
                }
            }
            for(unsigned int c2 = 0; c2 < closeBody->auraContactsSize; c2++)
            {
                ceBody* closeBody2 = closeBody->auraContacts[c2].body;
                if (closeBody2->pressure >= body->pressure)
                {
                    ceVector2 displacement2 = ceGetSubVectors2(closeBody2->center, closeBody2->centerPreStep);
                    if(displacement2.x != 0 || displacement2.y != 0)
                    {
                        totalDisplacement = ceGetAddVectors2(totalDisplacement, displacement2);
                        accumulator++;
                    }
                }
            }
        }
        if(accumulator > 0)
            _ceMoveBody(body, ceGetDivVector2ByScalar(totalDisplacement, accumulator));
    }
}

//Optimize
void _ceRelaxBodiesNotDetected(ceSpace* space)
{
    for(unsigned int i = space->ringsIndicator[space->ringsNumber]; i < space->populationSize; i++)
    {
        ceBody* body = space->ringsBuffer[i];
        body->stamp = _CE_STAMP_CLEAR;
        _ceResetForces(body);
        _ceComputeAABB(body);
        _ceInsertBodyInSpatialTable(space, body);
    }

    for(unsigned int i = space->ringsIndicator[space->ringsNumber]; i < space->populationSize; i++)
    {
        ceBody* b1 = space->ringsBuffer[i];
        unsigned int top = (space->middleTableLength + (int)(b1->center.y - b1->halfDimensionAABB.y)) / ceBoxLength;
        unsigned int right = (space->middleTableLength + (int)(b1->center.x + b1->halfDimensionAABB.x)) / ceBoxLength;
        unsigned int bottom = (space->middleTableLength + (int)(b1->center.y + b1->halfDimensionAABB.y)) / ceBoxLength;
        unsigned int left = (space->middleTableLength + (int)(b1->center.x - b1->halfDimensionAABB.x)) / ceBoxLength;
        for(unsigned int y = top; y <= bottom; y++)
            for(unsigned int x = left; x <= right; x++)
            {
                unsigned int index = y * space->tableBoxLength + x;
                for(unsigned int i = 0; i < space->spatialTable[index].size; i++)
                {
                    ceBody* b2 = space->spatialTable[index].bodies[i];
                    if(b1->id < b2->id)
                    {
                        if(b2->stamp != b1->id)
                        {
                            _ceCreateForcesOutward(b1, b2, _CE_PRESSURE_CLEAR, _CE_PRESSURE_CLEAR);
                            b2->stamp = b1->id;
                        }
                    }
                }
            }
    }

    for(unsigned int i = space->ringsIndicator[space->ringsNumber]; i < space->populationSize; i++)
        _cePushBody(space->ringsBuffer[i]);
}

void _ceRelaxRings(ceSpace* space, unsigned int internalRing, unsigned int externalRing, unsigned int physicsIterations)
{
    int internalBody = space->ringsIndicator[internalRing + 1] - 1;
    int externalBody = space->ringsIndicator[externalRing];

    for(unsigned int it = 0; it < physicsIterations; it++)
    {

        _ceClearSpatialTableByPressure(space, internalRing);

        for(int i = internalBody; i >= externalBody; i--)
        {
            ceBody* body = space->ringsBuffer[i];
            body->stamp = _CE_STAMP_CLEAR;
            _ceResetForces(body);
            _ceComputeAABB(body);
            _ceInsertBodyInSpatialTable(space, body);
        }

        _ceCollideBodiesAtRing(space, internalRing, externalRing);

        for(int i = internalBody; i >= externalBody; i--)
            _cePushBody(space->ringsBuffer[i]);
    }

    /* only bodies at ring with value "internalRing". */
    int internalBodyOut = space->ringsIndicator[internalRing] - 1;
    for(int i = internalBody; i >= internalBodyOut; i--)
    {
        ceBody* body = space->ringsBuffer[i];

        body->stamp = _CE_STAMP_CLEAR;

        _ceComputeAABB(body);
        _ceInsertBodyInSpatialTable(space, body);
    }
}

void _ceRelaxEntirePopulation(ceSpace* space, unsigned int physicsIterations)
{
    for(unsigned int it = 0; it < physicsIterations; it++)
    {
        _ceClearSpatialTable(space);

        ceBody* body = space->firstBody;
        while(body != NULL)
        {
            body->stamp = _CE_STAMP_CLEAR;
            _ceResetForces(body);

            _ceComputeAABB(body);
            _ceInsertBodyInSpatialTable(space, body);

            body = body->next;
        }

        /* Calculate all colony forces. */
        _ceTotalBodiesAction(space, _ceCreateForces);

        /* Move all bodies by the forces previously calculated. */
        body = space->firstBody;
        while(body != NULL)
        {
            /* Move body and rotate Body. */
            _cePushBody(body);

            body = body->next;
        }
    }
}

//**************************************************************************
//                              Debug functions
//**************************************************************************
int* _ceDebugFindLimits(ceSpace* space, unsigned int* size)
{
    unsigned int limit = space->limit;
    unsigned int limit2 = space->tableBoxLength - limit;

    unsigned int p = 0;
    *size = (space->tableBoxLength - 2 * limit - 1) * 8;
    int* limits = (int*) malloc(sizeof(int) * *size);

    /* Top limit line. */
    for(unsigned int x = limit; x < limit2; x++)
    {
        limits[p++] = x;
        limits[p++] = limit;
    }

    /* Left and right limit lines. */
    for(unsigned int y = limit + 1; y < limit2 - 1; y++)
    {
        limits[p++] = limit;
        limits[p++] = y;

        limits[p++] = limit2 - 1;
        limits[p++] = y;
    }

    /* Bottom limit line. */
    for(unsigned int x = limit; x < limit2; x++)
    {
        limits[p++] = x;
        limits[p++] = limit2 - 1;
    }

    return limits;
}
