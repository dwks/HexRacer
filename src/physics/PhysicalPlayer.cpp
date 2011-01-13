#include "PhysicalPlayer.h"
#include "Converter.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(btRigidBody* pRigidBody){
    primaryRigidBody=pRigidBody;
}

Math::Point PhysicalPlayer::getOrigin() {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toPoint(trans.getOrigin());
}

    
}  // namespace Physics
}  // namespace Project
