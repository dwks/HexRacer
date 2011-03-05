#ifndef PROJECT_SOUND__SOUND_SYSTEM_H
#define PROJECT_SOUND__SOUND_SYSTEM_H

#include "config.h"
#ifdef HAVE_OPENAL

#include "event/PhysicsCollision.h"
#include "event/Enabler.h"
#include "sound/EngineSound.h"
#include "sound/ALHelpers.h"

#include "AL/al.h"
#include "object/WorldManager.h"
#include "world/PlayerManager.h"
#include <boost/concept_check.hpp>

namespace Project {
namespace Sound {

class SoundSystem : public Event::Enabler {
protected:
    void physicsCollisionHandler(Event::PhysicsCollision *event);
private:
    //Buffers
    ALuint musicBuffer;
    ALuint engineBuffers[16];
    ALuint collisionBuffers[16];
    
    //Sources
    ALuint musicSource;
    ALuint engineSources[16];
    ALuint collisionSources[16];
public:
    SoundSystem();
    ~SoundSystem();
    
    virtual bool initialize(Object::WorldManager *worldManager, World::PlayerManager *playerManager);
    
    //virtual void doAction(unsigned long currentTime);
    void doAction();
private:
    void setupListener();
    void setupGameMusic();
    void setupEngines();
    void setupCollisions();
    
    void updateListener();
    
    void checkPlayerCount();
    void playerCountChanged(int count);
    void checkMusicIntroComplete();
    
    void cleanUp();
    
    Sound::EngineSound *engineSound;
    Object::WorldManager *worldManager;
    World::PlayerManager *playerManager;
    int playerCount;
};

}  // namespace Sound
}  // namespace Project

#else

namespace Project {
namespace Sound {
}
}
#endif 

#endif
