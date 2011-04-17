#include "WorldSetup.h"
#include "object/Player.h"
#include "map/Teams.h"

#include "log/Logger.h"

namespace Project {
namespace World {

WorldSetup WorldSetup::instance;  // singleton instance

void WorldSetup::addPlayerSettings(int id) {
    playerSettings[id] = PlayerSettings();
    playerSettings[id].setID(id);
    
    playerSettings[id].setName(Object::Player::getDefaultPlayerName(id));
	playerSettings[id].setColor((id / 2) % Map::Teams::MAX_TEAMS);
}

void WorldSetup::addClientSettings(int id) {
    clientSettings[id] = ClientSettings();
    clientSettings[id].setID(id);
}

void WorldSetup::removePlayer(int id) {
    LOG(WORLD, "Removing player " << id);
    
    PlayerSettingsList::iterator found2 = playerSettings.find(id);
    if(found2 != playerSettings.end()) {
        playerSettings.erase(found2);
    }
    
    // ...and...
    ClientSettingsList::iterator found = clientSettings.find(id);
    if(found != clientSettings.end()) {
        clientSettings.erase(found);
    }
}

WorldSetup::PlayerSettings *WorldSetup::getPlayerSettings(int id) {
    PlayerSettingsList::iterator found = playerSettings.find(id);
    if(found == playerSettings.end()) return NULL;
    
    return &(*found).second;
}

WorldSetup::ClientSettings *WorldSetup::getClientSettings(int id) {
    ClientSettingsList::iterator found = clientSettings.find(id);
    if(found == clientSettings.end()) return NULL;
    
    return &(*found).second;
}

bool WorldSetup::everyoneReadyToStart() const {
    // need at least one client for the server to start
    if(clientSettings.empty()) return false;
    
    for(ClientSettingsList::const_iterator i = clientSettings.begin();
        i != clientSettings.end(); ++ i) {
        
        if(!(*i).second.isReadyToStart()) return false;
    }
    
    return true;
}

bool WorldSetup::everyoneFullyLoaded() const {
    for(ClientSettingsList::const_iterator i = clientSettings.begin();
        i != clientSettings.end(); ++ i) {
        
        if(!(*i).second.isFullyLoaded()) return false;
    }
    
    return true;
}

int WorldSetup::getPlayersOnTeam(int team) const {
    int count = 0;
    
    for(PlayerSettingsList::const_iterator i = playerSettings.begin();
        i != playerSettings.end(); ++ i) {
        
        if((*i).second.getColor() == team) {
            count ++;
        }
    }
    
    return count;
}

void WorldSetup::getAllPlayerIDs(std::vector<int> &list) {
    list.clear();
    
    for(PlayerSettingsList::const_iterator i = playerSettings.begin();
        i != playerSettings.end(); ++ i) {
        
        list.push_back((*i).first);
    }
}

void WorldSetup::debugDump() {
    LOG(NETWORK, "Clients:");
    for(ClientSettingsList::const_iterator i = clientSettings.begin();
        i != clientSettings.end(); ++ i) {
        
        LOG(NETWORK, "    " << (*i).first);
    }
    
    LOG(NETWORK, "Players:");
    for(PlayerSettingsList::const_iterator i = playerSettings.begin();
        i != playerSettings.end(); ++ i) {
        
        LOG(NETWORK, "    " << (*i).first);
    }
}

}  // namespace World
}  // namespace Project
