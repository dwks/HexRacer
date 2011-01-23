#ifndef PROJECT_PHYSICS__SUSPENSION_H
#define PROJECT_PHYSICS__SUSPENSION_H

#include <map>

#include "render/RenderManager.h"
#include "object/PlayerList.h"

namespace Project {
namespace Physics {

/**
    A bit of a misnomer: this class is currently reponsible for
    suspension, drag force, and turning force (basically all car mechanics).
*/
class Suspension {
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
        double restLength, stretchLength;
        Displacement lastDisplacement;
    public:
        Spring(Math::Point suspensionPoint, Math::Point axis)
            : suspensionPoint(suspensionPoint), axis(axis) {}
        
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
    typedef std::map<int, std::vector<Displacement> > playerSuspensionType;
    playerSuspensionType playerSuspension;
public:
    /** If @a renderManager is NULL, no debug drawing will be done.
    */
    void applySuspension(Object::PlayerList *playerList,
        Render::RenderManager *renderManager);
private:
    void calculateSuspensionForPlayer(Object::Player *player);
    void applyDragForce(Object::Player *player);
    
    void debugDrawWheel(const Math::Matrix &transform,
        const Math::Point &centre);
};

}  // namespace Physics
}  // namespace Project

#endif
