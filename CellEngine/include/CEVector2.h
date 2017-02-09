#ifndef CEVECTOR2_HEADER
#define CEVECTOR2_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CE_EPSILON 1E-6f
#define CE_PI 3.141592653589
#define CE_RADIAN 57.2957795131

/* Struct to represent a mathematical vector in 2 dimension. */
typedef struct ceVector2
{
    float x;
    float y;
} ceVector2;

/* Create a vector in the stack memory. All functions to manage the ceVector2
treat the vector like a couple of values in the stack memory. The compiler with
optimization enabled must be able to put inline all those functions and transform
them inside of the called function for the best performance. */
static inline ceVector2 ceGetVector2(float x, float y);

/* Returns if two vectors are equals. */
static inline int ceAreEqualsVectors2(const ceVector2 v1, const ceVector2 v2);

/* Returns the inverted version of the vector given. */
static inline ceVector2 ceGetInvertedVector2(const ceVector2 v);

/* Returns the unitary version of the vector given. */
static inline ceVector2 ceGetUnitaryVector2(const ceVector2 v);

/* Returns the unitary version of the vector given. */
static inline ceVector2 ceGetOrthogonalVector2(const ceVector2 v);

/* Returns the length or module of the vector given. */
static inline float ceGetLengthVector2(const ceVector2 v);

/* Returns the square length or square module of the vector given. */
static inline float ceGetSquareLengthVector2(const ceVector2 v);

/* Returns the addition between the two vectors given. */
static inline ceVector2 ceGetAddVectors2(const ceVector2 v1, const ceVector2 v2);

/* Returns the substraction between the two vectors given. */
static inline ceVector2 ceGetSubVectors2(const ceVector2 v1, const ceVector2 v2);

/* Returns the multiplied vector between the given one and the parameter scalar. */
static inline ceVector2 ceGetMulVector2ByScalar(const ceVector2 v, float scalar);

/* Returns the multiplied vector given by the multiplication of their components. */
static inline ceVector2 ceGetComponentProductVector2(const ceVector2 v1, const ceVector2 v2);

/* Returns the multiplied vector given by the dot or scalar multiplication. */
static inline float ceGetDotProductVector2(const ceVector2 v1, const ceVector2 v2);

/* Returns the multiplied vector given by the cross multiplication. */
static inline float ceGetCrossProductVector2(const ceVector2 v1, const ceVector2 v2);

/* Returns the divided vector between the given one and the parameter scalar. */
static inline ceVector2 ceGetDivVector2ByScalar(const ceVector2 v, float scalar);

/* Returns the projection of v1 over v2. */
static inline float ceGetProjectionBetweenVectors2(const ceVector2 v1, const ceVector2 v2);

/* Returns the minimum angle formed by v1 and v2. */
static inline float ceGetAngleBetweenVectors2(const ceVector2 v1, const ceVector2 v2);

/* Returns a rotated vector by radians like a point from the 0, 0 point. */
static inline ceVector2 ceGetRotatedVector2(const ceVector2 v, float radians);

/* Returns a rotated vector by radians like a point from another point given. */
static inline ceVector2 ceGetRotatedVector2FromCenter(const ceVector2 v, float radians, const ceVector2 center);

/* Write the values of the vector into the string given. It is necessary
reserve minimum 20 bytes in the string for writting the vector inside of it.
The function returns the pointer to the next position of the last character
written. */
static inline char* ceWriteVector2(const ceVector2 v, char* string);

//****************************************************************************
//                          Inline implementations
//****************************************************************************

ceVector2 ceGetVector2(float x, float y)
{
    ceVector2 v = {x, y};
    return v;
}

int ceAreEqualsVectors2(const ceVector2 v1, const ceVector2 v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}

ceVector2 ceGetInvertedVector2(const ceVector2 v)
{
    return ceGetVector2(-v.x, -v.y);
}

ceVector2 ceGetUnitaryVector2(const ceVector2 v)
{
    float length = ceGetLengthVector2(v);
    if(length > CE_EPSILON)
        return ceGetMulVector2ByScalar(v, 1.0f / length);
    return v;
}

ceVector2 ceGetOrthogonalVector2(const ceVector2 v)
{
    return ceGetVector2(-v.y, v.x);
}

float ceGetLengthVector2(const ceVector2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

float ceGetSquareLengthVector2(const ceVector2 v)
{
    return v.x * v.x + v.y * v.y;
}

ceVector2 ceGetAddVectors2(const ceVector2 v1, const ceVector2 v2)
{
    return ceGetVector2(v1.x + v2.x, v1.y + v2.y);
}

ceVector2 ceGetSubVectors2(const ceVector2 v1, const ceVector2 v2)
{
    return ceGetVector2(v1.x - v2.x, v1.y - v2.y);
}

ceVector2 ceGetMulVector2ByScalar(const ceVector2 v, float scalar)
{
    return ceGetVector2(v.x * scalar, v.y * scalar);
}

ceVector2 ceGetComponentProductVector2(const ceVector2 v1, const ceVector2 v2)
{
    return ceGetVector2(v1.x * v2.x, v1.y * v2.y);
}

float ceGetDotProductVector2(const ceVector2 v1, const ceVector2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float ceGetCrossProductVector2(const ceVector2 v1, const ceVector2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

ceVector2 ceGetDivVector2ByScalar(const ceVector2 v, float scalar)
{
    return ceGetMulVector2ByScalar(v, 1.0f / scalar);
}

float ceGetProjectionBetweenVectors2(const ceVector2 v1, const ceVector2 v2)
{
    return ceGetDotProductVector2(v1, v2) / ceGetLengthVector2(v2);
}


float ceGetAngleBetweenVectors2(const ceVector2 v1, const ceVector2 v2)
{
    return acosf(ceGetDotProductVector2(v1, v2) / (ceGetLengthVector2(v1) * ceGetLengthVector2(v2)));
}

ceVector2 ceGetRotatedVector2(const ceVector2 v, float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);

    return ceGetVector2(v.x * c - v.y * s, v.x * s + v.y * c);
}

ceVector2 ceGetRotatedVector2FromCenter(const ceVector2 v, float radians, const ceVector2 center)
{
    ceVector2 aux = ceGetSubVectors2(v, center);
    return ceGetAddVectors2(center, ceGetRotatedVector2(aux, radians));
}

char* ceWriteVector2(const ceVector2 v, char* string)
{

    return string + sprintf(string, "(%.6f, %.6f)", v.x, v.y);
}

#ifdef __cplusplus
}
#endif

#endif //CEVECTOR2_HEADER