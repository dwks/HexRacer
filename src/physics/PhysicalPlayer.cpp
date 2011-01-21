#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsWorld.h"
#include "PhysicsFactory.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(const Math::Point &position) {
    primaryRigidBody = NULL;  // essential, constructRigidBody tries to delete
    
    constructRigidBody(position);
}

PhysicalPlayer::~PhysicalPlayer() {
    destroyRigidBody();
}

void PhysicalPlayer::destroyRigidBody() {
    if(primaryRigidBody) {
        PhysicsWorld::getInstance()->destroyRigidBody(primaryRigidBody);
        
        delete primaryRigidBody;  // works even if NULL
    }
}

void PhysicalPlayer::constructRigidBody(const Math::Point &position) {
    destroyRigidBody();
	
    primaryRigidBody = Physics::PhysicsFactory
        ::createRigidBox(0.4,0.2,0.8,position,2.0);
    PhysicsWorld::getInstance()->registerRigidBody(primaryRigidBody);
}

void PhysicalPlayer::constructRigidBody(const Math::Matrix &transformation) {
    destroyRigidBody();
    
    primaryRigidBody = Physics::PhysicsFactory
        ::createRigidBox(0.2,0.2,0.2,Math::Point(),2.0);
    primaryRigidBody->setWorldTransform(
        Converter::toTransform(transformation));
    PhysicsWorld::getInstance()->registerRigidBody(primaryRigidBody);
}

Math::Point PhysicalPlayer::getOrigin() const {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toPoint(trans.getOrigin());
}

Math::Matrix PhysicalPlayer::getTransformation() const {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toMatrix(trans);
}

void PhysicalPlayer::applyMovement(const Math::Point &movement) {
    primaryRigidBody->activate();
    
    double turn = movement.getX();
    primaryRigidBody->applyTorque(
        Converter::toVector(Math::Point(0.0, 1.0, 0.0)
            * turn * 400.0f));
    
    btTransform transform = primaryRigidBody->getWorldTransform();
    btMatrix3x3 matrix(transform.getRotation());
    Math::Point orientation
        = Converter::toPoint(matrix
            * Converter::toVector(Math::Point(0.0, 0.0, 1.0)));
    
    double accel = movement.getZ();
    primaryRigidBody->applyCentralForce(
        Converter::toVector(orientation
            * accel * 5000.0f));
}

void PhysicalPlayer::applyForce(const Math::Point &movement,
    const Math::Point &at) {
    
    primaryRigidBody->applyForce(
        Converter::toVector(movement),
        Converter::toVector(at));
}

}  // namespace Physics
}  // namespace Project
