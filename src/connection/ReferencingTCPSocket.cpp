#include "ReferencingTCPSocket.h"

namespace Project {
namespace Connection {

ReferencingTCPSocket::ReferencingTCPSocket(tcp::socket &socket)
    : socket(socket) {
    
}

bool ReferencingTCPSocket::open() {
    return true;  // nothing to do, always succeed
}

}  // namespace Connection
}  // namespace Project
