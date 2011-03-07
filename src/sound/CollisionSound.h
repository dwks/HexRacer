#ifndef PROJECT_SOUND__COLLISION_SOUND_H
#define PROJECT_SOUND__COLLISION_SOUND_H

#include "config.h"
#ifdef HAVE_OPENAL

#include "sound/ALHelpers.h"

#include "AL/al.h"
#include "object/WorldManager.h"
#include "world/PlayerManager.h"
#include <boost/concept_check.hpp>


namespace Project {
namespace Sound {

class CollisionSound {
public:
    CollisionSound();
    ~CollisionSound();
    
    void initialize();
    void cleanUp();
    void playCollision(Math::Point location, float impulse);
private:
    ALuint collisionBuffers[16];
    ALuint collisionSources[16];
    
    int collisionCounter;
    Math::Point lastLocation;
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
