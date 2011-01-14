#ifndef PROJECT_OBJECT__PLAYER_H
#define PROJECT_OBJECT__PLAYER_H

#include "AbstractObject.h"
#include "math/Point.h"

namespace Project {
namespace Object {

class Player : public AbstractObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<AbstractObject>(*this);
    }
private:
    Physics::PhysicalObject *physical;
    Render::RenderableObject *renderable;
public:
    Player();
    Player(int id);
    
    virtual Physics::PhysicalObject *getPhysicalObject()
        { return physical; }
    virtual Render::RenderableObject *getRenderableObject()
        { return renderable; }
    
    Math::Point getPosition() const { return Math::Point(); }
    void addPosition(Math::Point add) {}
};

}  // namespace Object
}  // namespace Project

#endif
