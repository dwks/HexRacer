#ifndef PROJECT_MAP__RACE_MANAGER_H
#define PROJECT_MAP__RACE_MANAGER_H

#include "HRMap.h"

namespace Project {
namespace Map {

class RaceManager {
private:
    HRMap *map;
	Math::BoundingPlane3D finishPlane;
public:
    RaceManager(HRMap *_map);
    
	Math::Point startingPlayerDirection();
    Math::Point startingPointForPlayer(int id);
	const Math::BoundingPlane3D& getBoundingPlane() { return finishPlane; }
};

}  // namespace Map
}  // namespace Project

#endif
