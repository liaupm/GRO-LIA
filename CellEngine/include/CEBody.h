#ifndef CEBODY_HEADER
#define CEBODY_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "CEVector2.h"

#include <stdlib.h>
#include <math.h>

typedef struct ceSpace ceSpace;
typedef struct ceBody ceBody;

/* Struct to contains information about the neighborhood. */
typedef struct ceContact
{
    ceBody* body;
    float squareDistance;
} ceContact;

/* Struct to contains the memory of a body. */
struct ceBody
{
    ceVector2 center;
    float length;
    float rotation;

    ceVector2 p1;
    ceVector2 p2;
    ceVector2 unitary;

    ceVector2 halfDimensionAABB;
    ceVector2 halfDimensionAABBAura;

    ceContact* auraContacts;
    unsigned int auraContactsSize;

    unsigned int pressure;
    ceVector2 centerPreStep;

    ceSpace* space;
    ceBody* previous;
    ceBody* next;

    unsigned int id;
    unsigned int stamp;

    ceVector2 totalForce;
    float totalTorque;

    void* data;
};

/* Create a body inside of a space and return its reference.
The width of the body depends of the value introduced in ceInit()
function. */
ceBody* ceCreateBody(ceSpace* space, float length, ceVector2 center, float rotation);

/* Destroy a body and dettach it from its associated space. */
void ceDestroyBody(ceBody* body);

/* Change the body to another space. */
void ceMoveBodyToSpace(ceBody* body, ceSpace* newSpace);

/* Grows the body by the parameter value. */
static inline void ceGrowBody(ceBody* body, float value);

/* Divide a body. The rotation parameter is the angular displacement
between the body and its child, meassured in degrees. The lengthProportion
paramenter is the proportion of the length that the child will have,
the value goes from 0 to 1. IMPORTANT: The returned body is in the space list,
but it is not a physical body of this space until the next call to ceStep.
For example, call to ceGetNearBodies function will return zero bodies although
the body have bodies next to it. */
ceBody* ceDivideBody(ceBody* body, float rotation, float lengthProportion);

/* Set the position of the body by the position parameter. */
static inline void ceSetBodyPosition(ceBody* body, ceVector2 position);

/* Displacement the body to anywhere. */
void ceMoveBody(ceBody* body, ceVector2 displacement);

/* Rotate the body by rotation paramenter, measured in degrees. */
void ceRotateBody(ceBody* body, float rotation);

/* Attach user data to a body. */
static inline void ceSetData(ceBody* body, void* data);

/* Test if a point is inside of a body. */
int ceTestBodyPoint(const ceBody* body, ceVector2 point);

/* Test if a segment intersect with a body. */
int ceTestBodySegment(const ceBody* body, ceVector2 segP1, ceVector2 segP2);

/* Test if a body intersect with another body. */
int ceTestBodyBody(const ceBody* body1, const ceBody* body2);

/* Get other bodies around a body given by the distance parameter.
This value goes from 0 to body width. If the value is 0, only the bodies that
are overlaping will be returned. If the overlap is considerable the function could
return a body behind another one. The parameter size contains the size of the
vector returned which is necessary to free. If body is created before call ceStep
(a division for example), the body will be zero neighbors. */
ceBody** ceGetNearBodies(const ceBody* body, float distance, unsigned int* size);

//****************************************************************************
//                          Inline implementations
//****************************************************************************

void ceGrowBody(ceBody* body, float value)
{
    body->length += value;
    ceVector2 middleGrowth = ceGetMulVector2ByScalar(body->unitary, value * 0.5f);
    body->p1 = ceGetSubVectors2(body->p1, middleGrowth);
    body->p2 = ceGetAddVectors2(body->p2, middleGrowth);
}

void ceSetBodyPosition(ceBody* body, ceVector2 position)
{
    ceVector2 displacement = ceGetSubVectors2(position, body->center);
    ceMoveBody(body, displacement);
}

void ceSetData(ceBody* body, void* data)
{
    body->data = data;
}

#ifdef __cplusplus
}
#endif

#endif //CEBODY_HEADER