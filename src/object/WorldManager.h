#ifndef PROJECT_OBJECT__WORLD_MANAGER_H
#define PROJECT_OBJECT__WORLD_MANAGER_H

#include "World.h"
#include "Player.h"
#include "PlayerList.h"

#include "map/PathManager.h"

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
	Map::PathManager *pathManager;
public:
    typedef PlayerList::IteratorType PlayerIteratorType;
public:
    WorldManager();
    WorldManager(World *world, PlayerList *playerList);
    ~WorldManager();
    
    void clearPlayerList();
    
    World *getWorld() { return world; }
    PlayerList *getPlayerList() { return playerList; }
    
    void addPlayer(Player *player);
    Player *getPlayer(int id);
    
	void initForClient(int id, const Math::Point &location, const Math::Point& direction);
	void setPathManager(Map::PathManager* manager) { pathManager = manager; }
	Map::PathManager *getPathManager() { return pathManager; }
    
    PlayerIteratorType getPlayerIterator()
        { return playerList->getIterator(); }
};

}  // namespace Object
}  // namespace Project

#endif
