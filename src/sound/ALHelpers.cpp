#include <cstdlib>
#include "ALHelpers.h"

#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

namespace Project {
namespace Sound {
void ALHelpers::initOpenAL(){   
    alGetError();
    alutInit(NULL, 0);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Could not initialize openAL");
    } else {
        LOG2(AUDIO, INIT, "Successfully initialized OpenAL");
    }
}
void ALHelpers::loadFileToBuffer(ALuint buffer, std::string file){
    ALenum format;
    ALsizei size;
    ALvoid *data;
    ALsizei freq;
    ALboolean loop;
        
    // !!! this is deprecated    
    alutLoadWAVFile((ALbyte *)file.c_str(), &format, &data, &size, &freq, &loop);
    alBufferData(buffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
    
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error loading file: " << (std::string)file << " to buffer");
    }
}

void ALHelpers::setupBuffer(ALuint *buffer, int count){
    alGetError();
    alGenBuffers(count, buffer);   
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error setting up " << count << " buffer(s)");
    }
}

void ALHelpers::setupSource(ALuint *source, int count){
    alGetError();
    alGenSources(count, source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error setting up source(s)");
    }
}

void ALHelpers::enqueueBufferToSource(ALuint *buffer, ALuint source, int count){
    alGetError();
    alSourceQueueBuffers(source, count, buffer);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error enqueuing buffer to source");
    }
}

void ALHelpers::bindBufferToSource(ALuint buffer, ALuint source){
    alGetError();
    alSourcei(source, AL_BUFFER, buffer);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error binding buffer to source");
    }
}

void ALHelpers::playFromSource(ALuint source){
    alGetError();
    alSourcePlay(source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error playing from source");
    }
}

void ALHelpers::pauseFromSource(ALuint source){
    alGetError();
    alSourcePause(source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error pausing from source");
    }
}

void ALHelpers::stopFromSource(ALuint source){
    alGetError();
    alSourceStop(source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error stopping from source");
    }
}

void ALHelpers::destroyBuffers(ALuint *buffers, int count) {
    alGetError();
    alDeleteBuffers(count, buffers);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error deleting Buffers");
    }
}

void ALHelpers::destroySources(ALuint *sources, int count) {
    alGetError();
    alDeleteSources(count, sources);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error deleting Sources");
    }
}
void ALHelpers::destroyBuffer(ALuint buffer) {
    alGetError();
    alDeleteBuffers(1, &buffer);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error deleting Buffer");
    }
}

void ALHelpers::destroySource(ALuint source) {
    alGetError();
    alDeleteSources(1, &source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error deleting Source");
    }
}
}  // namespace Sound
}  // namespace Project
#endif