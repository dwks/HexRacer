#include "ClientManager.h"

#include "misc/Sleeper.h"
#include "log/Logger.h"

namespace Project {
namespace Server {

ClientManager::ClientManager() {
    allowableTimeout = 0;
}

ClientManager::~ClientManager() {
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        delete (*i);
    }
}

void ClientManager::addClient(Connection::Socket *socket, int id) {
    //while(int(portal_list.size()) <= id) portal_list.push_back(NULL);
    portal_list.push_back(new Network::SinglePortal(socket));
    
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    timeout.push_back(now);
    
    gameid.push_back(id);
    
    LOG(NETWORK, "Registering new connection for client " << id);
}

int ClientManager::getGameID(int which) {
    LOG(NETWORK, "***** Game ID of " << which << " is " << gameid[which]);
    
    return gameid[which];
}

void ClientManager::forceDisconnect(int which) {
    portal_list[which]->forceClose();
}

void ClientManager::sendPacket(Network::Packet *packet) {
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        if(*i) {
            (*i)->sendPacket(packet);
        }
    }
}

void ClientManager::sendPacketExcept(Network::Packet *packet, int exception) {
    for(portal_list_t::size_type i = 0; i < portal_list.size(); ++ i) {
        if(int(i) != exception && portal_list[i]) {
            portal_list[i]->sendPacket(packet);
        }
    }
}

void ClientManager::sendPacketOnly(Network::Packet *packet, int which) {
    portal_list[which]->sendPacket(packet);
}

void ClientManager::setPingTimeout(int client, unsigned long ms) {
    timeout[client] = ms;
    
    LOG(NETWORK, "client " << client << " set to " << ms);
    
    for(portal_list_t::size_type i = 0; i < portal_list.size(); ++ i) {
        LOG(NETWORK, "client " << i << ": " << portal_list[i]
            << " (" << gameid[i] << ")");
    }
}

void ClientManager::setAllowableTimeout(unsigned long allowableTimeout) {
    this->allowableTimeout = allowableTimeout;
}

int ClientManager::nextDisconnectedClient(int &gid) {
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    int x = 0;
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        Network::SinglePortal *portal = *i;
        
        if(!portal) {
            x ++;
            continue;
        }
        
        if(!portal->isOpen()) {
            *i = NULL;
            gid = gameid[x];
            portal_list.erase(i);
            timeout.erase(timeout.begin() + x);
            gameid.erase(gameid.begin() + x);
            delete portal;
            return x;
        }
        
        if(now >= timeout[x] + allowableTimeout) {
            *i = NULL;
            gid = gameid[x];
            portal_list.erase(i);
            timeout.erase(timeout.begin() + x);
            gameid.erase(gameid.begin() + x);
            delete portal;
            return x;
        }
        
        x ++;
    }
    
    return -1;
}

Network::Packet *ClientManager::nextPacket(int *whichSocket) {
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        if(!*i) continue;
        
        Network::Packet *packet = (*i)->nextPacket();
        if(packet) {
            if(whichSocket) *whichSocket = i - portal_list.begin();
            return packet;
        }
    }
    
    return NULL;
}

bool ClientManager::socketExists(int which) const {
    return portal_list[which] != NULL;
}

int ClientManager::getSocketCount() const {
    return static_cast<int>(portal_list.size());
}

int ClientManager::getClientsStillConnected() const {
    int count = 0;
    
    for(portal_list_t::const_iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        if(*i != NULL) count ++;
    }
    
    return count;
}

}  // namespace Server
}  // namespace Project
