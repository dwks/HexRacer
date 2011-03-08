#ifndef PROJECT_AI__WEIGHTED_DRIVER_H
#define PROJECT_AI__WEIGHTED_DRIVER_H

#include "Driver.h"

namespace Project {
namespace AI {

class WeightedDriver : public Driver {
private:
    bool sittingStill;
    unsigned long sittingStillSince;
    World::PlayerIntention intention;
public:
    WeightedDriver(Object::Player *player);
    
    virtual const World::PlayerIntention &getAction();
private:
    void detectSittingStill();
};

}  // namespace AI
}  // namespace Project

#endif