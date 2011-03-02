#ifndef PROJECT_AI__DIRECTIONAL_DRIVER_H
#define PROJECT_AI__DIRECTIONAL_DRIVER_H

#include "Driver.h"

namespace Project {
namespace AI {

class DirectionalDriver : public Driver {
private:
    World::PlayerIntention intention;
    bool sittingStill;
    unsigned long sittingStillSince;
public:
    DirectionalDriver(Object::Player *player);
    
    virtual const World::PlayerIntention &getAction();
private:
    void detectSittingStill();
};

}  // namespace AI
}  // namespace Project

#endif
