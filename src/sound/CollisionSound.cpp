#include "CollisionSound.h"

#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

namespace Project {
namespace Sound {
CollisionSound::CollisionSound(){
}
CollisionSound::~CollisionSound() {
    cleanUp();
}
void CollisionSound::initialize(){
    ALHelpers::setupBuffer(collisionBuffers, 16);
    ALHelpers::setupSource(collisionSources, 16);
    
    collisionCounter=0;
    lastLocation=Math::Point(100.0,100.0,100.0);
    
    //Position of the source sound.
    ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    ALfloat sourceVelocity[] = {0.0, 0.0, 0.0};
    
    //Load the engine sound into the buffers
    string file[] = {"data/sound/soundfx/collision01.wav",
        "data/sound/soundfx/collision02.wav",
        "data/sound/soundfx/collision03.wav"
    };
    for(int i=0;i<16;i++){
        ALHelpers::loadFileToBuffer(collisionBuffers[i],file[i % 3]);
        
        // bind a sound source to the buffer data
        ALHelpers::bindBufferToSource(collisionBuffers[i], collisionSources[i]);

        alSourcei(collisionSources[i], AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(collisionSources[i], AL_PITCH, 1.0);
        alSourcef(collisionSources[i], AL_GAIN, GET_SETTING("sound.collision.volume", 0.0));
        alSourcef(collisionSources[i], AL_ROLLOFF_FACTOR, GET_SETTING("sound.collision.attenuation", 0.0));
        alSourcei(collisionSources[i], AL_LOOPING, AL_FALSE);
        alSourcefv(collisionSources[i], AL_POSITION, sourcePosition);
        alSourcefv(collisionSources[i], AL_VELOCITY, sourceVelocity);
    }
}
void CollisionSound::cleanUp(){
    ALHelpers::destroyBuffers(collisionBuffers,16);
    ALHelpers::destroySources(collisionSources,16);
}
void CollisionSound::playCollision(Math::Point location){
    if((location-lastLocation).length()>2.0){
        ALfloat sourcePosition[] = {
            location.getX(), 
            location.getY(), 
            location.getZ()};
        
        alSourcefv(collisionSources[collisionCounter], AL_POSITION, sourcePosition);
        
        ALHelpers::playFromSource(collisionSources[collisionCounter]);
        
        lastLocation = location;
        collisionCounter++;
        if(collisionCounter>15){
            collisionCounter=0;
        }
    }
}
}  // namespace Sound
}  // namespace Project
#endif