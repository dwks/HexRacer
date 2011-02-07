#ifndef PROJECT_PHYSICS__PHYSICAL_PLAYER_H
#define PROJECT_PHYSICS__PHYSICAL_PLAYER_H

#include "PhysicalObject.h"
#include "Converter.h"

#include "boost/serialization/access.hpp"
#include "boost/serialization/split_member.hpp"

#include "log/Logger.h"

namespace Project {
namespace Physics {

class PhysicalPlayer : public PhysicalObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void save(Archive &ar, const unsigned version) const {
        Math::Matrix transformation = getTransformation();
        Math::Point linearVelocity
            = Converter::toPoint(rigidBody->getLinearVelocity());
        Math::Point angularVelocity
            = Converter::toPoint(rigidBody->getAngularVelocity());
        
        ar << transformation << linearVelocity << angularVelocity;
    }
    
    template <typename Archive>
    void load(Archive &ar, const unsigned version) {
        Math::Matrix transformation;
        Math::Point linearVelocity, angularVelocity;
        ar >> transformation >> linearVelocity >> angularVelocity;
        
        //LOG(OPENGL, transformation);
        
        constructRigidBody(transformation);
        
        rigidBody->setLinearVelocity(
            Converter::toVector(linearVelocity));
        rigidBody->setAngularVelocity(
            Converter::toVector(angularVelocity));
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
    btRigidBody *rigidBody;
    bool onGround;  // not serialized
    double speedBoost;  // not serialized
public:
    PhysicalPlayer() : rigidBody(NULL), onGround(false), speedBoost(1.0) {}
    PhysicalPlayer(const Math::Point &position, const Math::Point &direction);
    virtual ~PhysicalPlayer();
    
    void setOnGround(bool onGround)
        { this->onGround = onGround; }
    bool getOnGround() const { return onGround; }
    void setSpeedBoost(double speedBoost)
        { this->speedBoost = speedBoost; }
    double getSpeedBoost()
        { return this->speedBoost; }
    
    void destroyRigidBody();
    void constructRigidBody(const Math::Point &position);
    void constructRigidBody(const Math::Matrix &transformation);
    
    virtual Math::Point getOrigin() const;
    virtual Math::Matrix getTransformation() const;
    virtual Math::Point getLinearVelocity() const;
    virtual Math::Point getAngularVelocity() const;
    
    virtual Math::Point getFrontDirection() const;
    virtual Math::Point getRightDirection() const;
    virtual Math::Point getUpDirection() const;
    
    /** Applies acceleration in the range [-1.0,+1.0], where +1.0 is full
        throttle forwards and -1.0 is backwards.
    */
    void applyAcceleration(double acceleration);
    
    /** Turns by a value @a amount in the range [-1.0,+1.0], where -1.0
        means turn fully left, and +1.0 means turn fully right.
    */
    void applyTurning(double amount);
    
    /** Applies jumping force.
    */
    void doJump();
    
    void applyForce(const Math::Point &force);
    void applyForce(const Math::Point &force, const Math::Point &at);
    void applyTorque(const Math::Point &torque);
    
    virtual void setData(const Math::Matrix &transform,
        const Math::Point &linearVelocity,
        const Math::Point &angularVelocity);
};

}  // namespace Physics
}  // namespace Project

#endif
