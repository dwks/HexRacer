#ifndef PROJECT_PHYSICS__PHYSICAL_PLAYER_H
#define PROJECT_PHYSICS__PHYSICAL_PLAYER_H

#include "PhysicalObject.h"

#include "boost/serialization/access.hpp"
#include "boost/serialization/split_member.hpp"

namespace Project {
namespace Physics {

class PhysicalPlayer : public PhysicalObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void save(Archive &ar, const unsigned version) const {
        Math::Point origin;
        ar & origin;
    }
    
    template <typename Archive>
    void load(Archive &ar, const unsigned version) {
        Math::Point origin;
        ar & origin;
        constructRigidBody(origin);
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
    PhysicalPlayer(btRigidBody* pRigidBody = NULL);
    
    virtual Math::Point getOrigin() const;
    
    void applyMovement(const Math::Point &movement);
private:
    void constructRigidBody(const Math::Point &origin);
protected:
    btRigidBody* primaryRigidBody;
};

}  // namespace Physics
}  // namespace Project

#if 0
namespace boost {
namespace serialization {

using namespace Project;

template <typename Archive>
void save_construct_data(Archive &ar,
    const Physics::PhysicalPlayer *player,
    const unsigned version) {
    
    ar << player->getOrigin();
}

template <typename Archive>
void load_construct_data(Archive &ar,
    Physics::PhysicalPlayer *player,
    const unsigned version) {
    
    Math::Point origin;
    ar >> origin;
    
    ::new(player) Physics::PhysicalPlayer(origin);
}

}  // namespace boost
}  // namespace serialization
#endif

#endif
