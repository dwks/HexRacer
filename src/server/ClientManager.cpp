#include "ClientManager.h"

namespace Project {
namespace Server {

ClientManager::~ClientManager() {
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        delete (*i);
    }
}

void ClientManager::addClient(Connection::Socket *socket) {
    portal_list.push_back(new Network::SinglePortal(socket));
}

void ClientManager::sendPacket(Network::Packet *packet) {
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        (*i)->sendPacket(packet);
    }
}

void ClientManager::sendPacketExcept(Network::Packet *packet, int exception) {
    for(portal_list_t::size_type i = 0; i < portal_list.size(); ++ i) {
        if(int(i) != exception) {
            portal_list[i]->sendPacket(packet);
        }
    }
}

void ClientManager::sendPacketOnly(Network::Packet *packet, int which) {
    portal_list[which]->sendPacket(packet);
}

int ClientManager::nextDisconnectedClient() {
    int x = 0;
    for(portal_list_t::iterator i = portal_list.begin();
        i != portal_list.end(); ++ i) {
        
        Network::SinglePortal *portal = *i;
        
        if(!portal->isOpen()) {
            portal_list.erase(i);
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

}  // namespace Server
}  // namespace Project
