#ifndef PROJECT_AI__DRIVER_H
#define PROJECT_AI__DRIVER_H

#include "world/PlayerIntention.h"
#include "object/Player.h"

namespace Project {
namespace AI {

class Driver {
private:
    Object::Player *player;
public:
    Driver(Object::Player *player) : player(player) {}
    virtual ~Driver() {}
    
    virtual Object::Player *getPlayer() const { return player; }
    
    virtual const World::PlayerIntention &getAction() = 0;
};

}  // namespace AI
}  // namespace Project

#endif
