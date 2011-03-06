#include "EngineSound.h"

#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

namespace Project {
namespace Sound {
EngineSound::EngineSound(){
}
    
EngineSound::EngineSound(Object::WorldManager *worldManager){
    this->worldManager = worldManager;
}
EngineSound::~EngineSound() {
    cleanUp();
}
void EngineSound::initialize(){
    playerCount = this->worldManager->getPlayerList()->getPlayerCount();
    ALHelpers::setupBuffer(engineBuffers, playerCount);
    ALHelpers::setupSource(engineSources, playerCount);
    
    //Position of the source sound.
    ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    ALfloat sourceVelocity[] = {0.0, 0.0, 0.0};
    
    //Load the engine sound into the buffers
    string file = "data/sound/soundfx/engine.wav";
    for(int i=0;i<playerCount;i++){
        engineTones[i] = 1.0;
        ALHelpers::loadFileToBuffer(engineBuffers[i],file);
        
        // bind a sound source to the buffer data
        ALHelpers::bindBufferToSource(engineBuffers[i], engineSources[i]);

        alSourcei(engineSources[i], AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(engineSources[i], AL_PITCH, 1.0);
        alSourcef(engineSources[i], AL_GAIN, 0.5);
        alSourcef(engineSources[i], AL_ROLLOFF_FACTOR, 0.25);
        alSourcei(engineSources[i], AL_LOOPING, AL_TRUE);
        alSourcefv(engineSources[i], AL_POSITION, sourcePosition);
        alSourcefv(engineSources[i], AL_VELOCITY, sourceVelocity);
        
        ALHelpers::playFromSource(engineSources[i]);
    }
}
void EngineSound::cleanUp(){
    ALHelpers::destroyBuffers(engineBuffers,playerCount);
    ALHelpers::destroySources(engineSources,playerCount);
}
void EngineSound::updateEngines(){
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    
    int count=0;
        
    while(it.hasNext()){
        Object::Player *player = it.next();
        
        changeEnginePitch(player,engineSources[count],count);
        updateEngineDetails(player,engineSources[count]);
        count++;
    }
}
void EngineSound::changePlayerCount(int count){
    playerCount = count;
    for(int i=0;i<playerCount;i++){
        engineTones[i]=1.0;
    }
}
void EngineSound::updateEngineDetails(Object::Player *player, ALuint source){
    Math::Point positionPoint = player->getPosition(); 
    Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity();
        
    ALfloat sourcePosition[] = {
        positionPoint.getX(), 
        positionPoint.getY(), 
        positionPoint.getZ()};
    
    ALfloat sourceVelocity[] = {
        velocityPoint.getX(), 
        velocityPoint.getY(), 
        velocityPoint.getZ()};
    
    alSourcefv(source, AL_POSITION, sourcePosition);
    alSourcefv(source, AL_VELOCITY, sourceVelocity);
}

void EngineSound::changeEnginePitch(Object::Player *player, ALuint source, int toneIndex){
    float max_pitch = 3.0;
    float min_pitch = 0.5;
    bool accelerating = player->getIntention().getAccel();
    bool onGround = player->getOnGround();
    
    Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity();
    
    float speed = (float)velocityPoint.length();
    
    if(accelerating&&onGround){
        engineTones[toneIndex] = (float)(min_pitch + (speed/30.0)*(max_pitch-min_pitch));
    }
    if(!accelerating&&onGround){
        engineTones[toneIndex] = (float)(min_pitch + (speed/30.0)*(max_pitch-min_pitch));
    }
    if(accelerating&&!onGround){
        engineTones[toneIndex] += 0.01;
        if(engineTones[toneIndex]>max_pitch){
            engineTones[toneIndex]=max_pitch;
        }
    }
    if(!accelerating&&!onGround){
        engineTones[toneIndex] -= 0.01;
        if(engineTones[toneIndex]<min_pitch){
            engineTones[toneIndex]=min_pitch;
        }
    }
    
    alSourcef(source, AL_PITCH, engineTones[toneIndex]);
}
}  // namespace Sound
}  // namespace Project
#endif