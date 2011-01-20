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
            = Converter::toPoint(primaryRigidBody->getLinearVelocity());
        Math::Point angularVelocity
            = Converter::toPoint(primaryRigidBody->getAngularVelocity());
        
        ar << transformation << linearVelocity << angularVelocity;
    }
    
    template <typename Archive>
    void load(Archive &ar, const unsigned version) {
        Math::Matrix transformation;
        Math::Point linearVelocity, angularVelocity;
        ar >> transformation >> linearVelocity >> angularVelocity;
        constructRigidBody(transformation);
        
        primaryRigidBody->setLinearVelocity(
            Converter::toVector(linearVelocity));
        primaryRigidBody->setAngularVelocity(
            Converter::toVector(angularVelocity));
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
    PhysicalPlayer() : primaryRigidBody(NULL) {}
    PhysicalPlayer(const Math::Point &position);
    virtual ~PhysicalPlayer();
    
    void destroyRigidBody();
    void constructRigidBody(const Math::Point &position);
    void constructRigidBody(const Math::Matrix &transformation);
    
    virtual Math::Point getOrigin() const;
    virtual Math::Matrix getTransformation() const;
    
    void applyMovement(const Math::Point &movement);
protected:
    btRigidBody* primaryRigidBody;
};

}  // namespace Physics
}  // namespace Project

#endif
