#ifndef PROJECT_PHYSICS__SUSPENSION_H
#define PROJECT_PHYSICS__SUSPENSION_H

#include <map>

#include "render/RenderManager.h"
#include "object/PlayerList.h"

#include "timing/TimedSubsystem.h"

namespace Project {
namespace Physics {

/**
    A bit of a misnomer: this class is currently reponsible for
    suspension, drag force, and turning force (basically all car mechanics).
*/
class Suspension : public Timing::TimedSubsystem {
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
    
    Object::PlayerList *playerList;
    Render::RenderManager *renderManager;
public:
    Suspension(unsigned long tickTime) : TimedSubsystem(tickTime),
        playerList(NULL), renderManager(NULL) {}
    
    void checkForWheelsOnGround();
    int wheelsOnGround(int playerID);
    
    void doAction(unsigned long currentTime);
    
    /** This function should be called before doStep().
        
        If @a renderManager is NULL, no debug drawing will be done.
    */
    void setData(Object::PlayerList *playerList,
        Render::RenderManager *renderManager);
private:
    void applySuspension();
    
    void calculateSuspensionForPlayer(Object::Player *player);
    void applyDragForce(Object::Player *player);
    
    void debugDrawWheel(const Math::Matrix &transform,
        const Math::Point &centre);
};

}  // namespace Physics
}  // namespace Project

#endif
