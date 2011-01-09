#ifndef PROJECT_OBJECT__PLAYER_H
#define PROJECT_OBJECT__PLAYER_H

#include "math/Point.h"

namespace Project {
namespace Object {

class Player {
private:
    int id;
    Math::Point position;
public:
    Player(int id, Math::Point position) : id(id), position(position) {}
    
    int getID() const { return id; }
    Math::Point getPosition() const { return position; }
    void addPosition(Math::Point add) { position += add; }
};

}  // namespace Object
}  // namespace Project

#endif
