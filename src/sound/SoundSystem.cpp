#include <cstdlib>
#include "SoundSystem.h"

#ifdef HAVE_OPENAL

#ifndef WIN32
#include "AL/al.h"
#include "AL/alut.h"
#else
#include "al.h"
#include "alut.h"
#endif


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
void SoundSystem::playerProgressHandler(Event::PlayerProgressEvent *event){
    if(event->getLapCount() == event->getTotalLaps()){
        ALHelpers::playFromSource(fxSources[1]);
    } else {
        ALHelpers::playFromSource(fxSources[0]);
    }
}
void SoundSystem::gameStateChangedHandler(Event::GameStageChanged *event){
    switch(event->getStage()){
        case World::WorldSetup::COUNT_1:
        case World::WorldSetup::COUNT_2:
        case World::WorldSetup::COUNT_3:
            ALHelpers::playFromSource(fxSources[2]);
            break;
        case World::WorldSetup::RUNNING_GAME:
            ALHelpers::playFromSource(fxSources[3]);
            break;
        default:
            break;
    }
}
bool SoundSystem::initialize(){
    systemType=0;
    
    ALHelpers::initOpenAL();
    ALHelpers::setupListener();
    
    setupGameMusic();
    
    return true;
}

bool SoundSystem::initialize(Object::WorldManager *worldManager, World::PlayerManager *playerManager) {
    systemType = 1;
    ALHelpers::initOpenAL();
    playerCount = worldManager->getPlayerList()->getPlayerCount();
    this->worldManager = worldManager;
    this->playerManager = playerManager;
    
    ALHelpers::setupListener();
    setupEngines();
    setupCollisions();
    setupGameMusic();
    setupGameSoundFX();
    
    METHOD_OBSERVER(&SoundSystem::physicsCollisionHandler);
    METHOD_OBSERVER(&SoundSystem::playerProgressHandler);
    METHOD_OBSERVER(&SoundSystem::gameStateChangedHandler);
    
    return true;
}

void SoundSystem::cleanUp() {
    ALHelpers::destroyBuffer(musicBuffer);
    ALHelpers::destroySource(musicSource);
    if(systemType==1){
        delete engineSound;
        delete collisionSound;
    }
    ALHelpers::exitOpenAL();
}

void SoundSystem::setupGameSoundFX()
{
    ALHelpers::setupBuffer(fxBuffers, 8);
    ALHelpers::setupSource(fxSources, 8);
    
    //Position of the source sound.
    ALfloat sourcePosition[] = {0.0, 0.0, 0.0};
    ALfloat sourceVelocity[] = {0.0, 0.0, 0.0};
    
    //Load all the generic soundfx
    string file[] = {"data/sound/soundfx/lapFinish.wav",
        "data/sound/soundfx/raceFinish.wav",
        "data/sound/soundfx/beep.wav",
        "data/sound/soundfx/start.wav"
    };
    
    for(int i=0;i<4;i++){
        ALHelpers::loadFileToBuffer(fxBuffers[i],file[i]);
        
        //bind a sound source to the buffer data
        ALHelpers::bindBufferToSource(fxBuffers[i], fxSources[i]);

        alSourcei(fxSources[i], AL_SOURCE_RELATIVE, AL_TRUE);
        alSourcef(fxSources[i], AL_PITCH, 1.0);
        alSourcef(fxSources[i], AL_GAIN, 1.0);
        alSourcei(fxSources[i], AL_LOOPING, AL_FALSE);
        alSourcefv(fxSources[i], AL_POSITION, sourcePosition);
        alSourcefv(fxSources[i], AL_VELOCITY, sourceVelocity);
    }
    alSourcef(fxSources[2], AL_GAIN, 0.4);
    alSourcef(fxSources[3], AL_GAIN, 0.2);
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
    string file = "";
    //Load the music into the buffer
    if(systemType==0){
        file = "data/sound/music/MenuMusicLoop.wav";
    } else if(systemType==1){
        file = "data/sound/music/GameMusicIntro.wav";
    }
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

void SoundSystem::stopMusic(){
    alSourcef(musicSource, AL_GAIN, 0.0);
}

void SoundSystem::playMusic(){
    ALHelpers::setupListener();
    alSourcef(musicSource, AL_GAIN, GET_SETTING("sound.music.volume", 0.0));
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
    string file = "";
    if((int)buffersProcessed == 1){
        if(systemType==0){
            file = "data/sound/music/MenuMusicLoop.wav";
        } else if(systemType==1){
            file = "data/sound/music/GameMusicLoop.wav";
        }
        
        alSourceUnqueueBuffers(musicSource,1,&musicBuffer);
        ALHelpers::loadFileToBuffer(musicBuffer,file);
        ALHelpers::bindBufferToSource(musicBuffer, musicSource);
        alSourcei(musicSource, AL_LOOPING, AL_TRUE);
        ALHelpers::playFromSource(musicSource);
    }
}

void SoundSystem::doAction() {
    checkMusicIntroComplete();
    
    if(systemType==1){
        checkPlayerCount();
        
        Object::Player *player = playerManager->getPlayer(); 
        Math::Point positionPoint = player->getPosition(); 
        Math::Point velocityPoint = player->getPhysicalObject()->getLinearVelocity(); 
        Math::Point orientationPoint = player->getPhysicalObject()->getFrontDirection(); 
        ALHelpers::updateListener(positionPoint,velocityPoint,orientationPoint); 
        
        engineSound->updateEngines(playerManager->getPlayer()->getID());
    }
}

}  // namespace Sound
}  // namespace Project

#endif
