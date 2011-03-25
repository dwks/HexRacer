#ifndef PROJECT_SOUND__ALHELPERS_H
#define PROJECT_SOUND__ALHELPERS_H

#include "config.h"
#include "math/Point.h"
#ifdef HAVE_OPENAL

#ifndef WIN32
#include "AL/al.h"
#else
#include "al.h"
#endif
#include <boost/concept_check.hpp>
#include <string>

namespace Project {
namespace Sound {

class ALHelpers {
public:
    static void initOpenAL();
    static void destroyBuffers(ALuint *buffers, int count);
    static void destroySources(ALuint *sources, int count);
    static void destroyBuffer(ALuint buffer);
    static void destroySource(ALuint source);
    static void exitOpenAL();
    
    static void playFromSource(ALuint source);
    static void pauseFromSource(ALuint source);
    static void stopFromSource(ALuint source);
    static void updateListener(Math::Point positionPoint, Math::Point velocityPoint, Math::Point orientationPoint);
    
    static void loadFileToBuffer(ALuint buffer, std::string file);
    
    static void setupBuffer(ALuint *buffer, int count);
    static void setupSource(ALuint *source, int count);
    static void setupListener();
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