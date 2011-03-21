#include "RaceManager.h"
#include "log/Logger.h"
#include "misc/StdVectorFunctions.h"

#include "settings/SettingsManager.h"
#include "config.h"

namespace Project {
namespace Map {

RaceManager::RaceManager(HRMap *_map)
: map(_map) {

	finishPlane = map->getFinishPlane();
	numLaps = map->getMapOptions().getNumLaps();

	killPlaneY = 0.0;
	
	//Find the lowest path node, set the kill plane to that plus an offset
	const vector<PathNode*>& path_nodes = map->getMapObjects().getPathNodes();
	for (unsigned int i = 0; i < path_nodes.size(); i++) {
		if (i == 0 || path_nodes[i]->getPosition().getY() < killPlaneY) {
			killPlaneY = path_nodes[i]->getPosition().getY();
		}
	}

	killPlaneY += MAP_KILL_PLANE_Y_GAP;

}

Math::Point RaceManager::startingPlayerDirection() {
	return map->getFinishPlane().getNormal();
}

Math::Point RaceManager::startingPointForPlayer(int id) {

    const std::vector<Math::Vertex3D *> &data = map->getMapObjects().getStartPoints();
    
    if(data.size() == 0) {
        LOG(GLOBAL, "No starting points in map, placing player "
            << id << " at default position");
        return INITIAL_CAR_LOCATION;
    }

	//Start a little higher than the actual start point to avoid starting inside the track
    double normalSpringOffset = GET_SETTING("physics.driving.restlength", 0.5);
    normalSpringOffset += VEHICLE_WARP_Y_OFFSET;
	Math::Point offset = Math::Point(0.0, normalSpringOffset, 0.0);
    
    return data[id % data.size()]->getPosition()+offset;
}

bool RaceManager::inBounds(const Math::Point& position) const {
	return (position.getY() > killPlaneY);
}

void RaceManager::updatePlayerRankings(Object::WorldManager* world) {

	playerRankings.clear();

	Object::WorldManager::PlayerIteratorType it
		= world->getPlayerIterator();
	while(it.hasNext()) {
		Object::Player* player = it.next();

		if (!player->getPathTracker()->getFinished() && player->getPathTracker()->getNumLaps() >= numLaps) {
			player->getPathTracker()->setFinished(true);
		}

		playerRankings.push_back(player);
	}

	Misc::vectorPointerMergeSort(playerRankings);

	for (unsigned int i = 0; i < playerRankings.size(); i++) {
		playerRankings[i]->getPathTracker()->setRanking(i);
	}

}
}  // namespace Map
}  // namespace Project
