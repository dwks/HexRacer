#ifndef PROJECT_PHYSICS__PHYSICS_SERIALIZER_H
#define PROJECT_PHYSICS__PHYSICS_SERIALIZER_H

#include "LinearMath/btSerializer.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace Project {
namespace Physics {

class PhysicsSerializer {
private:
    static const int SIZE = 2048 * 1024;
    btSerializer *serializer;
public:
    PhysicsSerializer();
    
    void serialize(btRigidBody *rigidBody);
};

}  // namespace Physics
}  // namespace Project

#endif
