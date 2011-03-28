#ifndef PROJECT_AI__WEIGHTED_DRIVER_H
#define PROJECT_AI__WEIGHTED_DRIVER_H

#include "Driver.h"
#include "map/PathNode.h"
#include "paint/PaintManager.h"

namespace Project {
namespace AI {

class WeightedDriver : public Driver {
private:
    bool sittingStill;
    unsigned long sittingStillSince;
    World::PlayerIntention intention;
	const Map::PathNode* nearestPathNode;
	unsigned long paintSwitchTime;
	Paint::PaintManager* paintManager;
public:
    WeightedDriver(Object::Player *player);
    
    virtual const World::PlayerIntention &getAction();
	void setPaintManager(Paint::PaintManager* manager) { paintManager = manager; }
private:
    void detectSittingStill();
	void detectPaintAhead();
};

}  // namespace AI
}  // namespace Project

#endif
