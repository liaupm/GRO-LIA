#include "CellEngine.h"
#include "private/CellEngine_p.h"

#include "CEVector2.h"

unsigned int ceCombinedRing;
unsigned int cePhysicsIterationsPerCombinedRing;
unsigned int cePhysicsIterationsWithoutRingSystem;

unsigned int ceMaxBodiesPerBox;
unsigned int ceMaxAuraContactsPerBody;
unsigned short ceBoxLength;

float ceBodyWidth;
float ceMiddleBodyWidth;
float ceBodySquareWidth;
float ceParallelForceRectification;

float ceThrustRelation;

void ceInit(unsigned int combinedRing, unsigned int physicsIterationsPerCombinedRing,
    float maxBodyLength, float minBodyLength, float bodyWidth)
{
    ceCombinedRing = combinedRing;
    cePhysicsIterationsPerCombinedRing = physicsIterationsPerCombinedRing;
    cePhysicsIterationsWithoutRingSystem = physicsIterationsPerCombinedRing * ceCombinedRing * ceCombinedRing / 4;

    ceBoxLength = (maxBodyLength + minBodyLength) * 0.5f;
    ceMaxBodiesPerBox = ((ceBoxLength / bodyWidth) + 2.0f) * ((ceBoxLength / minBodyLength) + 2.0f) * 4;
    ceMaxAuraContactsPerBody = (6 + (unsigned int) 2.0f * ((maxBodyLength - (bodyWidth * 2)) / (bodyWidth * 2) + 1.0f)) * 2;

    ceBodyWidth = bodyWidth;
    ceMiddleBodyWidth = bodyWidth * 0.5f;
    ceBodySquareWidth = bodyWidth * bodyWidth;
    ceParallelForceRectification = (maxBodyLength + minBodyLength) / (bodyWidth * 3);

    ceThrustRelation = _CE_THRUST_RELATION;
}