#include <cstdlib>
#include "SoundSystem.h"

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

namespace Project {
namespace Sound {

SoundSystem::SoundSystem() {
    if(!alutInit(NULL, 0)) {
        LOG2(AUDIO, INIT, "Can't initialize OpenAL");
        std::exit(1);  // !!!
    }
    else {
        // ...
        
        LOG2(AUDIO, INIT, "Successfully initialized OpenAL");
    }
}

SoundSystem::~SoundSystem() {
    alutExit();
}

void SoundSystem::playMusic() {
    ALuint buffer;
    alGenBuffers(1, &buffer);
    
    alDeleteBuffers(1, &buffer);
    //alutLoadMemoryFromFile();
}

}  // namespace Sound
}  // namespace Project
