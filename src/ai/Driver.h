#ifndef PROJECT_AI__DRIVER_H
#define PROJECT_AI__DRIVER_H

#include "world/PlayerIntention.h"

namespace Project {
namespace AI {

class Driver {
public:
    virtual ~Driver() {}
    
    virtual const World::PlayerIntention &getAction() = 0;
};

}  // namespace AI
}  // namespace Project

#endif
