#include <cmath>

#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsWorld.h"
#include "PhysicsFactory.h"

#include "math/Values.h"
#include "math/Geometry.h"

#include "log/Logger.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(const Math::Point &position, const Math::Point &direction) {
    onGround = false;
    speedBoost = 1.0;
    rigidBody = NULL;  // essential, constructRigidBody tries to delete it
    
	Math::Matrix transformation = Math::Matrix::getTranslationMatrix(position);
	double angle = Math::Geometry::vectorTo2DAngle(direction.rotate90CW(Math::Y_AXIS), Math::Y_AXIS);
	transformation *= Math::Matrix::getRotationMatrix(Math::Y_AXIS, -angle);

	constructRigidBody(transformation);
    //constructRigidBody(position);
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
        ::createRigidBox(0.4,0.2,1.04,position,2.0);
    
    /*btTransform transform = btTransform::getIdentity();
    transform.setOrigin(Converter::toVector(Math::Point(0.0, +1.0, 0.0)));
    rigidBody->setCenterOfMassTransform(transform);*/
    
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
    rigidBody->activate();  // !!! hack to prevent sleeping
    
    return Converter::toPoint(rigidBody->getLinearVelocity());
}

Math::Point PhysicalPlayer::getAngularVelocity() const {
    return Converter::toPoint(rigidBody->getAngularVelocity());
}

Math::Point PhysicalPlayer::getFrontDirection() const {
    // 4D vector (don't want translation included)
    return getTransformation() * Math::Point(0.0, 0.0, +1.0, 0.0);
}

Math::Point PhysicalPlayer::getRightDirection() const {
    // 4D vector (don't want translation included)
    return getTransformation() * Math::Point(+1.0, 0.0, 0.0, 0.0);
}

Math::Point PhysicalPlayer::getUpDirection() const {
    // 4D vector (don't want translation included)
    return getTransformation() * Math::Point(0.0, +1.0, 0.0, 0.0);
}

void PhysicalPlayer::applyAcceleration(double acceleration) {
    if(!onGround) return;
    
    rigidBody->activate();
    
    double constant = GET_SETTING("physics.constant.accel", 1.0);
	double brakeConstant = GET_SETTING("physics.constant.brake", 5.0);
    
    btTransform transform = rigidBody->getWorldTransform();
    btMatrix3x3 matrix(transform.getRotation());
    Math::Point orientation = Converter::toPoint(matrix
        * Converter::toVector(Math::Point(0.0, 0.0, 1.0) * constant));
    
    //LOG(PHYSICS, "accel at " << Misc::Sleeper::getTimeMilliseconds());

	if (acceleration >= 0.0 || getLinearVelocity().dotProduct(orientation) < 0.0) {
        double paintInfluence = (speedBoost - 1.0)
            * GET_SETTING("game.paint.boostinfluence", 1.0) + 1.0;
		applyForce(orientation * constant * acceleration * paintInfluence);
	}
	else {
		applyForce(orientation * brakeConstant * acceleration);
	}
}

void PhysicalPlayer::applyTurning(double amount) {
    if(!onGround) return;
    
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

	#if 0
    // turn in the opposite direction when travelling backwards
    if (getLinearVelocity().dotProduct(forwardAxis) < 0) {
        speed = -speed;
    }
	#endif

	double speedFactor = GET_SETTING("physics.turning.speedfactor", 0.5);
	double speedThreshhold = GET_SETTING("physics.turning.speedthreshhold", 15.0);
	double falloffFactor = GET_SETTING("physics.turning.fallofffactor", 0.25);

	double turning_factor = GET_SETTING("physics.turning.constant", 1.0);

	if (speed <= speedThreshhold) {
		turning_factor += sqrt(speed/speedThreshhold)*(speed*speedFactor);
	}
	else {
		turning_factor += (speedThreshhold*speedFactor)*(1.0/(1.0+(speed-speedThreshhold)*falloffFactor));
	}
	
	 // turn in the opposite direction when travelling backwards
    if (getLinearVelocity().dotProduct(forwardAxis) < 0) {
        turning_factor = -turning_factor;
    }
    
    applyForce(centripetalAxis * centripetalConstant * turning_factor * amount);
    applyTorque(Math::Point(0.0, -1.0, 0.0) * constant * turning_factor * amount);
}

void PhysicalPlayer::doJump() {
    Math::Point upwards = Math::Point(0.0, 1.0, 0.0);
    applyForce(upwards * 50.0f);
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

void PhysicalPlayer::setData(const Math::Matrix &transform,
    const Math::Point &linearVelocity,
    const Math::Point &angularVelocity) {
    
    rigidBody->setWorldTransform(
        Physics::Converter::toTransform(transform));
    rigidBody->setLinearVelocity(
        Physics::Converter::toVector(linearVelocity));
    rigidBody->setAngularVelocity(
        Physics::Converter::toVector(angularVelocity));
}

}  // namespace Physics
}  // namespace Project
