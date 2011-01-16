#ifndef PROJECT_PHYSICS__PHYSICAL_PLAYER_H
#define PROJECT_PHYSICS__PHYSICAL_PLAYER_H

#include "PhysicalObject.h"

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
        Math::Point origin = getOrigin();
        ar << origin;
    }
    
    template <typename Archive>
    void load(Archive &ar, const unsigned version) {
        Math::Point origin;
        ar >> origin;
        constructRigidBody(origin);
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
    PhysicalPlayer() : primaryRigidBody(NULL) {}
    PhysicalPlayer(const Math::Point &origin);
    virtual ~PhysicalPlayer();
    
    void constructRigidBody(const Math::Point &origin);
    
    virtual Math::Point getOrigin() const;
    
    void applyMovement(const Math::Point &movement);
protected:
    btRigidBody* primaryRigidBody;
};

}  // namespace Physics
}  // namespace Project

#endif
