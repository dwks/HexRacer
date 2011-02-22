#ifndef PROJECT_SDL__GAME_WORLD_H
#define PROJECT_SDL__GAME_WORLD_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include <string>

#include "NetworkPortal.h"
#include "history/Historian.h"

#include "map/HRMap.h"
#include "map/RaceManager.h"

#include "PlayerManager.h"
#include "ClientData.h"

#include "physics/PhysicsWorld.h"
#include "physics/Suspension.h"

#include "map/PathManager.h"
#include "map/PathingUpdater.h"

namespace Project {
namespace SDL {

class GameWorld {
private:
    boost::shared_ptr<NetworkPortal> network;
    boost::shared_ptr<History::Historian> historian;
    boost::shared_ptr<Map::RaceManager> raceManager;
    
    boost::shared_ptr<PlayerManager> playerManager;
    boost::shared_ptr<ClientData> clientData;
    
    boost::shared_ptr<Object::WorldManager> worldManager;
    boost::shared_ptr<Physics::PhysicsWorld> physicsWorld;
    boost::shared_ptr<Physics::Suspension> suspension;

	boost::shared_ptr<Map::PathManager> pathManager;
    boost::shared_ptr<Map::PathingUpdater> pathingUpdater;
    
    bool isConnectedToNetwork;
public:
    void doConnect(const std::string &host, unsigned short port);
    
    void construct(const std::string &host, unsigned short port);
    void construct2(Map::HRMap *map);
    
    PlayerManager *getPlayerManager() { return playerManager.get(); }
    ClientData *getClientData() { return clientData.get(); }
    Object::WorldManager *getWorldManager() { return worldManager.get(); }
    
    void checkNetwork();
    void doPhysics();
	void doAI();
    
    void render();
};

}  // namespace SDL
}  // namespace Project

#endif
