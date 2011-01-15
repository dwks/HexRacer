#include "PhysicalPlayer.h"
#include "Converter.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(btRigidBody* pRigidBody){
    primaryRigidBody = pRigidBody;
}

Math::Point PhysicalPlayer::getOrigin() {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toPoint(trans.getOrigin());
}

void PhysicalPlayer::applyMovement(const Math::Point &movement) {
    primaryRigidBody->activate();
    
    primaryRigidBody->applyCentralForce(Converter::toVector(
        movement * 5000.0f));
}

}  // namespace Physics
}  // namespace Project
