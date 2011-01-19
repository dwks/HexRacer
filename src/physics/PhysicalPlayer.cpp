#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsFactory.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(Math::Point position, float mass){
    primaryRigidBody = Physics::PhysicsFactory::createRigidBox(2.0,2.0,2.0,position,mass);
    
}

Math::Point PhysicalPlayer::getOrigin() {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toPoint(trans.getOrigin());
}

}  // namespace Physics
}  // namespace Project
