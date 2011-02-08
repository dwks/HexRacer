#ifndef PROJECT_MAP__RACE_MANAGER_H
#define PROJECT_MAP__RACE_MANAGER_H

#include "HRMap.h"

namespace Project {
namespace Map {

class RaceManager {
private:
    HRMap *map;
	Math::BoundingPlane3D finishPlane;
	double killPlaneY;
public:
    RaceManager(HRMap *_map);
    
	Math::Point startingPlayerDirection();
    Math::Point startingPointForPlayer(int id);
	const Math::BoundingPlane3D& getBoundingPlane() { return finishPlane; }
	double getKillPlaneY() { return killPlaneY; }
};

}  // namespace Map
}  // namespace Project

#endif
