#include "RaceManager.h"
#include "log/Logger.h"

#include "config.h"

namespace Project {
namespace Map {

Math::Point RaceManager::startingPlayerDirection() {
	return map->getFinishPlane().getNormal();
}

Math::Point RaceManager::startingPointForPlayer(int id) {

    const std::vector<Math::Vertex3D *> &data = map->getStartPoints();
    
    if(data.size() == 0) {
        LOG(GLOBAL, "No starting points in map, placing player "
            << id << " at default position");
        return INITIAL_CAR_LOCATION;
    }

	//Start a little higher than the actual start point to avoid starting inside the track
	Math::Point offset = Math::Point(0.0, 0.5, 0.0);
    
    return data[id % data.size()]->getPosition()+offset;
}

}  // namespace Map
}  // namespace Project
