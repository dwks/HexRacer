#ifndef PROJECT_MAP__PATHING_UPDATER_H
#define PROJECT_MAP__PATHING_UPDATER_H

#include "boost/shared_ptr.hpp"

#include "object/WorldManager.h"
#include "RaceManager.h"
#include "WarpDetector.h"

namespace Project {
namespace Map {

class PathingUpdater {
private:
    boost::shared_ptr<Object::WorldManager> worldManager;
    boost::shared_ptr<RaceManager> raceManager;
    WarpDetector *warpDetector;
public:
    PathingUpdater(boost::shared_ptr<Object::WorldManager> worldManager,
        boost::shared_ptr<RaceManager> raceManager);
    ~PathingUpdater();
    
    void update();
};

}  // namespace Map
}  // namespace Project

#endif
