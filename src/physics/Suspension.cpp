#include <cmath>

#include "Suspension.h"

#include "object/Player.h"
#include "settings/SettingsManager.h"

#include "opengl/OpenGL.h"
#include "GL/glu.h"
#include "opengl/MathWrapper.h"

#include "PhysicsWorld.h"

#include "event/EventSystem.h"
#include "math/Values.h"

namespace Project {
namespace Physics {

void Suspension::physicsTickHandler(Event::PhysicsTick *event) {
    doAction(event->getElapsed() * 1000.0);
}

Suspension::Displacement Suspension::Spring::doRaycast() {
    double length = Physics::PhysicsWorld::getInstance()
        ->raycastLength(suspensionPoint, suspensionPoint + axis);
    
    bool onGround = true;
    if(length > stretchLength) {  // stretched too far
        length = stretchLength;
        onGround = false;
    }
    else if(length < minLength) {  // compressed too much
        length = minLength;
    }
    
    double normalizedLength = length - restLength;
    
    return Displacement(normalizedLength, onGround);
}

double Suspension::Spring::calculateForceFactor(
    const Displacement &thisDisplacement) {
    
    /*LOG(PHYSICS, "spring displacement: " << thisDisplacement.getDisplacement()
        << " (" << thisDisplacement.isOnGround() << ")");*/
    
    if(!thisDisplacement.isOnGround()) {
        return 0.0;
    }
    
    double displacement = thisDisplacement.getDisplacement();
    double displacementSpeed
        = lastDisplacement.getDisplacement() - displacement;
    if(!lastDisplacement.isOnGround()) {
        displacementSpeed = 0.0;  // no damping force when first hit the ground
    }
    
    double GRAVITY = GET_SETTING("physics.constant.gravity", 9.81);
    static const double VEHICLE_WEIGHT = 2.0;
    static const double NORMAL_FORCE = (GRAVITY * VEHICLE_WEIGHT) / 4.0;
    
    static const double K_FACTOR = 100.0;
    static const double INITIAL_K = (K_FACTOR / VEHICLE_WEIGHT / 4.0);
    static const double INITIAL_C = 2 * std::sqrt(INITIAL_K * VEHICLE_WEIGHT);
    
    double K = GET_SETTING("physics.driving.constant.k", 1.0) * INITIAL_K;
    double C = GET_SETTING("physics.driving.constant.c", 1.0) * INITIAL_C;
    
    //LOG(PHYSICS, "normal: " << -NORMAL_FORCE << ", -K*disp: " << -K*displacement);
    //LOG(PHYSICS, "C*disp: " << C*displacementSpeed);
    
    double factor
        = -NORMAL_FORCE + (K * displacement)
        + (-C * displacementSpeed);
    
    // prevent springs from sucking the ground in
    if(factor > 0.0) factor = 0.0;
    
    //LOG(PHYSICS, "factor: " << factor);
    
    //LOG(PHYSICS, "     " << -K*displacement << ", " << C*displacementSpeed);
    
    return factor;
}

Suspension::Suspension() : worldManager(NULL) {
    METHOD_OBSERVER(&Suspension::physicsTickHandler);
}

void Suspension::checkForWheelsOnGround() {
    if(!GET_SETTING("physics.driving.enablesuspension", 0)) {
        return;
    }
    
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        int onGround = wheelsOnGround(player->getID());
        player->setOnGround(onGround > 2);
    }
}

int Suspension::wheelsOnGround(int playerID) {
    int onGround = 0;
    
    Object::Player *player = worldManager->getPlayer(playerID);
    if(!player) return 0;
    
    for(int wheel = 0; wheel < 4; wheel ++) {
        if(player->getPhysicalObject()->getSpring(wheel).isOnGround()) {
            onGround ++;
        }
    }
    
    return onGround;
}

void Suspension::doAction(unsigned long currentTime) {
	applySuspension(1.0);
}

void Suspension::setData(Object::WorldManager *worldManager) {
    this->worldManager = worldManager;
}

void Suspension::calculateSuspensionForPlayer(Object::Player *player, double forceScale) {

	double front_sep = GET_SETTING("physics.wheel.frontsep", 0.4);
	double front_z = GET_SETTING("physics.wheel.frontz", 0.68);
	double back_sep = GET_SETTING("physics.wheel.backsep", 0.4);
	double back_z = GET_SETTING("physics.wheel.backz", -0.68);

    static const Math::Point suspensionPoint[] = {
        Math::Point(front_sep, -0.2 + 0.05,front_z),
        Math::Point(-front_sep, -0.2 + 0.05, front_z),
        Math::Point(-back_sep, -0.2 + 0.05, back_z),
        Math::Point(back_sep, -0.2 + 0.05, back_z),
    };

	// downward axis of suspension springs
    Math::Point axis = -player->getPhysicalObject()->getUpDirection();
    axis.normalize();
	double downFactor = calculateDownFactor(axis);
	player->getPhysicalObject()->setTraction(std::sqrt(downFactor));

    for(int wheel = 0; wheel < 4; wheel ++) {
        Math::Matrix matrix = player->getTransformation();
        
        // suspension attachment point in world space
        Math::Point point = matrix * suspensionPoint[wheel];
        
        // rotated but not translated suspension attachment point
        Math::Point forcePoint = suspensionPoint[wheel];
        forcePoint.setW(0.0);
        forcePoint = matrix * forcePoint;
        
        // create a Spring object
        Spring spring(point, axis);
        spring.setMinLength(
            GET_SETTING("physics.driving.minlength", 0.0));
        spring.setRestLength(
            GET_SETTING("physics.driving.restlength", 1.0));
        spring.setStretchLength(
            GET_SETTING("physics.driving.stretchlength", 1.0));
        
        // restore last frame's displacement in the Spring object
        spring.setLastDisplacement(
            player->getPhysicalObject()->getSpring(wheel));
        
        // calculate and apply actual force
        Displacement displacement = spring.doRaycast();
        double forceFactor = spring.calculateForceFactor(displacement);
		
        player->applyForce(axis * forceFactor * downFactor * forceScale, forcePoint);
        
        // record displacement for next time
        player->getPhysicalObject()->setSpring(wheel, displacement);
        
        // find the position of the wheel
        double down = displacement.getDisplacement();
        player->setSuspension(wheel,
            suspensionPoint[wheel]
                + Math::Point(0.0, -down - GET_SETTING("physics.wheel.diameter", 0.2), 0.0));
    }
}

void Suspension::applySuspension(double forceScale) {
    if(!GET_SETTING("physics.driving.enablesuspension", 0)) {
        return;
    }
    
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        applyDragForce(player, forceScale);
        calculateSuspensionForPlayer(player, forceScale);
    }
}

void Suspension::applyDragForce(Object::Player *player, double forceScale) {
    Physics::PhysicalPlayer *physicalPlayer = player->getPhysicalObject();
    Math::Point linearVelocity = physicalPlayer->getLinearVelocity();
    Math::Point angularVelocity = physicalPlayer->getAngularVelocity();
    
    double linear;
	double angular;
	double sideways;

	if (player->getOnGround()) {
		linear = GET_SETTING("physics.driving.lineardrag", 0.1)*player->getPhysicalObject()->getTraction();
		angular = GET_SETTING("physics.driving.angulardrag", 0.1)*player->getPhysicalObject()->getTraction();
		sideways = GET_SETTING("physics.driving.sidewaysdrag", 0.1)*player->getPhysicalObject()->getTraction();
	}
	else {
		linear = GET_SETTING("physics.driving.airlineardrag", 0.1);
		angular = GET_SETTING("physics.driving.airangulardrag", 0.1);
		sideways = GET_SETTING("physics.driving.airsidewaysdrag", 0.1);
	}

    Math::Point linearDrag = -linear * linearVelocity;
    Math::Point angularDrag = -angular * angularVelocity;
    
    physicalPlayer->applyForce(linearDrag * forceScale);
    physicalPlayer->applyTorque(angularDrag * forceScale);
    
    // sideways drag (prevent slipping)
    
    Math::Point sidewaysAxis = physicalPlayer->getRightDirection();
    double sidewaysSpeed = linearVelocity.dotProduct(sidewaysAxis);
    Math::Point sidewaysDrag = -sideways * sidewaysSpeed * sidewaysAxis;
    
    double enterSlipState = GET_SETTING("physics.slipstate.enter", 1.0);
    double exitSlipState = GET_SETTING("physics.slipstate.exit", 1.0);
    
    if(std::fabs(sidewaysSpeed) >= enterSlipState) {
        physicalPlayer->setSliding(true);
    }
    if(std::fabs(sidewaysSpeed) <= exitSlipState) {
        physicalPlayer->setSliding(false);
    }
    
    //LOG(PHYSICS, "sideways " << sidewaysSpeed << "\t" << physicalPlayer->getSliding());
    
    if(physicalPlayer->getSliding()) {
        sidewaysDrag *= GET_SETTING("physics.slipstate.sidewaysfactor", 1.0);
    }
    
    physicalPlayer->applyForce(sidewaysDrag*forceScale);
}

double Suspension::calculateDownFactor(const Math::Point& axis) {
	return Math::maximum(GET_SETTING("physics.driving.mindownfactor", 0.1), -axis.normalized().getY());
}

}  // namespace Physics
}  // namespace Project
