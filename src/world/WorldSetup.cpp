#include "WorldSetup.h"

namespace Project {
namespace World {

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

}  // namespace World
}  // namespace Project
