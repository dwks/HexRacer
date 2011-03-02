#ifndef PROJECT_SDL__GAME_WORLD_H
#define PROJECT_SDL__GAME_WORLD_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include <string>

#include "world/BasicWorld.h"

#include "NetworkPortal.h"
#include "history/Historian.h"

#include "map/HRMap.h"
#include "map/RaceManager.h"

#include "ClientData.h"
#include "sound/SoundSystem.h"

namespace Project {
namespace SDL {

class GameWorld {
private:
    boost::shared_ptr<World::BasicWorld> basicWorld;
    
    boost::shared_ptr<NetworkPortal> network;
    boost::shared_ptr<History::Historian> historian;
    
    boost::shared_ptr<ClientData> clientData;
    
    boost::shared_ptr<Sound::SoundSystem> soundSystem;
    
    bool isConnectedToNetwork;
public:
    bool tryConnect(const std::string &host, unsigned short port);
    void constructAfterConnect(Map::HRMap *map);
    
    World::PlayerManager *getPlayerManager()
        { return basicWorld->getPlayerManager(); }
    ClientData *getClientData() { return clientData.get(); }
    Object::WorldManager *getWorldManager()
        { return basicWorld->getWorldManager(); }
    
    void checkNetwork();
    void doPhysics();
	void doAI();
    
    void render();
private:
    bool doConnect(const std::string &host, unsigned short port);
    void doNotConnect();
};

}  // namespace SDL
}  // namespace Project

#endif
