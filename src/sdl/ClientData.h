#ifndef PROJECT_SDL__CLIENT_DATA_H
#define PROJECT_SDL__CLIENT_DATA_H

namespace Project {
namespace SDL {

class ClientData {
private:
    int id;
public:
    ClientData() : id(0) {}
    ClientData(int id) : id(id) {}
    
    int getPlayerID() const { return id; }
};

}  // namespace SDL
}  // namespace Project

#endif
