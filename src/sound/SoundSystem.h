#ifndef PROJECT_SOUND__SOUND_SYSTEM_H
#define PROJECT_SOUND__SOUND_SYSTEM_H

#include "config.h"
#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "object/WorldManager.h"
#include "world/PlayerManager.h"
#include <boost/concept_check.hpp>

namespace Project {
namespace Sound {

class SoundSystem {
private:
    //Buffers
    ALuint musicBuffer;
    ALuint engineBuffers[16];
    
    //Sources
    ALuint musicSource;
    ALuint engineSources[16];
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
    
    void updateListener();
    void updateEngines();
    void changeEnginePitch(Object::Player *player, ALuint source);
    void updateEngineDetails(Object::Player *player, ALuint source);
    
    void checkPlayerCount();
    void playerCountChanged();
    void checkMusicIntroComplete();
    
    void destroyBuffers();
    void destroySources();
    
    void playFromSource(ALuint source);
    void pauseFromSource(ALuint source);
    void stopFromSource(ALuint source);
    
    void loadFileToBuffer(ALuint buffer, string file);
    
    void setupBuffer(ALuint *buffer, int count);
    void setupSource(ALuint *source, int count);
    void bindBufferToSource(ALuint buffer, ALuint source);
    void enqueueBufferToSource(ALuint *buffer, ALuint source, int count);
    
    void cleanUp();
    
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
