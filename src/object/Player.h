#ifndef PROJECT_OBJECT__PLAYER_H
#define PROJECT_OBJECT__PLAYER_H

#include "AbstractObject.h"
#include "math/Point.h"

namespace Project {
namespace Physics {
class PhysicalPlayer;
}  // namespace Physics
}  // namespace Project

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
    Physics::PhysicalPlayer *physical;
    Render::RenderableObject *renderable;
public:
    Player();
    Player(int id);
    
    virtual Physics::PhysicalObject *getPhysicalObject();
    virtual Render::RenderableObject *getRenderableObject();
    
    Math::Point getPosition() const;
    void applyMovement(const Math::Point &movement);
};

}  // namespace Object
}  // namespace Project

#endif
