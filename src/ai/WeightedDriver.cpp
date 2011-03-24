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

	paintManager = NULL;
    
    sittingStill = false;
    sittingStillSince = 0;
	paintSwitchTime = 0;

    intention.setAccel(1.0);
    //intention.setPaint(true);
}

const World::PlayerIntention &WeightedDriver::getAction() {
    Object::Player *player = getPlayer();
    const Map::PathNode *current = player->getPathTracker()->getCurrentNode();
    
    Map::PathNode *next;
	Map::PathNode *next2;
    
    // find the next node
    int random = player->getID() % current->getNextNodes().size();
    next = current->getNextNodes()[random];
	next2 = next->getNextNodes()[0];
    
    // calculate various vectors
    Math::Point playerFacing = player->getPhysicalObject()->getFrontDirection();
    Math::Point playerRight = player->getPhysicalObject()->getRightDirection();
    
    Math::Point pathDirection = next->getPosition() - current->getPosition();

	double beeline_u = 1.0-Math::bound(
		player->getPosition().distance(next->getPosition())/20.0,
		0.0, 1.0);
	Math::Point beelinePoint = next->getPosition()*(1.0-beeline_u)+next2->getPosition()*beeline_u;
	Math::Point beelineDirection = (beelinePoint-player->getPosition()).normalized();
    pathDirection.normalize();
    
    // compute error due to being far away from this path edge
    double turnSign = -Math::sign(playerRight.dotProduct(pathDirection));

	//double turnSign;

	double beelineAngle = Math::Geometry::vectorTo2DAngle(beelineDirection, Math::Y_AXIS);
	double facingAngle = Math::Geometry::vectorTo2DAngle(playerFacing, Math::Y_AXIS);

	double ccwAngle = std::fabs(beelineAngle-facingAngle);
	double cwAngle = std::fabs(Math::minimum(beelineAngle, facingAngle)+((PI*2.0)-Math::maximum(beelineAngle, facingAngle)));
	double offAngle = Math::minimum(ccwAngle, cwAngle);

	/*
	if (cwAngle > ccwAngle)
		turnSign = 1.0;
	else
		turnSign =-1.0;
	*/

	double angleFactor = 1.0+offAngle/PI*GET_SETTING("ai.pathanglefactor", 1.0);
    
	/*
    Math::Point intersection = Math::Geometry::intersectLine3D(
        current->getPosition(),
        next->getPosition(),
        player->getPosition());
		*/
    
	double distanceOff = (player->getPathTracker()->getProgressPosition() - player->getPosition()).length();
	distanceOff *= GET_SETTING("ai.pathdistancefactor", 1.0);
    
    // compute error due to facing in the wrong direction
    //double beelineAngle = playerRight.dotProduct(beelineDirection);
    
    intention.setTurn(turnSign * distanceOff * angleFactor * 0.5);
	intention.setAccel(Math::bound(1.0-(offAngle/PI*2.0), 0.25, 1.0));
    
    detectSittingStill();
	detectPaintAhead();
    
    return intention;
}

void WeightedDriver::detectSittingStill() {
    Object::Player *player = getPlayer();
    
    // if sitting still for too long, request a warp
    intention.setReset(false);

	const Map::PathNode* last_pathnode = nearestPathNode;
	nearestPathNode = player->getPathTracker()->getCurrentNode();

	if (nearestPathNode != NULL && nearestPathNode == last_pathnode
		&& player->getPhysicalObject()->getLinearVelocity().lengthSquared() < 4.0) {
        unsigned long now = Misc::Sleeper::getTimeMilliseconds();
        
        if(!sittingStill) {
            sittingStill = true;
            sittingStillSince = now;
        }
        else if(sittingStillSince + GET_SETTING("ai.resettimer", 3000) < now) {
            LOG(PHYSICS, "Player " << player->getID() << " is trying to reset");
            
            intention.setReset(true);
            
            sittingStill = false;
        }
    }
    else sittingStill = false;
}

void WeightedDriver::detectPaintAhead() {

	if (!paintManager)
		return;
	
	Math::Point paintQueryPoint = 
		getPlayer()->getPosition()+getPlayer()->getPhysicalObject()->getLinearVelocity()*GET_SETTING("ai.paintlookahead", 2.0);
	
	double paintScore = paintManager->weightedCellsInRadius(paintQueryPoint, PAINTING_RADIUS, getPlayer()->getTeamID());

	bool considering_change = false;
	bool change_to_paint = false;
	bool change_to_erase = false;

	double paintThresh = GET_SETTING("ai.stoppaintingthreshhold", 1.2);
	double eraseThresh =  GET_SETTING("ai.starterasingthreshhold", 0.9);

	if (!intention.getPaint() && !intention.getErase()) {
		//Not painting or erasing: Change to painting
		if (paintScore > eraseThresh && paintScore < paintThresh) {
			considering_change = true;
			change_to_paint = true;
		}
	}
	
	if (!considering_change && !intention.getErase()) {
		//Not erasing: Change to erasing
		if (paintScore <= eraseThresh) {
			considering_change = true;
			change_to_erase = true;
		}
	}
	else if (!considering_change && intention.getPaint() ) {
		//Is painting: Change to not painting
		considering_change = (paintScore >= paintThresh);
	}

	unsigned long now = Misc::Sleeper::getTimeMilliseconds();

	if (considering_change) {

		if (paintSwitchTime + GET_SETTING("ai.paintswitchdelay", 500) < now) {

			if (change_to_paint) {
				intention.setPaint(true);
				intention.setErase(false);
			}
			else if (change_to_erase) {
				intention.setPaint(false);
				intention.setErase(true);
			}
			else {
				intention.setPaint(false);
				intention.setErase(false);
			}
			
			paintSwitchTime = now;
		}

	}
	else
		paintSwitchTime = now;

}

}  // namespace AI
}  // namespace Project
