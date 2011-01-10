#ifndef PROJECT_OBJECT__PLAYER_H
#define PROJECT_OBJECT__PLAYER_H

#include "math/Point.h"

#include "boost/serialization/access.hpp"

namespace Project {
namespace Object {

class Player {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & id;
        ar & position;
    }
private:
    int id;
    Math::Point position;
public:
    Player() : id(-1) {}
    Player(int id, Math::Point position) : id(id), position(position) {}
    
    int getID() const { return id; }
    Math::Point getPosition() const { return position; }
    void addPosition(Math::Point add) { position += add; }
};

}  // namespace Object
}  // namespace Project

#endif
