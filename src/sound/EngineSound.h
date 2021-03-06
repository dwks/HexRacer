#ifndef PROJECT_SOUND__ENGINE_SOUND_H
#define PROJECT_SOUND__ENGINE_SOUND_H

#include "config.h"
#ifdef HAVE_OPENAL

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

class EngineSound {
public:
    EngineSound();
    EngineSound(Object::WorldManager *worldManager);
    ~EngineSound();
    
    void initialize();
    void cleanUp();
    void updateEngines(int id);
    void changeEnginePitch(Object::Player *player, ALuint source, int toneIndex, int id);
    void updateEngineDetails(Object::Player *player, ALuint source);
    void updateSkidForPlayer(Object::Player *player, ALuint source);
    void changePlayerCount(int count);
private:
    ALuint skidBuffers[16];
    ALuint skidSources[16];
    ALuint engineBuffers[16];
    ALuint engineSources[16];
    float engineTones[16];
    int playerCount;
    int max_buffers;
    
    Object::WorldManager *worldManager;
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