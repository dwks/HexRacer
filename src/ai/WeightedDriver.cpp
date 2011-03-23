#include <cstdlib>
#include "WeightedDriver.h"

#include "math/Point.h"
#include "math/Values.h"
#include "math/Geometry.h"

#include "misc/Sleeper.h"

namespace Project {
namespace AI {

WeightedDriver::WeightedDriver(Object::Player *player) : Driver(player) {
    LOG(WORLD, "Player " << player->getID()
        << " is controlled by a WeightedDriver");
    
    sittingStill = false;
    sittingStillSince = 0;

    intention.setAccel(1.0);
    intention.setPaint(true);
}

const World::PlayerIntention &WeightedDriver::getAction() {
    Object::Player *player = getPlayer();
    const Map::PathNode *current = player->getPathTracker()->getCurrentNode();
    
    Map::PathNode *next;
    
    // find the next node
    int random = player->getID() % current->getNextNodes().size();
    next = current->getNextNodes()[random];
    
    // calculate various vectors
    Math::Point playerFacing = player->getPhysicalObject()->getFrontDirection();
    Math::Point playerRight = player->getPhysicalObject()->getRightDirection();
    
    Math::Point pathDirection = next->getPosition() - current->getPosition();
    Math::Point beelineDirection = next->getPosition() - player->getPosition();
    beelineDirection.normalize();
    pathDirection.normalize();
    
    // compute error due to being far away from this path edge
    double offAngle = playerRight.dotProduct(pathDirection);
    double turnSign = -Math::sign(offAngle);
    
    Math::Point intersection = Math::Geometry::intersectLine3D(
        current->getPosition(),
        next->getPosition(),
        player->getPosition());
    
    double distanceOff = (intersection - player->getPosition()).length();
    
    // compute error due to facing in the wrong direction
    //double beelineAngle = playerRight.dotProduct(beelineDirection);
    
    intention.setTurn(turnSign * distanceOff * 0.5);
    
    detectSittingStill();
    
    return intention;
}

void WeightedDriver::detectSittingStill() {
    Object::Player *player = getPlayer();
    
    // if sitting still for too long, request a warp
    intention.setReset(false);
    if(player->getPhysicalObject()->getLinearVelocity().length() < 0.05) {
        unsigned long now = Misc::Sleeper::getTimeMilliseconds();
        
        if(!sittingStill) {
            sittingStill = true;
            sittingStillSince = now;
        }
        else if(sittingStillSince + 3000 < now) {
            LOG(PHYSICS, "Player " << player->getID() << " is trying to reset");
            
            intention.setReset(true);
            
            sittingStill = false;
        }
    }
    else sittingStill = false;
}

}  // namespace AI
}  // namespace Project
