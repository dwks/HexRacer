#ifndef PROJECT_CONNECTION__SERVER_SOCKET_H
#define PROJECT_CONNECTION__SERVER_SOCKET_H

namespace Project {
namespace Connection {

class ServerSocket {
public:
    virtual ~ServerSocket() {}
    
    /**
        @return True upon success.
    */
    virtual bool listen(unsigned short port) = 0;
};

}  // namespace Connection
}  // namespace Project

#endif
