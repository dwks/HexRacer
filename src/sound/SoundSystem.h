#ifndef PROJECT_SOUND__SOUND_SYSTEM_H
#define PROJECT_SOUND__SOUND_SYSTEM_H

namespace Project {
namespace Sound {

class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();
    
    virtual void playMusic();
};

}  // namespace Sound
}  // namespace Project

#endif
