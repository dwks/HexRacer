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
	Map::PathNode *previous;
    
    // find the next node
	if (current->getProgress() < player->getPathTracker()->getLapProgress()) {
		int random = player->getID() % current->getNextNodes().size();
		current = current->getNextNodes()[random];
	}

	previous = current->getPreviousNodes()[0];

	int random = player->getID() % current->getNextNodes().size();
    next = current->getNextNodes()[random];
	random = player->getID() % next->getNextNodes().size();
    next2 = next->getNextNodes()[random];
	
    
    // calculate various vectors
    Math::Point playerFacing = player->getPhysicalObject()->getFrontDirection();
    Math::Point playerRight = player->getPhysicalObject()->getRightDirection();

	Math::Point playerPos = player->getPosition();

	double playerSpeed = player->getPhysicalObject()->getLinearSpeed();
    
	double beeline_u = 1.0-Math::bound(
		playerPos.distance(current->getPosition())*GET_SETTING("ai.pathlookaheadfactor", 0.1),
		0.0, 1.0);

	Math::Point beelinePoint = next->getPosition()*(1.0-beeline_u)+next2->getPosition()*beeline_u;
	Math::Point beelineDirection = (beelinePoint-playerPos).normalized();

	Math::Point pathDirection = (current->getPosition() - previous->getPosition()).normalized() * 0.5
		+ (next->getPosition() - current->getPosition()).normalized() * 0.5;
	pathDirection.normalize();
    
    // compute error due to being far away from this path edge
    double pathTurnSign = -Math::sign(playerRight.dotProduct(pathDirection));

	double distanceOff = player->getPathTracker()->getProgressPosition().distance(playerPos);
	distanceOff *= GET_SETTING("ai.pathdistancefactor", 1.0);

	double angleTurnSign = -Math::sign(playerRight.dotProduct(beelineDirection));

	double beelineAngle = Math::Geometry::vectorTo2DAngle(beelineDirection, Math::Y_AXIS);
	double facingAngle = Math::Geometry::vectorTo2DAngle(playerFacing, Math::Y_AXIS);

	double ccwAngle = std::fabs(beelineAngle-facingAngle);
	double cwAngle = std::fabs(Math::minimum(beelineAngle, facingAngle)+((PI*2.0)-Math::maximum(beelineAngle, facingAngle)));
	double offAngle = Math::minimum(ccwAngle, cwAngle);

	double offAngleFactor = (offAngle/PI) - GET_SETTING("ai.minangleoffset", 0.1);
	double angleFactor;

	if (offAngleFactor < 0.0) {
		offAngleFactor = 0.0;
		angleFactor = 0.0;
	}
	else
		angleFactor = offAngleFactor*GET_SETTING("ai.pathanglefactor", 1.0);
    
    
    // compute error due to facing in the wrong direction
    //double beelineAngle = playerRight.dotProduct(beelineDirection);

	double slowDownFactor = playerSpeed-GET_SETTING("ai.minslowdownspeed", 10.0);
	if (slowDownFactor < 0.0)
		slowDownFactor = 0.0;
	else
		slowDownFactor *= GET_SETTING("ai.angleslowdownfactor", 1.0);
    
	intention.setTurn(Math::bound((pathTurnSign * distanceOff) + (angleTurnSign * angleFactor), -1.0, 1.0));
	intention.setAccel(Math::bound(2.0-(offAngleFactor*slowDownFactor), -1.0, 1.0));
    
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
		&& player->getPhysicalObject()->getLinearSpeed() < 4.0) {
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

	double paintLookAhead = GET_SETTING("ai.paintlookahead", 2.0);
	double minLookAhead = GET_SETTING("ai.minlookahead", 2.0);

	Math::Point headingDir = getPlayer()->getPhysicalObject()->getLinearVelocity()*paintLookAhead;
	double dist = getPlayer()->getPhysicalObject()->getLinearSpeed()*headingDir.length();

	if (dist < minLookAhead) {
		headingDir /= dist;
		headingDir *= minLookAhead;
	}

	Math::Point paintQueryPoint = getPlayer()->getPosition()+headingDir;
	
	double paintScore = paintManager->weightedCellsInRadius(paintQueryPoint, PAINTING_RADIUS, getPlayer()->getTeamID());

	bool considering_change = false;
	bool change_to_paint = false;
	bool change_to_erase = false;

	double paintThresh = GET_SETTING("ai.stoppaintingthreshhold", 1.2);
	double eraseThresh =  GET_SETTING("ai.starterasingthreshhold", 0.9);

	if (!intention.getPaint()) {
		//Not painting: Change to painting
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
	
	if (!considering_change && intention.getPaint() ) {
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
