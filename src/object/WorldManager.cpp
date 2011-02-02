#include "WorldManager.h"
#include "config.h"

#include "event/ObserverList.h"

#include "physics/Converter.h"

namespace Project {
namespace Object {

void WorldManager::CreateObjectHandler
    ::observe(Event::CreateObject *createObject) {
    
    worldManager->getWorld()->addObject(createObject->getObject());
}

void WorldManager::DestroyObjectHandler
    ::observe(Event::DestroyObject *destroyObject) {
    
    worldManager->getWorld()->removeObject(
        worldManager->getWorld()->getObject(destroyObject->getID()));
}

void WorldManager::UpdateObjectHandler
    ::observe(Event::UpdateObject *updateObject) {
    
    Object::ObjectBase *object
        = worldManager->getWorld()->getObject(updateObject->getID());
    
    object->getPhysicalObject()->setData(
        updateObject->getTransformation(),
        updateObject->getLinearVelocity(),
        updateObject->getAngularVelocity());
}

WorldManager::WorldManager() {
    world = new World();
    playerList = new PlayerList();
    
    ADD_OBSERVER(new CreateObjectHandler(this));
    ADD_OBSERVER(new DestroyObjectHandler(this));
    ADD_OBSERVER(new UpdateObjectHandler(this));
}

WorldManager::WorldManager(World *world, PlayerList *playerList)
    : world(world), playerList(playerList) {
    
}

WorldManager::~WorldManager() {
    delete world;
    delete playerList;
}

Player *WorldManager::getPlayer(int id) {
    return playerList->getPlayer(id);
}

void WorldManager::usePlayerList(PlayerList *playerList) {
    PlayerList::IteratorType iterator = this->playerList->getIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        world->removeObject(player);
    }
    // the players are deleted by the PlayerList's destructor
    delete this->playerList;
    
    // now add the new player list
    this->playerList = playerList;
    iterator = playerList->getIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        world->addObject(player);
    }
}

void WorldManager::initForClient(int id, const Math::Point &location) {
    Object::Player *player = new Object::Player(id, location);
    
    playerList->addPlayer(player);
    world->addObject(player);
}

}  // namespace Object
}  // namespace Project
