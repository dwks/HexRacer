#include "Historian.h"

#include "event/CreateObject.h"
#include "event/DestroyObject.h"
#include "event/UpdateObject.h"
#include "event/UpdateWorld.h"
#include "event/ChangeOfIntention.h"

#include "event/EventSystem.h"

#include "settings/ProgramSettings.h"
#include "misc/Sleeper.h"

namespace Project {
namespace History {

void Historian::WorldHandler::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::CREATE_OBJECT: {
        Event::CreateObject *createObject
            = dynamic_cast<Event::CreateObject *>(event);
        
        if(dynamic_cast<Object::Player *>(createObject->getObject())) {
            Object::Player *player
                = dynamic_cast<Object::Player *>(createObject->getObject());
            
            if(Settings::ProgramSettings::getInstance()->isClient()) {
                player->setPathTracker(
                    new Map::PathTracker(*worldManager->getPathManager()));
            }
            
            worldManager->addPlayer(player);
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
        
        historian->handleUpdateWorld(updateWorld);
        if(historian->updateWorldTooOld(updateWorld)) break;
        
        std::vector<Event::UpdateObject *> &vector = updateWorld->getVector();
        for(int x = 0; x < int(vector.size()); x ++) {
            Event::UpdateObject *updateObject = vector[x];
            
            Object::ObjectBase *object
                = worldManager->getWorld()->getObject(updateObject->getID());
            
            //LOG(WORLD, "Updating " << updateObject->getID() << ", " << object);
            
            // we don't know about this object
            if(!object) continue;
            
#if 0
            Math::Point current = object->getPhysicalObject()->getTransformation()
                * Math::Point(0.0, 0.0, 0.0, 1.0);
            Math::Point required = updateObject->getTransformation()
                * Math::Point(0.0, 0.0, 0.0, 1.0);
            
            Math::Matrix transform = updateObject->getTransformation();
            LOG(PHYSICS, "drift from server: " << (current - required).length());
            if((current - required).length() < 0.2) {
                transform = transform
                    * Math::Matrix::getTranslationMatrix(required - current);
            }
#endif
            
            object->getPhysicalObject()->setData(
                //transform,
                updateObject->getTransformation(),
                updateObject->getLinearVelocity(),
                updateObject->getAngularVelocity());
        }
        
        historian->advanceWorld(updateWorld);
        
        break;
    }
    case Event::EventType::CHANGE_OF_INTENTION: {
        Event::ChangeOfIntention *changeOfIntention
            = dynamic_cast<Event::ChangeOfIntention *>(event);
        
        if(changeOfIntention->getIntention().getPaint()) {
            EMIT_EVENT(new Event::TogglePainting(
                changeOfIntention->getPlayer(),
                Event::TogglePainting::PAINTING));
        }
        else if(changeOfIntention->getIntention().getErase()) {
            EMIT_EVENT(new Event::TogglePainting(
                changeOfIntention->getPlayer(),
                Event::TogglePainting::ERASING));
        }
        else {
            EMIT_EVENT(new Event::TogglePainting(
                changeOfIntention->getPlayer(),
                Event::TogglePainting::NOTHING));
        }
        
        /*LOG(NETWORK, "Received change of intention for player "
            << changeOfIntention->getPlayer());*/
        
        break;
    }
    default:
        break;
    }
}

bool Historian::WorldHandler::interestedIn(Event::EventType::type_t type) {
    switch(type) {
    case Event::EventType::CREATE_OBJECT:
    case Event::EventType::DESTROY_OBJECT:
    case Event::EventType::UPDATE_OBJECT:
    case Event::EventType::UPDATE_WORLD:
    case Event::EventType::CHANGE_OF_INTENTION:
        return true;
    default:
        break;
    }
    
    return false;
}

Historian::Historian() {
    pingTime = new PingTimeMeasurer();
}

Historian::~Historian() {
    delete pingTime;
}

void Historian::setWorldManager(Object::WorldManager *worldManager) {
    this->worldManager = worldManager;
    
    ADD_OBSERVER(new WorldHandler(worldManager, this));
}

void Historian::setPhysicsWorld(Physics::PhysicsWorld *physicsWorld) {
    this->physicsWorld = physicsWorld;
}

void Historian::handleUpdateWorld(Event::UpdateWorld *updateWorld) {
    if(false && pingTime->getClockOffset() == PingTimeMeasurer::NO_CLOCK_OFFSET) {
        unsigned long sent = updateWorld->getMilliseconds();
        unsigned long now = Misc::Sleeper::getTimeMilliseconds();
        
        long offset = -long(now - sent);
        
        LOG(NETWORK, "ClockOffset set to " << offset);
        
        pingTime->setClockOffset(offset);
    }
    else {
#if 0
        unsigned long sent = updateWorld->getMilliseconds();
        unsigned long now = Misc::Sleeper::getTimeMilliseconds();
        
        long offset = long(now - sent) + pingTime->getClockOffset();
        
        LOG(NETWORK, "UpdateWorld is " << offset << " milliseconds old");
#endif
    }
}

bool Historian::updateWorldTooOld(Event::UpdateWorld *updateWorld) {
    unsigned long sent = updateWorld->getMilliseconds();
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    long offset = long(now - sent) + pingTime->getClockOffset();
    
    return offset > 10000;
}

void Historian::advanceWorld(Event::UpdateWorld *updateWorld) {
    unsigned long sent = updateWorld->getMilliseconds();
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    long offset = long(now - sent) + pingTime->getClockOffset();
    
    /*if(offset < 0) {
        pingTime->addClockOffset(-offset);
    }*/
    
    if(offset > 0) {
        physicsWorld->stepWorld(offset);
    }
}

}  // namespace History
}  // namespace Project
