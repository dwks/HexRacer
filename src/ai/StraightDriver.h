#ifndef PROJECT_AI__STRAIGHT_DRIVER_H
#define PROJECT_AI__STRAIGHT_DRIVER_H

#include "Driver.h"

namespace Project {
namespace AI {

class StraightDriver : public Driver {
private:
    World::PlayerIntention intention;
public:
    StraightDriver(Object::Player *player);
    
    virtual const World::PlayerIntention &getAction();
};

}  // namespace AI
}  // namespace Project

#endif
