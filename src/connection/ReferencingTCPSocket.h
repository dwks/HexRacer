#ifndef PROJECT_CONNECTION__REFERENCING_TCPSOCKET_H
#define PROJECT_CONNECTION__REFERENCING_TCPSOCKET_H

#include "AbstractTCPSocket.h"

namespace Project {
namespace Connection {

/** A TCPSocket which is a wrapper around a boost TCP socket that has already
    been created elsewhere.
*/
class ReferencingTCPSocket : public AbstractTCPSocket {
private:
    tcp::socket &socket;
public:
    ReferencingTCPSocket(tcp::socket &socket);
    
    virtual bool open();
protected:
    virtual tcp::socket &getSocket() { return socket; }
};

}  // namespace Connection
}  // namespace Project

#endif
