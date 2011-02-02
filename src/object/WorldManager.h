#ifndef PROJECT_OBJECT__WORLD_MANAGER_H
#define PROJECT_OBJECT__WORLD_MANAGER_H

#include "World.h"
#include "Player.h"
#include "PlayerList.h"

#include "event/CreateObject.h"
#include "event/DestroyObject.h"
#include "event/UpdateObject.h"
#include "event/TypedObserver.h"

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
    class CreateObjectHandler
        : public Event::TypedObserver<Event::CreateObject> {
    private:
        WorldManager *worldManager;
    public:
        CreateObjectHandler(WorldManager *worldManager)
            : worldManager(worldManager) {}
        
        virtual void observe(Event::CreateObject *createObject);
    };
    
    class DestroyObjectHandler
        : public Event::TypedObserver<Event::DestroyObject> {
    private:
        WorldManager *worldManager;
    public:
        DestroyObjectHandler(WorldManager *worldManager)
            : worldManager(worldManager) {}
        
        virtual void observe(Event::DestroyObject *destroyObject);
    };
    
    class UpdateObjectHandler
        : public Event::TypedObserver<Event::UpdateObject> {
    private:
        WorldManager *worldManager;
    public:
        UpdateObjectHandler(WorldManager *worldManager)
            : worldManager(worldManager) {}
        
        virtual void observe(Event::UpdateObject *updateObject);
    };
public:
    WorldManager();
    WorldManager(World *world, PlayerList *playerList);
    ~WorldManager();
    
    World *getWorld() { return world; }
    PlayerList *getPlayerList() { return playerList; }
    
    Player *getPlayer(int id);
    
    void usePlayerList(PlayerList *playerList);
    void initForClient(int id, const Math::Point &location);
};

}  // namespace Object
}  // namespace Project

#endif
