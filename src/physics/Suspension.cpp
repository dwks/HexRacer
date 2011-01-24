#include <cmath>

#include "Suspension.h"

#include "object/Player.h"
#include "object/PlayerList.h"
#include "settings/SettingsManager.h"

#include "opengl/OpenGL.h"
#include "GL/glu.h"
#include "opengl/MathWrapper.h"

#include "PhysicsWorld.h"

#define WHEEL_DIAMETER 0.2

namespace Project {
namespace Physics {

Suspension::Displacement Suspension::Spring::doRaycast() {
    double length = Physics::PhysicsWorld::getInstance()
        ->raycastLength(suspensionPoint, suspensionPoint + axis);
    
    bool onGround = true;
    if(length > stretchLength) {  // stretched too far
        length = restLength;
        onGround = false;
    }
    else if(length < 0.0) {  // compressed too much
        length = 0.0;
    }
    
    return Displacement(stretchLength - length, onGround);
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
    
    static const double GRAVITY = 9.81;
    static const double VEHICLE_WEIGHT = 2.0;
    static const double INITIAL_K = (GRAVITY * VEHICLE_WEIGHT) / (restLength * 4);
    static const double INITIAL_C = 2 * std::sqrt(INITIAL_K * VEHICLE_WEIGHT);
    
    double K = GET_SETTING("physics.driving.constant.k", 1.0) * INITIAL_K;
    double C = GET_SETTING("physics.driving.constant.c", 1.0) * INITIAL_C;
    
    double factor
        = -K * displacement
        + C * displacementSpeed;
    
    //LOG(PHYSICS, "    " << -K*displacement << ", " << C*displacementSpeed);
    
    return factor;
}

void Suspension::calculateSuspensionForPlayer(Object::Player *player) {
    // add +0.01 to Y so that suspension points are not usually underground
    // multiply Z by 0.9 to shift suspension points inwards slightly
    static const Math::Point suspensionPoint[] = {
        Math::Point(0.4, -0.2 + 0.01, 0.8 * 0.9),
        Math::Point(-0.4, -0.2 + 0.01, 0.8 * 0.9),
        Math::Point(-0.4, -0.2 + 0.01, -0.8 * 0.9),
        Math::Point(0.4, -0.2 + 0.01, -0.8 * 0.9),
    };
    
    for(int wheel = 0; wheel < 4; wheel ++) {
        Math::Matrix matrix = player->getTransformation();
        
        // 4D vector (don't want translation included)
        Math::Point axis = matrix * Math::Point(0.0, -1.0, 0.0, 0.0);
        axis.normalize();
        
        // suspension attachment point in world space
        Math::Point point = matrix * suspensionPoint[wheel];
        
        // rotated but not translated suspension attachment point
        Math::Point forcePoint = suspensionPoint[wheel];
        forcePoint.setW(0.0);
        forcePoint = matrix * forcePoint;
        
        // create a Spring object
        Spring spring(point, axis);
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
        debugDrawWheel(matrix, suspensionPoint[wheel]
            + Math::Point(0.0, -1.0, 0.0)
                * (GET_SETTING("physics.driving.stretchlength", 1.0)
                    - (displacement.getDisplacement() + WHEEL_DIAMETER)));
    }
}

void Suspension::applySuspension(Object::PlayerList *playerList,
    Render::RenderManager *renderManager) {
    
    if(!GET_SETTING("physics.driving.enablesuspension", 0)) {
        return;
    }
    
    Object::PlayerList::IteratorType it = playerList->getIterator();
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
    
    Math::Matrix matrix = physicalPlayer->getTransformation();
    Math::Point sidewaysAxis = matrix * Math::Point(1.0, 0.0, 0.0, 0.0);
    
    if(linearVelocity.lengthSquared()) {
        double sidewaysSpeed = linearVelocity.dotProduct(sidewaysAxis)
            /*/ linearVelocity.length()*/;
        
        double sideways = GET_SETTING("physics.driving.sidewaysdrag", 0.1);
        Math::Point sidewaysDrag = -sideways * sidewaysSpeed * sidewaysAxis;
        
        physicalPlayer->applyForce(sidewaysDrag);
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
    
    glCallList(diskID);
    
    glPopMatrix();
}

}  // namespace Physics
}  // namespace Project