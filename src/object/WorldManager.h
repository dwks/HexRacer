#ifndef PROJECT_OBJECT__WORLD_MANAGER_H
#define PROJECT_OBJECT__WORLD_MANAGER_H

#include "World.h"
#include "Player.h"
#include "PlayerList.h"

namespace Project {
namespace Object {

/** Maintains the current World.
    
    Note that the currently active World object can change, so pointers to the
    World should not be cached.
*/
class WorldManager {
private:
    World *world;
    PlayerList *playerList;
public:
    WorldManager();
    WorldManager(World *world, PlayerList *playerList);
    
    World *getWorld() { return world; }
    
    Player *getPlayer(int id);
};

}  // namespace Object
}  // namespace Project

#endif
