#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsWorld.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(btRigidBody* pRigidBody){
    primaryRigidBody = pRigidBody;
}

/*PhysicalPlayer *PhysicalPlayer::construct(const Math::Point &origin) {
    return new Physics::PhysicalPlayer(
        PhysicsWorld::getInstance()->createRigidBox(
            2.0, 2.0, 2.0, origin, 2.0));
}*/

Math::Point PhysicalPlayer::getOrigin() const {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toPoint(trans.getOrigin());
}

void PhysicalPlayer::applyMovement(const Math::Point &movement) {
    primaryRigidBody->activate();
    
    primaryRigidBody->applyCentralForce(Converter::toVector(
        movement * 5000.0f));
}

void PhysicalPlayer::constructRigidBody(const Math::Point &origin) {
    primaryRigidBody = PhysicsWorld::getInstance()->createRigidBox(
        2.0, 2.0, 2.0, origin, 2.0);
}

}  // namespace Physics
}  // namespace Project
