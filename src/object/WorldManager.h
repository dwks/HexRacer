#ifndef PROJECT_OBJECT__WORLD_MANAGER_H
#define PROJECT_OBJECT__WORLD_MANAGER_H

#include "World.h"
#include "Player.h"
#include "PlayerList.h"

#include "event/MultiObserver.h"

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
private:
    class WorldHandler : public Event::MultiObserver {
    private:
        WorldManager *worldManager;
    public:
        WorldHandler(WorldManager *worldManager)
            : worldManager(worldManager) {}
        
        virtual void observe(Event::EventBase *event);
        virtual bool interestedIn(Event::EventType::type_t type);
    };
public:
    typedef PlayerList::IteratorType PlayerIteratorType;
public:
    WorldManager();
    WorldManager(World *world, PlayerList *playerList);
    ~WorldManager();
    
    World *getWorld() { return world; }
    PlayerList *getPlayerList() { return playerList; }
    
    void addPlayer(Player *player);
    Player *getPlayer(int id);
    
    void initForClient(int id, const Math::Point &location);
    
    PlayerIteratorType getPlayerIterator()
        { return playerList->getIterator(); }
};

}  // namespace Object
}  // namespace Project

#endif
