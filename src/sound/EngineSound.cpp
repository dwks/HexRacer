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
    max_buffers = 16;
}
EngineSound::~EngineSound() {
    cleanUp();
}
void EngineSound::initialize(){
    playerCount = this->worldManager->getPlayerList()->getPlayerCount();
    if(playerCount>max_buffers){
        playerCount=max_buffers;
    }
    ALHelpers::setupBuffer(engineBuffers, playerCount);
    ALHelpers::setupSource(engineSources, playerCount);
    ALHelpers::setupBuffer(skidBuffers, playerCount);
    ALHelpers::setupSource(skidSources, playerCount);
    //Position of the source sound.
    ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    ALfloat sourceVelocity[] = {0.0, 0.0, 0.0};
    
    //Load the engine sound into the buffers
    string file = "data/sound/soundfx/engine.wav";
    string skidfile ="data/sound/soundfx/skid01.wav";
    for(int i=0;i<playerCount;i++){
        engineTones[i] = 1.0;
        ALHelpers::loadFileToBuffer(engineBuffers[i],file);
        ALHelpers::loadFileToBuffer(skidBuffers[i],skidfile);
        // bind a sound source to the buffer data
        ALHelpers::bindBufferToSource(engineBuffers[i], engineSources[i]);
        ALHelpers::bindBufferToSource(skidBuffers[i], skidSources[i]);

        alSourcei(skidSources[i], AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(skidSources[i], AL_PITCH, 1.0);
        alSourcef(skidSources[i], AL_GAIN, GET_SETTING("sound.skid.volume", 0.0));
        alSourcef(skidSources[i], AL_ROLLOFF_FACTOR, GET_SETTING("sound.skid.attenuation", 0.0));
        alSourcei(skidSources[i], AL_LOOPING, AL_TRUE);
        alSourcefv(skidSources[i], AL_POSITION, sourcePosition);
        alSourcefv(skidSources[i], AL_VELOCITY, sourceVelocity);
        
        alSourcei(engineSources[i], AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(engineSources[i], AL_PITCH, 1.0);
        alSourcef(engineSources[i], AL_GAIN, GET_SETTING("sound.engine.volume", 0.0));
        alSourcef(engineSources[i], AL_ROLLOFF_FACTOR, GET_SETTING("sound.engine.attenuation", 0.0));
        alSourcei(engineSources[i], AL_LOOPING, AL_TRUE);
        alSourcefv(engineSources[i], AL_POSITION, sourcePosition);
        alSourcefv(engineSources[i], AL_VELOCITY, sourceVelocity);
        
        ALHelpers::playFromSource(engineSources[i]);
    }
}
void EngineSound::cleanUp(){
    ALHelpers::destroyBuffers(engineBuffers,playerCount);
    ALHelpers::destroySources(engineSources,playerCount);
    ALHelpers::destroyBuffers(skidBuffers,playerCount);
    ALHelpers::destroySources(skidSources,playerCount);
}
void EngineSound::updateEngines(){
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    
    int count=0;
        
    while(it.hasNext()&&count<max_buffers){
        Object::Player *player = it.next();
        
        updateSkidForPlayer(player,skidSources[count]);
        changeEnginePitch(player,engineSources[count],count);
        updateEngineDetails(player,engineSources[count]);
        count++;
    }
}
void EngineSound::changePlayerCount(int count){
    playerCount = count;
    if(playerCount>max_buffers){
        playerCount=max_buffers;
    }
    for(int i=0;i<playerCount;i++){
        engineTones[i]=1.0;
    }
}
void EngineSound::updateSkidForPlayer(Object::Player *player, ALuint source){
    float max_pitch = 3.0;
    float min_pitch = 0.5;
    float final_pitch = 0.5;
    if(player->getPhysicalObject()->getSliding() && player->getPhysicalObject()->getOnGround()){
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
        
        double extent = ((float)velocityPoint.dotProduct(player->getPhysicalObject()->getRightDirection()));
        
        extent = abs(extent);
        
        final_pitch = (float)(min_pitch + (extent/15.0)*(max_pitch-min_pitch));
        
        alSourcef(source, AL_PITCH, final_pitch);
        alSourcefv(source, AL_POSITION, sourcePosition);
        alSourcefv(source, AL_VELOCITY, sourceVelocity);
        ALint state = 0;
        alGetSourcei(source,AL_SOURCE_STATE,&state);
        if(state == AL_PLAYING){
            //Do nothing
        } else {
            ALHelpers::playFromSource(source);
        }
        
    } else {
        ALHelpers::pauseFromSource(source);  
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