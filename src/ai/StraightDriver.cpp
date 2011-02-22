#include <cstdlib>
#include "StraightDriver.h"

#include "math/Point.h"
#include "math/Values.h"

#include "misc/Sleeper.h"

namespace Project {
namespace AI {

StraightDriver::StraightDriver(Object::Player *player) : Driver(player) {
    sittingStill = false;
    sittingStillSince = 0;
    
    intention.setAccel(1.0);
    intention.setPaint(true);
}

const World::PlayerIntention &StraightDriver::getAction() {
    Object::Player *player = getPlayer();
    const Map::PathNode *current = player->getPathTracker()->getCurrentNode();
    
    int random = std::rand() % current->getNextNodes().size();
    Map::PathNode *next = current->getNextNodes()[random];
    
    Math::Point facing = player->getPhysicalObject()->getFrontDirection();
    Math::Point right = player->getPhysicalObject()->getRightDirection();
    Math::Point desired = next->getPosition() - player->getPosition();
    desired.normalize();
    
    double angle = right.dotProduct(desired);
    
    intention.setTurn(0.0);
    if(angle < -PI / 8) intention.setTurn(1.0);
    else if(angle < -PI / 16) intention.setTurn(0.3);
    else if(angle < -PI / 32) intention.setTurn(0.1);
    if(angle > +PI / 8) intention.setTurn(-1.0);
    else if(angle > +PI / 16) intention.setTurn(-0.3);
    else if(angle > +PI / 32) intention.setTurn(-0.1);
    
    intention.setReset(false);
    if(player->getPhysicalObject()->getLinearVelocity().length() < 0.01) {
        unsigned long now = Misc::Sleeper::getTimeMilliseconds();
        
        if(!sittingStill) {
            sittingStill = true;
            sittingStillSince = now;
        }
        else if(sittingStillSince + 2000 < now) {
            LOG(PHYSICS, "Player " << player->getID() << " is trying to reset");
            
            intention.setReset(true);
            
            sittingStill = false;
        }
    }
    else sittingStill = false;
    
    return intention;
}

}  // namespace AI
}  // namespace Project
