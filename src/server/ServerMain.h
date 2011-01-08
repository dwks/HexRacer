#ifndef PROJECT_SERVER__SERVER_MAIN_H
#define PROJECT_SERVER__SERVER_MAIN_H

namespace Project {
namespace Server {

class ServerMain {
private:
    int clientCount;
public:
    ServerMain() : clientCount(0) {}
    
    void run();
};

}  // namespace Server
}  // namespace Project

#endif
