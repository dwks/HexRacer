#include "RaceManager.h"
#include "log/Logger.h"

#include "settings/SettingsManager.h"
#include "config.h"

namespace Project {
namespace Map {

RaceManager::RaceManager(HRMap *_map)
: map(_map) {
	finishPlane = map->getFinishPlane();
}

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
    double normalSpringOffset = GET_SETTING("physics.driving.restlength", 0.5);
    normalSpringOffset += 0.05;  // tweaking
	Math::Point offset = Math::Point(0.0, normalSpringOffset, 0.0);
    
    return data[id % data.size()]->getPosition()+offset;
}

}  // namespace Map
}  // namespace Project
