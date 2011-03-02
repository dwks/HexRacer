#include "SDL.h"  // for SDL_GetTicks()

#include "GameWorld.h"

#include "settings/SettingsManager.h"
#include "settings/ProgramSettings.h"

#include "event/EventSystem.h"

#include "timing/AccelControl.h"

#include "history/PingTimeMeasurer.h"
#include "misc/Sleeper.h"

namespace Project {
namespace SDL {

bool GameWorld::doConnect(const std::string &host, unsigned short port) {
    network = boost::shared_ptr<NetworkPortal>(new NetworkPortal());
    
    // must happen before we try to connect, so that PingTimeMeasurer exists
    historian = boost::shared_ptr<History::Historian>(
        new History::Historian());
    
    if(!network->connectTo(host.c_str(), port)) {
        LOG2(NETWORK, ERROR, "Could not connect to server "
            << host << ":" << port << "!");
        return false;
    }
    
    Object::World *world;
    Object::PlayerList *playerList;
    network->waitForWorld(world, playerList);
    
    basicWorld->constructDuringConnect(world, playerList, network->getID());
    clientData = boost::shared_ptr<ClientData>(new ClientData(network->getID()));
    Settings::ProgramSettings::getInstance()->setConnected(true);
    isConnectedToNetwork = true;
    
    historian->setWorldManager(basicWorld->getWorldManager());
    historian->setPhysicsWorld(basicWorld->getPhysicsWorld());
    
    return true;
}

void GameWorld::doNotConnect() {
    basicWorld->constructSkippingConnect();
    
    // do not initialize network, historian
    
    clientData = boost::shared_ptr<ClientData>(new ClientData());
    isConnectedToNetwork = false;
}

bool GameWorld::tryConnect(const std::string &host, unsigned short port) {
    basicWorld = boost::shared_ptr<World::BasicWorld>(new World::BasicWorld());
    basicWorld->constructBeforeConnect();
    
    if(host == "" && port == 0) {
        doNotConnect();
        return true;
    }
    else {
        return doConnect(host, port);
    }
}

void GameWorld::constructAfterConnect(Map::HRMap *map) {
    basicWorld->constructAfterConnect(map);
    
    // hack: add path trackers to existing players in the deserialized world
    Object::PlayerList::IteratorType it
        = getWorldManager()->getPlayerList()->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        player->setPathTracker(
            new Map::PathTracker(*getWorldManager()->getPathManager()));
    }
    
    if(!isConnectedToNetwork) {
        Map::RaceManager *raceManager = basicWorld->getRaceManager();
        
        getWorldManager()->initForClient(clientData->getPlayerID(),
            raceManager->startingPointForPlayer(clientData->getPlayerID()),
            raceManager->startingPlayerDirection());
    }
    
    soundSystem = boost::shared_ptr<Sound::SoundSystem>(new Sound::SoundSystem());
    soundSystem->initialize();
}

void GameWorld::checkNetwork() {
    if(isConnectedToNetwork) {
        network->checkNetwork();
    }
}

void GameWorld::doPhysics() {
    basicWorld->doPhysics();
}

void GameWorld::doAI() {
    basicWorld->doAI();
}

void GameWorld::doSound() {
    soundSystem->doAction();
}


void GameWorld::render() {
    basicWorld->getPhysicsWorld()->render();
}

}  // namespace SDL
}  // namespace Project
