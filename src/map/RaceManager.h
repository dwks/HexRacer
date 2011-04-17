#ifndef PROJECT_MAP__RACE_MANAGER_H
#define PROJECT_MAP__RACE_MANAGER_H

#include "HRMap.h"
#include "object/WorldManager.h"
#include "object/Player.h"
#include "map/RaceResults.h"

namespace Project {
namespace Map {

class RaceManager {
private:
    HRMap *map;
	Math::BoundingPlane3D finishPlane;
	double killPlaneY;
	int numLaps;
	std::vector<Object::Player*> playerRankings;
    mutable unsigned long whenToFinish;
public:
    RaceManager(HRMap *_map);
    
	Math::Point startingPlayerDirection();
    Math::Point startingPointForPlayer(int id);
	const Math::BoundingPlane3D& getBoundingPlane() const { return finishPlane; }
	double getKillPlaneY() const { return killPlaneY; }
	void updatePlayerRankings(Object::WorldManager* world);
	const std::vector<Object::Player*>& getPlayerRankings() const { return playerRankings; }
	bool getRaceFinished(Object::WorldManager* world) const;
	RaceResults getRaceResults() const;

	int getNumLapsToWin() const { return numLaps; }

	bool inBounds(const Math::Point& position) const;
};

}  // namespace Map
}  // namespace Project

#endif
