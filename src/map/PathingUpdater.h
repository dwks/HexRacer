#ifndef PROJECT_MAP__PATHING_UPDATER_H
#define PROJECT_MAP__PATHING_UPDATER_H

#include "object/WorldManager.h"
#include "RaceManager.h"

namespace Project {
namespace Map {

class PathingUpdater {
private:
    Object::WorldManager *worldManager;
    RaceManager *raceManager;
public:
    PathingUpdater(Object::WorldManager *worldManager,
        RaceManager *raceManager)
        : worldManager(worldManager), raceManager(raceManager) {}
    
    void update();
};

}  // namespace Map
}  // namespace Project

#endif
