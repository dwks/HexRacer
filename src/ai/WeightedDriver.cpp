#include <cstdlib>
#include "WeightedDriver.h"

#include "math/Point.h"
#include "math/Values.h"
#include "math/Geometry.h"

namespace Project {
namespace AI {

WeightedDriver::WeightedDriver(Object::Player *player) : Driver(player) {
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
    
    return intention;
}

}  // namespace AI
}  // namespace Project
