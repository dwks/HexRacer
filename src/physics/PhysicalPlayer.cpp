#include "PhysicalPlayer.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(btRigidBody* pRigidBody){
    primaryRigidBody=pRigidBody;
}

Math::Point PhysicalPlayer::getOrigin() {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Math::Point( trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

    
}  // namespace Physics
}  // namespace Project
