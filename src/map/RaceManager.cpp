#include "RaceManager.h"
#include "log/Logger.h"

#include "config.h"

namespace Project {
namespace Map {

Math::Point RaceManager::startingPointForPlayer(int id) {
    const std::vector<Math::Vertex3D *> &data = map->getStartPoints();
    
    if(data.size() == 0) {
        LOG(GLOBAL, "No starting points in map, placing player "
            << id << " at default position");
        return INITIAL_CAR_LOCATION;
    }
    
    return data[id % data.size()]->getPosition();
}

}  // namespace Map
}  // namespace Project
