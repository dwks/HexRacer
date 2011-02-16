#include <cmath>

#include "Suspension.h"

#include "object/Player.h"
#include "settings/SettingsManager.h"

#include "opengl/OpenGL.h"
#include "GL/glu.h"
#include "opengl/MathWrapper.h"

#include "PhysicsWorld.h"

#include "event/ObserverList.h"
#include "math/Values.h"

#define WHEEL_DIAMETER 0.2

namespace Project {
namespace Physics {

void Suspension::PhysicsTickObserver::observe(Event::PhysicsTick *event) {
    suspension->doAction(event->getElapsed() * 1000.0);
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

	//Set the force scaling factor depending on the orientation of the wheel
	double downwardsFactor = Math::maximum(
		axis.unitDotProduct(Math::Point(0.0, -1.0, 0.0)),
		GET_SETTING("physics.driving.mindownfactor", 0.25)
		);
    
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
    
    return factor*downwardsFactor;
}

Suspension::Suspension() : worldManager(NULL), renderManager(NULL) {
    ADD_OBSERVER(new PhysicsTickObserver(this));
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
    
    for(int wheel = 0; wheel < 4; wheel ++) {
        if(playerSuspension.find(playerID) == playerSuspension.end()) {
            continue;
        }
        
        if(playerSuspension[playerID][wheel].isOnGround()) {
            onGround ++;
        }
    }
    
    return onGround;
}

void Suspension::doAction(unsigned long currentTime) {
    applySuspension();
}

void Suspension::setData(Object::WorldManager *worldManager,
    Render::RenderManager *renderManager) {
    
    this->worldManager = worldManager;
    this->renderManager = renderManager;
}

void Suspension::calculateSuspensionForPlayer(Object::Player *player) {
    // add +0.01 to Y so that suspension points are not usually underground
    // multiply Z by 0.9 to shift suspension points inwards slightly

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
    
    // bullet rays appear to have radius 0.01.
    /*static const Math::Point suspensionPoint[] = {
        Math::Point(0.4 + 0.010001, +0.2 + 0.1, 0.8 * 0.9),
        Math::Point(-0.4 - 0.010001, +0.2 + 0.1, 0.8 * 0.9),
        Math::Point(-0.4 - 0.010001, +0.2 + 0.1, -0.8 * 0.9),
        Math::Point(0.4 + 0.010001, +0.2 + 0.1, -0.8 * 0.9),
    };*/
    
    for(int wheel = 0; wheel < 4; wheel ++) {
        Math::Matrix matrix = player->getTransformation();
        
        // downward axis of suspension springs
        Math::Point axis = -player->getPhysicalObject()->getUpDirection();
        axis.normalize();
        
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
        playerSuspension[player->getID()].resize(4);
        spring.setLastDisplacement(playerSuspension[player->getID()][wheel]);
        
        // calculate and apply actual force
        Displacement displacement = spring.doRaycast();
        double forceFactor = spring.calculateForceFactor(displacement);
        player->applyForce(axis * forceFactor, forcePoint);
        
        // record displacement for next time
        playerSuspension[player->getID()][wheel] = displacement;
        
        // draw a wheel
        
        double down = displacement.getDisplacement();
        
        player->setSuspension(wheel,
            suspensionPoint[wheel]
                + Math::Point(0.0, -down - GET_SETTING("physics.wheel.diameter", 0.2), 0.0));
        
        /*debugDrawWheel(matrix, suspensionPoint[wheel]
            + Math::Point(0.0, -down - WHEEL_DIAMETER - 0.05, 0.0));*/
    }
}

void Suspension::applySuspension() {
    if(!GET_SETTING("physics.driving.enablesuspension", 0)) {
        return;
    }
    
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        applyDragForce(player);
        calculateSuspensionForPlayer(player);
    }
}

void Suspension::applyDragForce(Object::Player *player) {
    Physics::PhysicalPlayer *physicalPlayer
        = dynamic_cast<Physics::PhysicalPlayer *>(player->getPhysicalObject());
    Math::Point linearVelocity = physicalPlayer->getLinearVelocity();
    Math::Point angularVelocity = physicalPlayer->getAngularVelocity();
    
    double linear = GET_SETTING("physics.driving.lineardrag", 0.1);
    double angular = GET_SETTING("physics.driving.angulardrag", 0.1);
    Math::Point linearDrag = -linear * linearVelocity;
    Math::Point angularDrag = -angular * angularVelocity;
    
    physicalPlayer->applyForce(linearDrag);
    physicalPlayer->applyTorque(angularDrag);
    
    // sideways drag (prevent slipping)
    
    if(player->getOnGround()) {
        Math::Point sidewaysAxis = physicalPlayer->getRightDirection();
        
        if(linearVelocity.lengthSquared()) {
            double sidewaysSpeed = linearVelocity.dotProduct(sidewaysAxis)
                /*/ linearVelocity.length()*/;
            
            double sideways = GET_SETTING("physics.driving.sidewaysdrag", 0.1);
            Math::Point sidewaysDrag = -sideways * sidewaysSpeed * sidewaysAxis;
            
            physicalPlayer->applyForce(sidewaysDrag);
        }
    }
}

void Suspension::debugDrawWheel(const Math::Matrix &transform,
    const Math::Point &centre) {
    
    static bool first = true;
    static int diskID;
    if(first) {
        GLUquadric *quadric = gluNewQuadric();
        diskID = glGenLists(1);
        glNewList(diskID, GL_COMPILE);
        gluDisk(quadric, WHEEL_DIAMETER * 0.1, WHEEL_DIAMETER, 18, 18);
        glEndList();
        gluDeleteQuadric(quadric);
    }
    
    glPushMatrix();
    
    OpenGL::MathWrapper::glMultMatrix(transform);
    OpenGL::MathWrapper::glMultMatrix(
        Math::Matrix::getTranslationMatrix(centre));
    glRotated(90.0, 0.0, 1.0, 0.0);
    
    //LOG(PHYSICS, "debug wheel at " << centre);
    
    glCallList(diskID);
    
    glPopMatrix();
}

}  // namespace Physics
}  // namespace Project
