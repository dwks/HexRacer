#include <cstdlib>
#include "StraightDriver.h"

#include "math/Point.h"
#include "math/Values.h"

namespace Project {
namespace AI {

StraightDriver::StraightDriver(Object::Player *player) : Driver(player) {
    intention.setAccel(1.0);
    intention.setPaint(true);
}

const World::PlayerIntention &StraightDriver::getAction() {
    Object::Player *player = getPlayer();
    const Map::PathNode *current = player->getPathTracker()->getCurrentNode();
    
    // get the angle to the next node (if the angle is really high, try the
    // next node and the next etc.)
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
    
    // if the first few nodes have a high angle there's probably a corner,
    // so slow down
    if(x > 3) intention.setAccel(0.3);
    else intention.setAccel(1.0);
    
    // turn towards the next node
    intention.setTurn(0.0);
    if(angle < -PI / 6) intention.setTurn(1.0);
    else if(angle < -PI / 16) intention.setTurn(0.5);
    else if(angle < -PI / 24) intention.setTurn(0.2);
    else if(angle < -PI / 32) intention.setTurn(0.1);
    if(angle > +PI / 6) intention.setTurn(-1.0);
    else if(angle > +PI / 16) intention.setTurn(-0.5);
    else if(angle > +PI / 24) intention.setTurn(-0.2);
    else if(angle > +PI / 32) intention.setTurn(-0.1);
    
    return intention;
}

}  // namespace AI
}  // namespace Project
