#ifndef PROJECT_SOUND__SOUND_SYSTEM_H
#define PROJECT_SOUND__SOUND_SYSTEM_H

#include "config.h"
#ifdef HAVE_OPENAL

#include "AL/al.h"

namespace Project {
namespace Sound {

class SoundSystem {
private:
    ALuint musicBuffer;
    ALuint musicSource;
public:
    SoundSystem();
    ~SoundSystem();
    
    virtual bool initialize();
    
    //virtual void doAction(unsigned long currentTime);
    void doAction();
private:
    void setupListener();
    
    void setupMusic();
    void destroyMusic();
    
    void cleanUp();
};

}  // namespace Sound
}  // namespace Project

#endif
#endif
