#include "private/CellEngine_p.h"
#include "CEBody.h"
#include "private/CEBody_p.h"
#include "CESpace.h"
#include "private/CESpace_p.h"

//**************************************************************************
//                              Public functions
//**************************************************************************

ceBody* ceCreateBody(ceSpace* space, float length, ceVector2 center, float rotation)
{
    ceBody* body = (ceBody*) malloc(sizeof(ceBody));

    body->center = center;
    body->length = length;
    body->rotation = rotation;

    body->unitary = ceGetVector2(cosf(rotation), sinf(rotation));
    ceVector2 centerToPoint = ceGetMulVector2ByScalar(body->unitary, (length - ceBodyWidth) * 0.5f);
    body->p1 = ceGetSubVectors2(body->center, centerToPoint);
    body->p2 = ceGetAddVectors2(body->center, centerToPoint);

    body->auraContacts = (ceContact*) malloc(sizeof(ceContact) * ceMaxAuraContactsPerBody);
    body->auraContactsSize = 0;

    _ceCheckAndModifySpatialTable(space, body);
    _ceAddBodyToSpace(space, body);

    return body;
}

void ceDestroyBody(ceBody* body)
{
    if(body->previous)
        body->previous->next = body->next;
    if(body->next)
        body->next->previous = body->previous;

    if(body == body->space->firstBody)
        body->space->firstBody = body->next;
    if(body == body->space->lastBody)
        body->space->lastBody = body->previous;

    body->space->populationSize--;
    free(body->auraContacts);
    free(body);
}

void ceMoveBodyToSpace(ceBody* body, ceSpace* newSpace)
{
    if(body->previous)
        body->previous->next = body->next;
    if(body->next)
        body->next->previous = body->previous;

    if(body == body->space->firstBody)
        body->space->firstBody = body->next;
    if(body == body->space->lastBody)
        body->space->lastBody = body->previous;

    body->space->populationSize--;
    _ceCheckAndModifySpatialTable(newSpace, body);
    _ceAddBodyToSpace(newSpace, body);
}

ceBody* ceDivideBody(ceBody* body, float rotation, float lengthProportion)
{

    ceBody* child = (ceBody*) malloc(sizeof(ceBody));

    child->length = body->length * lengthProportion;
    body->length = body->length - child->length;

    /* parent */
    ceVector2 bodyCenterToPoints = ceGetMulVector2ByScalar(body->unitary, (body->length - ceBodyWidth) * 0.5f);
    body->center = ceGetAddVectors2(body->p1, bodyCenterToPoints);
    body->p2 = ceGetAddVectors2(body->center, bodyCenterToPoints);

    /* child */
    child->rotation = body->rotation + rotation;
    child->unitary = ceGetVector2(cosf(child->rotation), sinf(child->rotation));
    ceVector2 bodycenterTochildCenter = ceGetMulVector2ByScalar(body->unitary, (child->length + body->length) * 0.5f);
    child->center = ceGetAddVectors2(body->center, bodycenterTochildCenter);

    ceVector2 childCenterToChildPoint = ceGetMulVector2ByScalar(child->unitary, (child->length - ceBodyWidth) * 0.5f);
    child->p1 = ceGetSubVectors2(child->center, childCenterToChildPoint);
    child->p2 = ceGetAddVectors2(child->center, childCenterToChildPoint);

    child->auraContacts = (ceContact*) malloc(sizeof(ceContact) * ceMaxAuraContactsPerBody);
    child->auraContactsSize = 0;

    _ceAddBodyToSpace(body->space, child);

    return child;
}

void ceMoveBody(ceBody* body, ceVector2 displacement)
{
    body->center = ceGetAddVectors2(body->center, displacement);
    body->p1 = ceGetAddVectors2(body->p1, displacement);
    body->p2 = ceGetAddVectors2(body->p2, displacement);
    _ceCheckAndModifySpatialTable(body->space, body);
}

void ceRotateBody(ceBody* body, float rotation)
{
    body->rotation += rotation;
    body->unitary = ceGetVector2(cosf(body->rotation), sinf(body->rotation));
    ceVector2 centerToPoint = ceGetMulVector2ByScalar(body->unitary, (body->length - ceBodyWidth) * 0.5f);
    body->p1 = ceGetSubVectors2(body->center, centerToPoint);
    body->p2 = ceGetAddVectors2(body->center, centerToPoint);
}

int ceTestBodyPoint(const ceBody* body, ceVector2 point)
{
    ceVector2 ap = ceGetSubVectors2(point, body->p1);
    ceVector2 bp = ceGetSubVectors2(point, body->p2);
    ceVector2 ab = ceGetSubVectors2(body->p2, body->p1);

    float squareDistance;
    float projection = ceGetDotProductVector2(ap, ab);
    if(projection <= 0.0f)
        squareDistance = ceGetDotProductVector2(ap, ap);
    else
    {
        float totalProjection = ceGetDotProductVector2(ab, ab);
        if(projection >= totalProjection)
            squareDistance = ceGetDotProductVector2(bp, bp);
        else
            squareDistance = ceGetDotProductVector2(ap, ap) - (projection * projection / totalProjection);
    }

    return squareDistance <= ceBodySquareWidth * 0.25f;
}

int ceTestBodySegment(const ceBody* b1, ceVector2 segP1, ceVector2 segP2)
{
    ceVector2 c1;
    ceVector2 c2;
    _ceDistanceBetweenSegments(b1->p1, b1->p2, segP1, segP2, &c1, &c2);
    ceVector2 distance = ceGetSubVectors2(c1, c2);
    float squareDistance = ceGetSquareLengthVector2(distance);
    return squareDistance <= ceBodySquareWidth * 0.25f;
}

int ceTestBodyBody(const ceBody* b1, const ceBody* b2)
{
    ceVector2 c1;
    ceVector2 c2;
    _ceDistanceBetweenSegments(b1->p1, b1->p2, b2->p1, b2->p2, &c1, &c2);
    ceVector2 distance = ceGetSubVectors2(c1, c2);
    float squareDistance = ceGetSquareLengthVector2(distance);
    return squareDistance <= ceBodySquareWidth;
}

ceBody** ceGetNearBodies(const ceBody* body, float distance, unsigned int* size)
{
    ceBody** list = (ceBody**) malloc(sizeof(ceBody*) * body->auraContactsSize);
    *size = 0;
    for(unsigned int i = 0; i < body->auraContactsSize; i++)
    {
        if(sqrtf(body->auraContacts[i].squareDistance) < distance + ceBodyWidth)
            list[(*size)++] = body->auraContacts[i].body;
    }
    if(*size == 0)
    {
        free(list);
        return NULL;
    }
    return list;
}

//**************************************************************************
//                             Private functions
//**************************************************************************

void _ceFilterForcesByDensity(ceBody* body)
{
    ceSpace* space = body->space;
    unsigned int top = (space->middleTableLength + (int)(body->center.y - body->halfDimensionAABB.y)) / ceBoxLength;
    unsigned int right = (space->middleTableLength + (int)(body->center.x + body->halfDimensionAABB.x)) / ceBoxLength;
    unsigned int bottom = (space->middleTableLength + (int)(body->center.y + body->halfDimensionAABB.y)) / ceBoxLength;
    unsigned int left = (space->middleTableLength + (int)(body->center.x - body->halfDimensionAABB.x)) / ceBoxLength;
    for(unsigned int y = top; y <= bottom; y++)
        for(unsigned int x = left; x <= right; x++)
        {
            unsigned int index = y * space->tableBoxLength + x;
            /* 6 is a heuristic result. Change this value to modify the global overlaping. */
            if(space->spatialTable[index].size < 6)
            {
                body->totalForce = ceGetMulVector2ByScalar(body->totalForce, 0.5f);
                body->totalTorque *= 0.5f;
            }
        }
}

void _cePushBody(ceBody* body)
{
    _ceFilterForcesByDensity(body);

    if(body->pressure == 0)
        body->totalTorque *= 10.0f;

    ceVector2 displacement = ceGetMulVector2ByScalar(body->totalForce, _CE_GLOBAL_FORCE_COEFFICIENT * ceThrustRelation);
    float rotation = body->totalTorque * _CE_GLOBAL_FORCE_COEFFICIENT * (1.0f - ceThrustRelation);

    body->rotation += rotation;
    body->unitary = ceGetVector2(cosf(body->rotation), sinf(body->rotation));
    ceVector2 centerToPoint = ceGetMulVector2ByScalar(body->unitary, (body->length - ceBodyWidth) * 0.5f);
    body->center = ceGetAddVectors2(body->center, displacement);
    body->p1 = ceGetSubVectors2(body->center, centerToPoint);
    body->p2 = ceGetAddVectors2(body->center, centerToPoint);
}

void _ceDistanceBetweenSegments(ceVector2 s1p1, ceVector2 s1p2, ceVector2 s2p1, ceVector2 s2p2, ceVector2* c1, ceVector2* c2)
{
    ceVector2 d1 = ceGetSubVectors2(s1p1, s1p2);
    ceVector2 d2 = ceGetSubVectors2(s2p1, s2p2);
    ceVector2 r = ceGetSubVectors2(s1p2, s2p2);

    float a = ceGetDotProductVector2(d1, d1);
    float b = ceGetDotProductVector2(d1, d2);
    float e = ceGetDotProductVector2(d2, d2);
    float f = ceGetDotProductVector2(d2, r);

    float t;
    float s;

    float denom = a * e - b * b;
    if(denom != 0.0f)
    {
        float c = ceGetDotProductVector2(d1, r);
        s = _ceClampToUnit((b * f - c * e) / denom);
    }
    else
        s = 0.0f;

    float tnom = b * s + f;
    if(tnom < 0.0f)
    {
        t = 0.0f;
        float c = ceGetDotProductVector2(d1, r);
        s = _ceClampToUnit(-c / a);
    }
    else if (tnom > e)
    {
        t = 1.0f;
        float c = ceGetDotProductVector2(d1, r);
        s = _ceClampToUnit((b - c) / a);
    }
    else
        t = tnom / e;

    *c1 = ceGetAddVectors2(s1p2, ceGetMulVector2ByScalar(d1, s));
    *c2 = ceGetAddVectors2(s2p2, ceGetMulVector2ByScalar(d2, t));
}

void _ceCreateContact(ceBody* b1, ceBody* b2)
{
    if(!_ceCollideAABBAuras(b1, b2))
        return;

    ceVector2 c1;
    ceVector2 c2;
    _ceDistanceBetweenSegments(b1->p1, b1->p2, b2->p1, b2->p2, &c1, &c2);
    ceVector2 vDistance = ceGetSubVectors2(c1, c2);
    float squareDistance = ceGetSquareLengthVector2(vDistance);

    if(squareDistance <= ceBodySquareWidth * 4.0f)
    {
        if(b1->auraContactsSize < ceMaxAuraContactsPerBody && b2->auraContactsSize < ceMaxAuraContactsPerBody)
        {
            b1->auraContacts[b1->auraContactsSize].body = b2;
            b1->auraContacts[b1->auraContactsSize++].squareDistance = squareDistance;

            b2->auraContacts[b2->auraContactsSize].body = b1;
            b2->auraContacts[b2->auraContactsSize++].squareDistance = squareDistance;
        }
    }
}

void _ceCreateForcesOutward(ceBody* b1, ceBody* b2, unsigned int internal, unsigned int external)
{
    if(!_ceCollideAABBs(b1, b2))
        return;

    ceVector2 c1;
    ceVector2 c2;
    _ceDistanceBetweenSegments(b1->p1, b1->p2, b2->p1, b2->p2, &c1, &c2);
    ceVector2 vDistance = ceGetSubVectors2(c1, c2);
    float squareDistance = ceGetSquareLengthVector2(vDistance);

    if(squareDistance <= ceBodySquareWidth)
    {
        float distance = sqrtf(squareDistance);
        ceVector2 contactPoint = ceGetAddVectors2(c2, ceGetMulVector2ByScalar(vDistance, 0.5f));
        ceVector2 vDistanceUnitary = ceGetUnitaryVector2(vDistance);

        float lengthForceToB1 = 0;
        float lengthForceToB2 = 0;

        if(b1->pressure > internal)
            lengthForceToB2 = distance - ceBodyWidth;
        else if(b2->pressure > internal)
            lengthForceToB1 = ceBodyWidth - distance;
        else if(b1->pressure >= external && b2->pressure >= external)
        {
            lengthForceToB1 = (ceBodyWidth - distance) * 0.5f;
            lengthForceToB2 = -lengthForceToB1;
        }

        if(fabs(ceGetDotProductVector2(b1->unitary, b2->unitary)) > 0.95f && fabs(ceGetDotProductVector2(vDistanceUnitary, b2->unitary)) < 0.05f)
        {
            lengthForceToB1 *= ceParallelForceRectification;
            lengthForceToB2 *= ceParallelForceRectification;
        }

        ceVector2 forceToB1 = ceGetMulVector2ByScalar(vDistanceUnitary, lengthForceToB1);
        ceVector2 forceToB2 = ceGetMulVector2ByScalar(vDistanceUnitary, lengthForceToB2);

        _ceAddForce(b1, forceToB1, contactPoint);
        _ceAddForce(b2, forceToB2, contactPoint);
    }
}