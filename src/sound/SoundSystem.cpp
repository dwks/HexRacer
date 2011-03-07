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
    collisionSound->playCollision(event->getLocation(),event->getImpulse());
}

bool SoundSystem::initialize(Object::WorldManager *worldManager, World::PlayerManager *playerManager) {
    ALHelpers::initOpenAL();
    
    playerCount = worldManager->getPlayerList()->getPlayerCount();
    this->worldManager = worldManager;
    this->playerManager = playerManager;
    
    ALHelpers::setupListener();
    setupEngines();
    setupCollisions();
    setupGameMusic();
    
    METHOD_OBSERVER(&SoundSystem::physicsCollisionHandler);
    return true;
}

void SoundSystem::cleanUp() {
    ALHelpers::destroyBuffer(musicBuffer);
    ALHelpers::destroySource(musicSource);
    delete engineSound;
    delete collisionSound;
    ALHelpers::exitOpenAL();
}

void SoundSystem::setupCollisions() {
    collisionSound = new CollisionSound();
    collisionSound->initialize();
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
    alSourcef(musicSource, AL_GAIN, GET_SETTING("sound.music.volume", 0.0));
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

void SoundSystem::doAction() {
    checkMusicIntroComplete();
    checkPlayerCount();
    
    Object::Player *player = playerManager->getPlayer();
    Math::Point positionPoint = player->getPosition(); 
    Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity();
    Math::Point orientationPoint = player->getPhysicalObject()->getFrontDirection();
    ALHelpers::updateListener(positionPoint,velocityPoint,orientationPoint);
    
    engineSound->updateEngines(playerManager->getPlayer()->getID());
}

}  // namespace Sound
}  // namespace Project

#endif
