#include <cstdlib>
#include "SoundSystem.h"

#ifdef HAVE_OPENAL

#include "AL/al.h"
#include "AL/alut.h"

#include "log/Logger.h"

#include "event/EventSystem.h"

namespace Project {
namespace Sound {

SoundSystem::SoundSystem(){
}

SoundSystem::~SoundSystem() {
    cleanUp();
}

void SoundSystem::physicsCollisionHandler(Event::PhysicsCollision *event){
    
}

bool SoundSystem::initialize(Object::WorldManager *worldManager, World::PlayerManager *playerManager) {
    ALHelpers::initOpenAL();
    
    playerCount = worldManager->getPlayerList()->getPlayerCount();
    this->worldManager = worldManager;
    this->playerManager = playerManager;
    
    setupListener();
    setupEngines();
    //setupCollisions();
    setupGameMusic();
    
    METHOD_OBSERVER(&SoundSystem::physicsCollisionHandler);
    return true;
}

void SoundSystem::cleanUp() {
    ALHelpers::destroyBuffer(musicBuffer);
    ALHelpers::destroySource(musicSource);
    delete engineSound;
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
void SoundSystem::setupCollisions() {
    
}
void SoundSystem::setupEngines(){
    engineSound = new EngineSound(worldManager);
    engineSound->initialize();
}
void SoundSystem::setupGameMusic() {
    ALHelpers::setupBuffer(&musicBuffer,1);
    
    //Load the music into the buffer
    string file = "data/sound/music/GameMusicIntro.wav";
    ALHelpers::loadFileToBuffer(musicBuffer,file);
    
    //setup the music source
    ALHelpers::setupSource(&musicSource,1);
    
    // bind a sound source to the buffer data
    ALHelpers::bindBufferToSource(musicBuffer, musicSource);
    
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
    
    ALHelpers::playFromSource(musicSource);
}

void SoundSystem::checkPlayerCount(){
    int count = worldManager->getPlayerList()->getPlayerCount();
    if(count != playerCount){
        playerCountChanged(count);
    }
}

void SoundSystem::playerCountChanged(int count){
    playerCount = count;
    engineSound->cleanUp();
    engineSound->changePlayerCount(worldManager->getPlayerList()->getPlayerCount());
    engineSound->initialize();
}

//This is the least hacky way I could figure out how to do this
//and it is still pretty hacky
void SoundSystem::checkMusicIntroComplete(){
    ALint buffersProcessed = 0;
    alGetSourcei(musicSource,AL_BUFFERS_PROCESSED,&buffersProcessed);
    if((int)buffersProcessed == 1){
        string file = "data/sound/music/GameMusicLoop.wav";
        alSourceUnqueueBuffers(musicSource,1,&musicBuffer);
        ALHelpers::loadFileToBuffer(musicBuffer,file);
        ALHelpers::bindBufferToSource(musicBuffer, musicSource);
        alSourcei(musicSource, AL_LOOPING, AL_TRUE);
        ALHelpers::playFromSource(musicSource);
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

void SoundSystem::doAction() {
    checkMusicIntroComplete();
    checkPlayerCount();
    updateListener();
    engineSound->updateEngines();
    
}

}  // namespace Sound
}  // namespace Project

#endif
