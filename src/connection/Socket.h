#ifndef PROJECT_CONNECTION__SOCKET_H
#define PROJECT_CONNECTION__SOCKET_H

#include <QObject>

#include <string>

namespace Project {
namespace Connection {

/** Base class for all (normal) sockets that connect to some port.
    
    Server sockets do not inherit from this class.
*/
class Socket : public QObject {
    Q_OBJECT
public:
    virtual ~Socket() {}
    
    /** Establishes a connection to @a host which is listening on port @a port.
        
        @return True upon success.
    */
    virtual bool open(const std::string &host, unsigned short port) = 0;
    
    /** Closes this socket, if open() was ever successfully called.
        
        This function will be automatically called when this Socket is
        destructed, and can safely be called more than once.
    */
    virtual void close() = 0;
signals:
    void disconnected();
};

}  // namespace Connection
}  // namespace Project

#endif
