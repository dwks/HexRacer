#ifndef PROJECT_SOUND__ALHELPERS_H
#define PROJECT_SOUND__ALHELPERS_H

#include "config.h"
#ifdef HAVE_OPENAL

#include "AL/al.h"
#include <boost/concept_check.hpp>

namespace Project {
namespace Sound {

class ALHelpers {
public:
    static void initOpenAL();
    static void destroyBuffers(ALuint *buffers, int count);
    static void destroySources(ALuint *sources, int count);
    static void destroyBuffer(ALuint buffer);
    static void destroySource(ALuint source);
    
    static void playFromSource(ALuint source);
    static void pauseFromSource(ALuint source);
    static void stopFromSource(ALuint source);
    
    static void loadFileToBuffer(ALuint buffer, std::string file);
    
    static void setupBuffer(ALuint *buffer, int count);
    static void setupSource(ALuint *source, int count);
    static void bindBufferToSource(ALuint buffer, ALuint source);
    static void enqueueBufferToSource(ALuint *buffer, ALuint source, int count);
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