#ifndef PROJECT_PHYSICS__SUSPENSION_H
#define PROJECT_PHYSICS__SUSPENSION_H

#include "object/WorldManager.h"
#include "physics/PhysicalPlayer.h"

#include "event/PhysicsTick.h"
#include "event/Enabler.h"

namespace Project {
namespace Physics {

/**
    A bit of a misnomer: this class is currently reponsible for
    suspension, drag force, and turning force (basically all car mechanics).
*/
class Suspension : public Event::Enabler {
protected:
    void physicsTickHandler(Event::PhysicsTick *event);
private:
    typedef Physics::PhysicalPlayer::SpringDisplacement Displacement;
    
    class Spring {
    private:
        Math::Point suspensionPoint;
        Math::Point axis;
        double minLength, restLength, stretchLength;
        Displacement lastDisplacement;
		unsigned long lastUpdateTime;
    public:
        Spring(Math::Point suspensionPoint, Math::Point axis)
            : suspensionPoint(suspensionPoint), axis(axis) {}
        
        void setMinLength(double minLength)
            { this->minLength = minLength; }
        void setRestLength(double restLength)
            { this->restLength = restLength; }
        void setStretchLength(double stretchLength)
            { this->stretchLength = stretchLength; }
        
        void setLastDisplacement(Displacement lastDisplacement)
            { this->lastDisplacement = lastDisplacement; }
        
        Displacement doRaycast();
        
        double calculateForceFactor(const Displacement &thisDisplacement);
    };
private:
    Object::WorldManager *worldManager;
public:
    Suspension();
    
    void checkForWheelsOnGround();
    int wheelsOnGround(int playerID);
    
    /** Called automatically by PhysicsTickObserver.
    */
    void doAction(unsigned long currentTime);
    
    /** This function should be called before doStep().
    */
    void setData(Object::WorldManager *worldManager);

	static double calculateDownFactor(const Math::Point& axis);

private:
    void applySuspension(double forceScale);
    
    void calculateSuspensionForPlayer(Object::Player *player, double forceScale);
    void applyDragForce(Object::Player *player, double forceScale);
	
};

}  // namespace Physics
}  // namespace Project

#endif
