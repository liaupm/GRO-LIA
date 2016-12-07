#ifndef CELLENGINE_PRIVATE_HEADER
#define CELLENGINE_PRIVATE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

//**************************************************************************
//                         Internal CellEngine defines
//**************************************************************************

#define _CE_DEFAULT_LIMIT_VALUE 0.02f
#define _CE_INITIAL_TABLE_BOX_LENGTH 10
#define _CE_RESIZE_MULTIPLIER 1.5f
#define _CE_STAMP_CLEAR -1
#define _CE_STAMP_SELECTED -2
#define _CE_PRESSURE_CLEAR -1
#define _CE_ACCURACY_EDGE 20
#define _CE_INITIAL_RINGS_INDICATOR_SIZE 10000
#define _CE_GLOBAL_FORCE_COEFFICIENT 0.5f
#define _CE_THRUST_RELATION 0.875f

//**************************************************************************
//                             Global variables
//**************************************************************************

extern unsigned int ceCombinedRing;
extern unsigned int cePhysicsIterationsPerCombinedRing;
extern unsigned int cePhysicsIterationsWithoutRingSystem;

extern unsigned int ceMaxBodiesPerBox;
extern unsigned int ceMaxAuraContactsPerBody;
extern unsigned short ceBoxLength;

extern float ceBodyWidth;
extern float ceMiddleBodyWidth;
extern float ceBodySquareWidth;

extern float ceParallelForceRectification;

extern float ceThrustRelation;

//**************************************************************************
//                                  Utils
//**************************************************************************

static inline float _ceClampToUnit(float value);

//****************************************************************************
//                          Inline general implementations
//****************************************************************************

float _ceClampToUnit(float value)
{
    if(value < 0.0f)
        return 0.0f;
    if(value > 1.0f)
        return 1.0f;
    return value;
}

#ifdef __cplusplus
}
#endif

#endif //CELLENGINE_PRIVATE_HEADER