#ifndef PROJECT_PHYSICS__SUSPENSION_H
#define PROJECT_PHYSICS__SUSPENSION_H

#include <map>

#include "render/RenderManager.h"
#include "object/WorldManager.h"

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
    class Displacement {
    private:
        double displacement;
        bool onGround;
    public:
        Displacement() : displacement(0.0), onGround(false) {}
        Displacement(double displacement, bool onGround)
            : displacement(displacement), onGround(onGround) {}
        
        double getDisplacement() const { return displacement; }
        bool isOnGround() const { return onGround; }
    };
    
    class Spring {
    private:
        Math::Point suspensionPoint;
        Math::Point axis;
        double minLength, restLength, stretchLength;
        Displacement lastDisplacement;
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
    // map from player ID to vector of 4 elements,
    // the last known displacement of each wheel
    typedef std::map<int, std::vector<Displacement> > playerSuspensionType;
    playerSuspensionType playerSuspension;
    
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
private:
    void applySuspension();
    
    void calculateSuspensionForPlayer(Object::Player *player);
    void applyDragForce(Object::Player *player);
};

}  // namespace Physics
}  // namespace Project

#endif
