#ifndef PROJECT_OBJECT__PLAYER_H
#define PROJECT_OBJECT__PLAYER_H

#include "AbstractObject.h"
#include "math/Point.h"
#include "physics/PhysicalPlayer.h"
#include "render/RenderablePlayer.h"

namespace Project {
namespace Object {

class Player : public AbstractObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void save(Archive &ar, const unsigned version) const {
        ar & boost::serialization::base_object<AbstractObject>(*this);
        ar & physical;
    }
    
    template <typename Archive>
    void load(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<AbstractObject>(*this);
        ar & physical;
        initialize();
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
    Physics::PhysicalPlayer *physical;
    Render::RenderablePlayer *renderable;
public:
    Player();
    Player(int id, const Math::Point &origin);
    virtual ~Player();
    
    void setOnGround(bool onGround)
        { physical->setOnGround(onGround); }
    
    void setPhysicalObject(Physics::PhysicalPlayer *physical)
        { this->physical = physical; }
    virtual Physics::PhysicalObject *getPhysicalObject();
    
    void setRenderableObject(Render::RenderablePlayer *renderable)
        { this->renderable = renderable; }
    virtual Render::RenderableObject *getRenderableObject();
    
    void applyAcceleration(double acceleration)
        { physical->applyAcceleration(acceleration); }
    void applyTurning(double amount)
        { physical->applyTurning(amount); }
    void doJump() { physical->doJump(); }
    
    Math::Point getPosition() const;
    Math::Matrix getTransformation() const;
    void applyForce(const Math::Point &movement, const Math::Point &at);
    
    void initialize();
};

}  // namespace Object
}  // namespace Project

#endif
