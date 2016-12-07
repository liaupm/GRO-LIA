#ifndef CELLENGINE_HEADER
#define CELLENGINE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "CEVector2.h"
#include "CEBody.h"
#include "CESpace.h"

/* Init CellEngine.
-The CombinedRing parameter is a quality parameter, if you increase it, you will
get more parallelism with the reality, but you will need more physics iterations
per frame to archive the same overlap.
-The physicsIterations parameter is a quality parameter, it specific the homogeneous
overlap you will have in the colony. In you increase it, you will get less overlap.
-The parameter maxBodyLength is a statistical parameter. We use it to improve the
performance of the engine. If any of your bacterials exceeds slightly the value put
here, nothing happens (in terms of quality).
-The parameter minBodyLength follows the same logic that maxBodyLength parameter.
It value goes from bodyWidth to maxBodyLength.
-The parameter bodyWidth will be the width of all yours bacterials.
-The parameter bodyFriction is the friction occasioned bewteen bodies in a collision.
*/
void ceInit(unsigned int combinedRing, unsigned int physicsIterations,
    float maxBodyLength, float minBodyLength, float bodyWidth);

#ifdef __cplusplus
}
#endif

#endif //CELLENGINE_HEADER