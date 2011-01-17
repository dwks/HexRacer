//#include <cstdio>
#include "PhysicsSerializer.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

PhysicsSerializer::PhysicsSerializer() {
    serializer = new btDefaultSerializer(SIZE);
}

void PhysicsSerializer::serialize(btRigidBody *rigidBody) {
    serializer->startSerialization();
    rigidBody->serializeSingleObject(serializer);
    serializer->finishSerialization();
    
    LOG(NETWORK, "btRigidBody serialization is "
        << serializer->getCurrentBufferSize() << " bytes");
    /*std::fwrite(
        serializer->getBufferPointer(),
        serializer->getCurrentBufferSize(),
        1,
        stdout);*/
}

}  // namespace Physics
}  // namespace Project
