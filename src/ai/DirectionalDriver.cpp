#include <cstdlib>
#include "DirectionalDriver.h"

#include "math/Point.h"
#include "math/Values.h"
#include "math/Geometry.h"

namespace Project {
namespace AI {

DirectionalDriver::DirectionalDriver(Object::Player *player) : Driver(player) {
    intention.setAccel(1.0);
    intention.setPaint(true);
}

const World::PlayerIntention &DirectionalDriver::getAction() {
    Object::Player *player = getPlayer();
    const Map::PathNode *current = player->getPathTracker()->getCurrentNode();
    
    Map::PathNode *next;
    
    // find the next node
    int random = player->getID() % current->getNextNodes().size();
    next = current->getNextNodes()[random];
    
    Math::Point playerFacing = player->getPhysicalObject()->getFrontDirection();
    Math::Point playerRight = player->getPhysicalObject()->getRightDirection();
    
    Math::Point pathDirection = next->getPosition() - current->getPosition();
    Math::Point beelineDirection = next->getPosition() - player->getPosition();
    beelineDirection.normalize();
    pathDirection.normalize();
    
    double offAngle = playerRight.dotProduct(pathDirection);
    double turnSign = -Math::sign(offAngle);
    //bool onLeftSide = (offAngle < 0.0);
    
    Math::Point intersection = Math::Geometry::intersectLine3D(
        current->getPosition(),
        next->getPosition(),
        player->getPosition());
    
    double distanceOff = (intersection - player->getPosition()).length();
    
    /*intention.setTurn(0.0);
    if(distanceOff > 3.0) {
        intention.setTurn(turnSign * 1.0);
    }
    else if(distanceOff > 2.0) {
        intention.setTurn(turnSign * 0.5);
    }
    else if(distanceOff > 1.0) {
        intention.setTurn(turnSign * 0.3);
    }
    else if(distanceOff > 0.5) {
        intention.setTurn(turnSign * 0.1);
    }*/
    
    intention.setTurn(turnSign * distanceOff * 0.2);
    
    return intention;
}

}  // namespace AI
}  // namespace Project
