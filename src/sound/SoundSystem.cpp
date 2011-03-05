#include <cstdlib>
#include "SoundSystem.h"

#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

namespace Project {
namespace Sound {

SoundSystem::SoundSystem(){
    
}

SoundSystem::~SoundSystem() {
    cleanUp();
}

bool SoundSystem::initialize(Object::WorldManager *worldManager, World::PlayerManager *playerManager) {
    alutInit(NULL, 0);
    alGetError();
    
    LOG2(AUDIO, INIT, "Successfully initialized OpenAL");
    
    this->worldManager = worldManager;
    this->playerManager = playerManager;
    
    setupListener();
    setupEngines();
    setupGameMusic();
    
    return true;
}

void SoundSystem::cleanUp() {
    destroyBuffers();
    destroySources();
    alutExit();
}

void SoundSystem::setupListener() {
    ALfloat position[] = {0.0, 0.0, 0.0};
    ALfloat velocity[] = {0.0, 0.0, 0.0};
    ALfloat orientation[] = {
        0.0, 0.0, -1.0,  // "at" position
        0.0, 1.0, 0.0    // "up" direction
    };
    
    alListenerfv(AL_POSITION, position);
    alListenerfv(AL_VELOCITY, velocity);
    alListenerfv(AL_ORIENTATION, orientation);
}

void SoundSystem::checkPlayerCount(){
    if(worldManager->getPlayerList()->getPlayerCount() != playerCount){
            playerCountChanged();
    }
}

void SoundSystem::playerCountChanged(){
    destroyBuffers();
    destroySources();
    setupEngines();
}
//This is the least hacky way I could figure out how to do this
//and it is still pretty hacky
void SoundSystem::checkMusicIntroComplete(){
    ALint buffersProcessed = 0;
    alGetSourcei(musicSource,AL_BUFFERS_PROCESSED,&buffersProcessed);
    if((int)buffersProcessed == 1){
        string file = "data/sound/music/GameMusicLoop.wav";
        alSourceUnqueueBuffers(musicSource,1,&musicBuffer);
        loadFileToBuffer(musicBuffer,file);
        bindBufferToSource(musicBuffer, musicSource);
        alSourcei(musicSource, AL_LOOPING, AL_TRUE);
        playFromSource(musicSource);
    }
}

void SoundSystem::setupGameMusic() {
    setupBuffer(&musicBuffer,1);
    
    //Load the music into the buffer
    string file = "data/sound/music/GameMusicIntro.wav";
    loadFileToBuffer(musicBuffer,file);
    
    //setup the music source
    setupSource(&musicSource,1);
    
    // bind a sound source to the buffer data
    bindBufferToSource(musicBuffer, musicSource);
    
    //Position of the source sound.
    //Music is relative positioning
    ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    ALfloat sourceVelocity[] = {0.0, 0.0, 0.0};
    
    alSourcei(musicSource, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef(musicSource, AL_PITCH, 1.0);
    alSourcef(musicSource, AL_GAIN, 0.25);
    alSourcei(musicSource, AL_LOOPING, AL_FALSE);
    alSourcefv(musicSource, AL_POSITION, sourcePosition);
    alSourcefv(musicSource, AL_VELOCITY, sourceVelocity);
    
    playFromSource(musicSource);
}

void SoundSystem::setupEngines(){
    playerCount = worldManager->getPlayerList()->getPlayerCount();
    setupBuffer(engineBuffers, playerCount);
    setupSource(engineSources, playerCount);
    
    //Position of the source sound.
    ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    ALfloat sourceVelocity[] = {0.0, 0.0, 0.0};
    
    //Load the engine sound into the buffers
    string file = "data/sound/soundfx/engine.wav";
    for(int i=0;i<playerCount;i++){
        loadFileToBuffer(engineBuffers[i],file);
        
        // bind a sound source to the buffer data
        bindBufferToSource(engineBuffers[i], engineSources[i]);

        alSourcei(engineSources[i], AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(engineSources[i], AL_PITCH, 1.0);
        alSourcef(engineSources[i], AL_GAIN, 0.5);
        alSourcef(engineSources[i], AL_ROLLOFF_FACTOR, 0.25);
        alSourcei(engineSources[i], AL_LOOPING, AL_TRUE);
        alSourcefv(engineSources[i], AL_POSITION, sourcePosition);
        alSourcefv(engineSources[i], AL_VELOCITY, sourceVelocity);
        
        playFromSource(engineSources[i]);
    }
}

void SoundSystem::updateListener(){
    Object::Player *player = playerManager->getPlayer();
    
    Math::Point positionPoint = player->getPosition(); 
    Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity();
    Math::Point orientationPoint = player->getPhysicalObject()->getFrontDirection();
    
    // position of the listener
    ALfloat position[] = {
        positionPoint.getX(), 
        positionPoint.getY(), 
        positionPoint.getZ()};
    
    // velocity of the listener
    ALfloat velocity[] = {
        velocityPoint.getX(), 
        velocityPoint.getY(), 
        velocityPoint.getZ()};
    
    // orientation of the Listener
    ALfloat orientation[] = {
        orientationPoint.getX(),
        orientationPoint.getY(),
        orientationPoint.getZ(),  // "at" position
        0.0, 1.0, 0.0    // "up" direction
    };
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    alListenerfv(AL_POSITION, position);
    alListenerfv(AL_VELOCITY, velocity);
    alListenerfv(AL_ORIENTATION, orientation);
}

void SoundSystem::updateEngines(){
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    
    int count=0;
        
    while(it.hasNext()){
        Object::Player *player = it.next();
        
        changeEnginePitch(player,engineSources[count]);
        updateEngineDetails(player,engineSources[count]);
        count++;
    }
}
void SoundSystem::updateEngineDetails(Object::Player *player, ALuint source){
    Math::Point positionPoint = player->getPosition(); 
    Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity();
    Math::Point orientationPoint = player->getPhysicalObject()->getFrontDirection();
        
    ALfloat sourcePosition[] = {
        positionPoint.getX(), 
        positionPoint.getY(), 
        positionPoint.getZ()};
    
    ALfloat sourceVelocity[] = {
        velocityPoint.getX(), 
        velocityPoint.getY(), 
        velocityPoint.getZ()};
    
    ALfloat sourceOrientation[] = {
        orientationPoint.getX(),
        orientationPoint.getY(),
        orientationPoint.getZ(),  // "at" position
        0.0, 1.0, 0.0    // "up" direction
    };
    
    alSourcefv(source, AL_POSITION, sourcePosition);
    alSourcefv(source, AL_VELOCITY, sourceVelocity);
    alSourcefv(source, AL_ORIENTATION, sourceOrientation);
}

void SoundSystem::changeEnginePitch(Object::Player *player, ALuint source){
    float max_pitch = 3.0;
    float min_pitch = 0.5;
    float final_pitch = 0.5;
    bool accelerating = player->getIntention().getAccel();
    bool onGround = player->getOnGround();
    
    Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity();
    
    float speed = (float)velocityPoint.length();
    
    if(accelerating&&onGround){
        final_pitch = (float)(min_pitch + (speed/30.0)*(max_pitch-min_pitch));
    }
    if(!accelerating&&onGround){
        final_pitch = (float)(min_pitch + (speed/30.0)*(max_pitch-min_pitch));
    }
    if(accelerating&&!onGround){
        final_pitch = (float)(min_pitch + (speed/30.0)*(max_pitch-min_pitch));
    }
    if(!accelerating&&!onGround){
        final_pitch = min_pitch;
    }
    
    alSourcef(source, AL_PITCH, final_pitch);
}

void SoundSystem::loadFileToBuffer(ALuint buffer, string file){
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
        LOG2(AUDIO, INIT, "Error loading file: " << (string)file << " to buffer");
    }
}

void SoundSystem::setupBuffer(ALuint *buffer, int count){
    alGetError();
    alGenBuffers(count, buffer);   
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error setting up " << count << " buffer(s)");
    }
}

void SoundSystem::setupSource(ALuint *source, int count){
    alGetError();
    alGenSources(count, source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error setting up source(s)");
    }
}

void SoundSystem::enqueueBufferToSource(ALuint *buffer, ALuint source, int count){
    alGetError();
    alSourceQueueBuffers(source, count, buffer);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error enqueuing buffer to source");
    }
}

void SoundSystem::bindBufferToSource(ALuint buffer, ALuint source){
    alGetError();
    alSourcei(source, AL_BUFFER, buffer);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error binding buffer to source");
    }
}

void SoundSystem::playFromSource(ALuint source){
    alGetError();
    alSourcePlay(source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error playing from source");
    }
}

void SoundSystem::pauseFromSource(ALuint source){
    alGetError();
    alSourcePause(source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error pausing from source");
    }
}

void SoundSystem::stopFromSource(ALuint source){
    alGetError();
    alSourceStop(source);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error stopping from source");
    }
}

void SoundSystem::destroyBuffers() {
    alGetError();
    alDeleteBuffers(1, &musicBuffer);
    alDeleteBuffers(playerCount, engineBuffers);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error deleting Buffers");
    }
}

void SoundSystem::destroySources() {
    alGetError();
    alDeleteSources(1, &musicSource);
    alDeleteSources(playerCount, engineSources);
    if(alGetError() != AL_NO_ERROR) {
        LOG2(AUDIO, INIT, "Error deleting Sources");
    }
}

void SoundSystem::doAction() {
    checkMusicIntroComplete();
    checkPlayerCount();
    updateListener();
    updateEngines();
    
}

}  // namespace Sound
}  // namespace Project

#endif
