#ifndef PROJECT_AI__AIMANAGER_H
#define PROJECT_AI__AIMANAGER_H

#include <vector>
#include "boost/shared_ptr.hpp"

#include "map/RaceManager.h"
#include "map/PathManager.h"

#include "Driver.h"

namespace Project {
namespace AI {

class AIManager {
private:
    typedef std::vector<boost::shared_ptr<Driver> > driverListType;
    driverListType driverList;
private:
    Map::RaceManager *raceManager;
    Map::PathManager *pathManager;
public:
    AIManager(Map::RaceManager *raceManager, Map::PathManager *pathManager)
        : raceManager(raceManager), pathManager(pathManager) {}
    
    void createAIs(int count);
};

}  // namespace AI
}  // namespace Project

#endif
