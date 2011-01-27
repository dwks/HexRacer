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
        if(i != exception) {
            portal_list[i]->sendPacket(packet);
        }
    }
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

}  // namespace Server
}  // namespace Project
