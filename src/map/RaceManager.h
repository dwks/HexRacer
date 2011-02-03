#ifndef PROJECT_MAP__RACE_MANAGER_H
#define PROJECT_MAP__RACE_MANAGER_H

#include "HRMap.h"

namespace Project {
namespace Map {

class RaceManager {
private:
    HRMap *map;
public:
    RaceManager(HRMap *map) : map(map) {}
    
	Math::Point startingPlayerDirection();
    Math::Point startingPointForPlayer(int id);
};

}  // namespace Map
}  // namespace Project

#endif
