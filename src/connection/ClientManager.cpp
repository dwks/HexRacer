#include "ClientManager.h"

namespace Project {
namespace Connection {

ClientManager::~ClientManager() {
    for(socket_list_t::iterator i = socket_list.begin();
        i != socket_list.end(); ++ i) {
        
        delete (*i);
    }
}

void ClientManager::addSocket(Socket *socket) {
    socket_list.push_back(socket);
}

bool ClientManager::broadcast(const std::string &message) {
    return false;
}

bool ClientManager::tryReceive(Socket *&socket, std::string &message) {
    return false;
}

}  // namespace Connection
}  // namespace Project
