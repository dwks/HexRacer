#include <cstdlib>
#include "SoundSystem.h"

#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

namespace Project {
namespace Sound {

SoundSystem::SoundSystem() : Timing::TimedSubsystem(10) {
    
}

SoundSystem::~SoundSystem() {
    cleanUp();
}

bool SoundSystem::initialize() {
    /*if(alutInit(NULL, 0)) {
        LOG2(AUDIO, INIT, "Can't initialize OpenAL");
        return false;
    }*/
    
    alutInit(NULL, 0);
    alGetError();  // clear error bit . . . why?
    
    LOG2(AUDIO, INIT, "Successfully initialized OpenAL");
    
    setupListener();
    setupMusic();
    
    LOG2(AUDIO, INIT, "Set up background music");
    
    return true;
}

void SoundSystem::cleanUp() {
    destroyMusic();
    alutExit();
}

void SoundSystem::setupListener() {
    // position of the listener
    static const ALfloat position[] = {0.0, 0.0, 0.0};
    
    // velocity of the listener
    static const ALfloat velocity[] = {0.0, 0.0, 0.0};
    
    // orientation of the Listener
    static const ALfloat orientation[] = {
        0.0, 0.0, -1.0,  // "at" position
        0.0, 1.0, 0.0    // "up" direction
    };
    
    alListenerfv(AL_POSITION, position);
    alListenerfv(AL_VELOCITY, velocity);
    alListenerfv(AL_ORIENTATION, orientation);
}

void SoundSystem::setupMusic() {
    // load data from .wav file
    alGenBuffers(1, &musicBuffer);
    
    ALenum format;
    ALsizei size;
    ALvoid *data;
    ALsizei freq;
    ALboolean loop;
    
    // !!! this is deprecated
    alutLoadWAVFile((ALbyte *)"data/sound/music/mainloop.wav",
        &format, &data, &size, &freq, &loop);
    alBufferData(musicBuffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
    
    // bind a sound source to the buffer data
    alGenSources(1, &musicSource);
    
    // Position of the source sound.
    static const ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    
    // Velocity of the source sound.
    static const ALfloat sourceVelocity[] = { 0.0, 0.0, 0.0 };
    
    alSourcei(musicSource, AL_BUFFER, musicBuffer);
    alSourcef(musicSource, AL_PITCH, 1.0);
    alSourcef(musicSource, AL_GAIN, 1.0);
    alSourcefv(musicSource, AL_POSITION, sourcePosition);
    alSourcefv(musicSource, AL_VELOCITY, sourceVelocity);
    alSourcei(musicSource, AL_LOOPING, AL_TRUE);  // don't use loop from file
    
    // Do another error check and return.
    
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error initializing background music");
    }
    
    alSourcePlay(musicSource);
}

void SoundSystem::destroyMusic() {
    alDeleteBuffers(1, &musicBuffer);
    alDeleteSources(1, &musicSource);
}

void SoundSystem::doAction(unsigned long currentTime) {
    // nothing yet
}

}  // namespace Sound
}  // namespace Project

#endif
