#ifndef PROJECT_AI__AIMANAGER_H
#define PROJECT_AI__AIMANAGER_H

#include <vector>
#include "boost/shared_ptr.hpp"

#include "map/RaceManager.h"
#include "map/PathManager.h"
#include "world/PlayerManager.h"
#include "paint/PaintManager.h"
#include "world/WorldSetup.h"

#include "event/Enabler.h"
#include "event/PhysicsTick.h"

#include "Driver.h"

namespace Project {
namespace AI {

class AIManager : public Event::Enabler {
protected:
    void physicsTickHandler(Event::PhysicsTick *event);
private:
    typedef std::vector<boost::shared_ptr<Driver> > driverListType;
    driverListType driverList;
private:
    Map::RaceManager *raceManager;
    Map::PathManager *pathManager;
    World::PlayerManager *playerManager;
	Paint::PaintManager *paintManager;
public:
    AIManager(Map::RaceManager *raceManager, Map::PathManager *pathManager,
		World::PlayerManager *playerManager, Paint::PaintManager* paintManager);
    
    /** Returns number of AIs created.
    */
    int createAIs(World::WorldSetup *worldSetup);
};

}  // namespace AI
}  // namespace Project

#endif
