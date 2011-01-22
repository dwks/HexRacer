#include <cmath>

#include "Suspension.h"

#include "object/Player.h"
#include "object/PlayerList.h"
#include "settings/SettingsManager.h"

#include "PhysicsWorld.h"

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
    }
}

void Suspension::applySuspension(Object::PlayerList *playerList,
    Render::RenderManager *renderManager) {
    
    if(!GET_SETTING("physics.driving.dosuspension", 0)) {
        return;
    }
    
    Object::PlayerList::IteratorType it = playerList->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        calculateSuspensionForPlayer(player);
#if 0
        for(int wheel = 0; wheel < 4; wheel ++) {
            Math::Matrix matrix = player->getTransformation();
            
            //LOG(OPENGL, "Centre of mass: " << matrix * Math::Point(0.0, 0.0, 0.0, 1.0));
            
            // uses 4D points
            Math::Point axis = matrix * Math::Point(0.0, -1.0, 0.0, 0.0);
            Math::Point point = matrix * suspensionPoint[wheel];
            
            //LOG(OPENGL, "Wheel " << wheel << ": " << point);
            
            Physics::PhysicsWorld::getInstance()->getDebug()
                .drawLine(Physics::Converter::toVector(point),
                    Physics::Converter::toVector(point + axis), btVector3(1.0, 0.0, 0.0));
            
            const double REST_LENGTH
                = GET_SETTING("physics.driving.restlength", 1.0);
            const double STRETCH_LENGTH
                = GET_SETTING("physics.driving.stretchlength", 1.0);
            
            double length = Physics::PhysicsWorld::getInstance()
                ->raycastLength(point, point + axis);
            
            length = length - REST_LENGTH;
            if(length < -STRETCH_LENGTH) length = -STRETCH_LENGTH;
            if(length > +STRETCH_LENGTH) length = 0.0; //length = +STRETCH_LENGTH;
            
            static const double K = 20.0; //(9.81 * 1.0) / (REST_LENGTH * 4);
            double factor = K * length;
            
            //if(factor > -4.5) factor = (factor + 4.5) * 2.0 - 4.5;
            
            if(factor < -5.0) factor = -5.0;
            
            //player->applyForce(Math::Point(0.0, 1.0, 0.0) * K, suspensionPoint[wheel]);
            //player->applyForce(axis * factor, suspensionPoint[wheel]);
            
            if(renderManager) {
                Physics::PhysicsWorld::getInstance()->getDebug()
                    .drawLine(Physics::Converter::toVector(point),
                        Physics::Converter::toVector(point + axis), btVector3(1.0, 0.0, 0.0));
            }
            
            //factor = (factor + 4.5) * 0.5 - 4.5;
            
            //LOG(PHYSICS, "force: " << factor << " * " << axis);
            player->applyForce(axis * factor, suspensionPoint[wheel]);
        }
#endif
    }
}

}  // namespace Physics
}  // namespace Project
