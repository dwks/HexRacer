#ifndef PROJECT_MAP__WARP_DETECTOR_H
#define PROJECT_MAP__WARP_DETECTOR_H

#include "object/Player.h"
#include "RaceManager.h"

namespace Project {
namespace Map {

class WarpDetector {
private:
    RaceManager *raceManager;
public:
    WarpDetector(RaceManager *raceManager) : raceManager(raceManager) {}
    
    void checkForWarping(Object::Player *player);
};

}  // namespace Map
}  // namespace Project

#endif
