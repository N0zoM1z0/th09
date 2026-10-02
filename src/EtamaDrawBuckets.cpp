#include "BulletManager.hpp"

// Keep the real bucket reset out of the update compile carrier. VC7.1
// otherwise uses knowledge of its register clobbers and changes OnUpdate.
// This separation is codegen evidence, not unique original TU ownership.
int EtamaController::ClearDrawBuckets()
{
    this->drawBuckets[5] = NULL;
    this->drawBuckets[4] = NULL;
    this->drawBuckets[3] = NULL;
    this->drawBuckets[2] = NULL;
    this->drawBuckets[1] = NULL;
    this->drawBuckets[0] = NULL;
    return 0;
}
