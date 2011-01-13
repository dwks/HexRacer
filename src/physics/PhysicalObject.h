#ifndef PROJECT_PHYSICS__PHYSICAL_OBJECT_H
#define PROJECT_PHYSICS__PHYSICAL_OBJECT_H

#include "math/Point.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicalObject {
public:
    Math::Point getOrigin();
    
protected:
    btRigidBody* primaryRigidBody; 
};

}  // namespace Physics
}  // namespace Project

#endif
