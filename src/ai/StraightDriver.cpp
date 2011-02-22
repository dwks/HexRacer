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
    
    double angle;
    int x = 0;
    do {
        //int random = std::rand() % current->getNextNodes().size();
        int random = player->getID() % current->getNextNodes().size();
        Map::PathNode *next = current->getNextNodes()[random];
        
        Math::Point facing = player->getPhysicalObject()->getFrontDirection();
        Math::Point right = player->getPhysicalObject()->getRightDirection();
        Math::Point desired = next->getPosition() - player->getPosition();
        desired.normalize();
        
        angle = right.dotProduct(desired);
        
        current = next;
    } while((angle < -PI / 8 || angle > PI / 8) && ++x < 5);
    
    if(x > 3) intention.setAccel(0.3);
    else intention.setAccel(1.0);
    
    intention.setTurn(0.0);
    if(angle < -PI / 6) intention.setTurn(1.0);
    else if(angle < -PI / 16) intention.setTurn(0.5);
    else if(angle < -PI / 24) intention.setTurn(0.2);
    else if(angle < -PI / 32) intention.setTurn(0.1);
    if(angle > +PI / 6) intention.setTurn(-1.0);
    else if(angle > +PI / 16) intention.setTurn(-0.5);
    else if(angle > +PI / 24) intention.setTurn(-0.2);
    else if(angle > +PI / 32) intention.setTurn(-0.1);
    
    intention.setReset(false);
    if(player->getPhysicalObject()->getLinearVelocity().length() < 0.05) {
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
