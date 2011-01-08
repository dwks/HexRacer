#include "ReferencingTCPSocket.h"

namespace Project {
namespace Connection {

ReferencingTCPSocket::ReferencingTCPSocket(tcp::socket &socket)
    : socket(socket) {
    
}

bool ReferencingTCPSocket::open() {
    return true;  // nothing to do, always succeed
}

void ReferencingTCPSocket::close() {
    socket.close();
}

}  // namespace Connection
}  // namespace Project
