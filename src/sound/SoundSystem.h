#ifndef PROJECT_SOUND__SOUND_SYSTEM_H
#define PROJECT_SOUND__SOUND_SYSTEM_H

#include "config.h"
#ifdef HAVE_OPENAL

#include "event/PhysicsCollision.h"
#include "event/PlayerProgressEvent.h"
#include "event/Enabler.h"
#include "sound/EngineSound.h"
#include "sound/CollisionSound.h"
#include "sound/ALHelpers.h"

#ifndef WIN32
#include "AL/al.h"
#else
#include "al.h"
#endif
#include "object/WorldManager.h"
#include "world/PlayerManager.h"

namespace Project {
namespace Sound {

class SoundSystem : public Event::Enabler {
protected:
    void physicsCollisionHandler(Event::PhysicsCollision *event);
    void playerProgressHandler(Event::PlayerProgressEvent *event);
    
    //Sources
    ALuint musicSource;
private:
    //Buffers
    ALuint musicBuffer;
    ALuint fxBuffers[8];
    ALuint fxSources[8];
public:
    SoundSystem();
    ~SoundSystem();
    
    virtual bool initialize(Object::WorldManager *worldManager, World::PlayerManager *playerManager);
    virtual bool initialize();
    
    void playMusic();
    void stopMusic();
    void doAction();
    
    void cleanUp();
private:
    void setupGameMusic();
    void setupEngines();
    void setupCollisions();
    void setupGameSoundFX();
    
    void checkPlayerCount();
    void playerCountChanged(int count);
    void checkMusicIntroComplete();
    
    Sound::EngineSound *engineSound;
    Sound::CollisionSound *collisionSound;
    Object::WorldManager *worldManager;
    World::PlayerManager *playerManager;
    int playerCount;
    int systemType;
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
