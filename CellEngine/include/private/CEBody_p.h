#ifndef CEBODY_PRIVATE_HEADER
#define CEBODY_PRIVATE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "CEBody.h"
#include "CESpace.h"

static inline void _ceMoveBody(ceBody* body, ceVector2 displacement);
void _cePushBody(ceBody* body);

static inline void _ceAddForce(ceBody* body, ceVector2 force, ceVector2 contactPoint);
static inline void _ceAddTorque(ceBody* body, ceVector2 force, ceVector2 contactPoint);

static inline void _ceResetForces(ceBody* body);

void _ceFilterForcesByDensity(ceBody* body);

static inline void _ceComputeAABB(ceBody* body);
static inline void _ceComputeAABBAura(ceBody* body);
static inline int _ceCollideAABBs(const ceBody* b1, const ceBody* b2);
static inline int _ceCollideAABBAuras(const ceBody* b1, const ceBody* b2);

void _ceDistanceBetweenSegments(ceVector2 s1p1, ceVector2 s1p2, ceVector2 s2p1, ceVector2 s2p2, ceVector2* c1, ceVector2* c2);

void _ceCreateContact(ceBody* b1, ceBody* b2);
static inline void _ceCreateForces(ceBody* b1, ceBody* b2);
void _ceCreateForcesOutward(ceBody* b1, ceBody* b2, unsigned int internal, unsigned int external);

//****************************************************************************
//                          Inline implementations
//****************************************************************************

void _ceMoveBody(ceBody* body, ceVector2 displacement)
{
    body->center = ceGetAddVectors2(body->center, displacement);
    body->p1 = ceGetAddVectors2(body->p1, displacement);
    body->p2 = ceGetAddVectors2(body->p2, displacement);
}

void _ceAddForce(ceBody* body, ceVector2 force, ceVector2 contactPoint)
{
    body->totalForce = ceGetAddVectors2(body->totalForce, force);
    _ceAddTorque(body, force, contactPoint);
}

void _ceAddTorque(ceBody* body, ceVector2 force, ceVector2 contactPoint)
{
    ceVector2 centerToContact = ceGetSubVectors2(contactPoint, body->center);
    float centerToContactSquare = ceGetSquareLengthVector2(centerToContact);
    if(centerToContactSquare >= CE_EPSILON)
        body->totalTorque += ceGetCrossProductVector2(centerToContact, force) / centerToContactSquare;
}

void _ceResetForces(ceBody* body)
{
    body->totalForce = ceGetVector2(0.0f, 0.0f);
    body->totalTorque = 0.0f;
}

void _ceComputeAABB(ceBody* body)
{
    body->halfDimensionAABB.x = (fabs(body->p1.x - body->p2.x) + ceBodyWidth) * 0.5f;
    body->halfDimensionAABB.y = (fabs(body->p1.y - body->p2.y) + ceBodyWidth) * 0.5f;
}

void _ceComputeAABBAura(ceBody* body)
{
    body->halfDimensionAABBAura.x = ceMiddleBodyWidth + (fabs(body->p1.x - body->p2.x) + ceBodyWidth) * 0.5f;
    body->halfDimensionAABBAura.y = ceMiddleBodyWidth + (fabs(body->p1.y - body->p2.y) + ceBodyWidth) * 0.5f;
}

int _ceCollideAABBs(const ceBody* b1, const ceBody* b2)
{
    if(fabs(b2->center.x - b1->center.x) < b1->halfDimensionAABB.x + b2->halfDimensionAABB.x)
        if(fabs(b2->center.y - b1->center.y) < b1->halfDimensionAABB.y + b2->halfDimensionAABB.y)
            return 1;
    return 0;
}

int _ceCollideAABBAuras(const ceBody* b1, const ceBody* b2)
{
    if(fabs(b2->center.x - b1->center.x) < b1->halfDimensionAABBAura.x + b2->halfDimensionAABBAura.x)
        if(fabs(b2->center.y - b1->center.y) < b1->halfDimensionAABBAura.y + b2->halfDimensionAABBAura.y)
            return 1;
    return 0;
}

void _ceCreateForces(ceBody* b1, ceBody* b2)
{
    _ceCreateForcesOutward(b1, b2, b1->space->ringsNumber - 1, 0);
}

#ifdef __cplusplus
}
#endif

#endif //CEBODY_PRIVATE_HEADER