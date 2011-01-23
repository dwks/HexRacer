#include <cmath>

#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsWorld.h"
#include "PhysicsFactory.h"

#include "math/Values.h"

#include "log/Logger.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(const Math::Point &position) {
    rigidBody = NULL;  // essential, constructRigidBody tries to delete it
    
    constructRigidBody(position);
}

PhysicalPlayer::~PhysicalPlayer() {
    destroyRigidBody();
}

void PhysicalPlayer::destroyRigidBody() {
    if(rigidBody) {
        PhysicsWorld::getInstance()->destroyRigidBody(rigidBody);
        
        delete rigidBody;  // works even if NULL
    }
}

void PhysicalPlayer::constructRigidBody(const Math::Point &position) {
    destroyRigidBody();
	
    rigidBody = Physics::PhysicsFactory
        ::createRigidBox(0.4,0.2,0.8,position,2.0);
    
    /*rigidBody->setCenterOfMassTransform(Converter::toTransform(
        Math::Matrix::getTranslationMatrix(Math::Point(0.0, +3.0, 0.0))));*/
    
    PhysicsWorld::getInstance()->registerRigidBody(rigidBody);
}

void PhysicalPlayer::constructRigidBody(const Math::Matrix &transformation) {
    constructRigidBody(Math::Point());
    
    rigidBody->setWorldTransform(
        Converter::toTransform(transformation));
}

Math::Point PhysicalPlayer::getOrigin() const {
    btTransform trans = rigidBody->getWorldTransform();
    
    return Converter::toPoint(trans.getOrigin());
}

Math::Matrix PhysicalPlayer::getTransformation() const {
    btTransform trans = rigidBody->getWorldTransform();
    
    return Converter::toMatrix(trans);
}

Math::Point PhysicalPlayer::getLinearVelocity() const {
    return Converter::toPoint(rigidBody->getLinearVelocity());
}

Math::Point PhysicalPlayer::getAngularVelocity() const {
    return Converter::toPoint(rigidBody->getAngularVelocity());
}

void PhysicalPlayer::applyAcceleration(double acceleration) {
    rigidBody->activate();
    
    double constant = GET_SETTING("physics.constant.accel", 1.0);
    
    btTransform transform = rigidBody->getWorldTransform();
    btMatrix3x3 matrix(transform.getRotation());
    Math::Point orientation = Converter::toPoint(matrix
        * Converter::toVector(Math::Point(0.0, 0.0, 1.0) * constant));
    
    applyForce(orientation * constant * acceleration);
}

void PhysicalPlayer::applyTurning(double amount) {
    rigidBody->activate();
    
    double constant = GET_SETTING("physics.constant.turn", 1.0);
    double centripetalConstant
        = GET_SETTING("physics.constant.centripetal", 1.0);
    
    Math::Matrix matrix = getTransformation();
    Math::Point forwardAxis = matrix * Math::Point(0.0, 0.0, 1.0, 0.0);
    Math::Point centripetalAxis = matrix * Math::Point(-1.0, 0.0, 0.0, 0.0);
    
    forwardAxis.normalize();
    centripetalAxis.normalize();
    
    double speed = getLinearVelocity().length();
    /*double centripetalSpeed = getLinearVelocity().dotProduct(forwardAxis)
        / getLinearVelocity().length();*/
    
    //LOG(PHYSICS, "centripetal: " << centripetalSpeed);
    
    applyForce(centripetalAxis * centripetalConstant * speed * amount);
    applyTorque(Math::Point(0.0, -1.0, 0.0) * constant * speed * amount);
}

void PhysicalPlayer::doJump() {
    /*Math::Point upwards = Converter::toPoint(matrix
        * Converter::toVector(Math::Point(0.0, 1.0, 0.0)));*/
    Math::Point upwards = Math::Point(0.0, 1.0, 0.0);
    applyForce(upwards * 30.0f);
}

void PhysicalPlayer::applyForce(const Math::Point &force) {
    rigidBody->applyCentralForce(Converter::toVector(force));
}

void PhysicalPlayer::applyForce(const Math::Point &movement,
    const Math::Point &force) {
    
    rigidBody->applyForce(
        Converter::toVector(movement),
        Converter::toVector(force));
}

void PhysicalPlayer::applyTorque(const Math::Point &torque) {
    rigidBody->applyTorque(Converter::toVector(torque));
}

}  // namespace Physics
}  // namespace Project
