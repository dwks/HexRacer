#include "WorldManager.h"
#include "config.h"

#include "event/CreateObject.h"
#include "event/DestroyObject.h"
#include "event/UpdateObject.h"
#include "event/UpdateWorld.h"

#include "event/ObserverList.h"

#include "physics/Converter.h"
#include "map/PathTracker.h"

namespace Project {
namespace Object {

void WorldManager::WorldHandler::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::CREATE_OBJECT: {
        Event::CreateObject *createObject
            = dynamic_cast<Event::CreateObject *>(event);
        
        if(dynamic_cast<Player *>(createObject->getObject())) {
            worldManager->addPlayer(
                dynamic_cast<Player *>(createObject->getObject()));
        }
        else {
            worldManager->getWorld()->addObject(createObject->getObject());
        }
        break;
    }
    case Event::EventType::DESTROY_OBJECT: {
        Event::DestroyObject *destroyObject
            = dynamic_cast<Event::DestroyObject *>(event);
        
        worldManager->getWorld()->removeObject(
            worldManager->getWorld()->getObject(destroyObject->getID()));
        
        break;
    }
    case Event::EventType::UPDATE_OBJECT: {
        Event::UpdateObject *updateObject
            = dynamic_cast<Event::UpdateObject *>(event);
        
        Object::ObjectBase *object
            = worldManager->getWorld()->getObject(updateObject->getID());
        
        object->getPhysicalObject()->setData(
            updateObject->getTransformation(),
            updateObject->getLinearVelocity(),
            updateObject->getAngularVelocity());
        
        break;
    }
    case Event::EventType::UPDATE_WORLD: {
        Event::UpdateWorld *updateWorld
            = dynamic_cast<Event::UpdateWorld *>(event);
        
        std::vector<Event::UpdateObject *> &vector = updateWorld->getVector();
        for(int x = 0; x < int(vector.size()); x ++) {
            Event::UpdateObject *updateObject = vector[x];
            
            Object::ObjectBase *object
                = worldManager->getWorld()->getObject(updateObject->getID());
            
            //LOG(WORLD, "Updating " << updateObject->getID() << ", " << object);
            
            // we don't know about this object
            if(!object) continue;
            
            object->getPhysicalObject()->setData(
                updateObject->getTransformation(),
                updateObject->getLinearVelocity(),
                updateObject->getAngularVelocity());
        }
        
        break;
    }
    default:
        break;
    }
}

bool WorldManager::WorldHandler::interestedIn(Event::EventType::type_t type) {
    switch(type) {
    case Event::EventType::CREATE_OBJECT:
    case Event::EventType::DESTROY_OBJECT:
    case Event::EventType::UPDATE_OBJECT:
    case Event::EventType::UPDATE_WORLD:
        return true;
    default:
        break;
    }
    
    return false;
}

WorldManager::WorldManager() {
    world = new World();
    playerList = new PlayerList(this);
    
    ADD_OBSERVER(new WorldHandler(this));
}

WorldManager::WorldManager(World *world, PlayerList *playerList)
    : world(world), playerList(playerList) {
    
    playerList->setWorldManager(this);
    
    ADD_OBSERVER(new WorldHandler(this));
}

WorldManager::~WorldManager() {
    delete world;
    delete playerList;
}

void WorldManager::addPlayer(Player *player) {
    if(world->getObject(player->getID())) return;
    
    world->addObject(player);
    playerList->addPlayer(player);

	if (pathManager)
		player->setPathTracker(new Map::PathTracker(*pathManager));
}

Player *WorldManager::getPlayer(int id) {
    return dynamic_cast<Player *>(world->getObject(id));
}

void WorldManager::initForClient(int id, const Math::Point &location, const Math::Point& direction) {
    Object::Player *player = new Object::Player(id, location, direction);
    addPlayer(player);
}

}  // namespace Object
}  // namespace Project
