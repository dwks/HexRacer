#ifndef PROJECT_PHYSICS__PHYSICAL_PLAYER_H
#define PROJECT_PHYSICS__PHYSICAL_PLAYER_H

#include "PhysicalObject.h"

namespace Project {
namespace Physics {

class PhysicalPlayer : public PhysicalObject {
public:
    PhysicalPlayer(btRigidBody* pRigidBody);
    virtual Math::Point getOrigin();
    
protected:
    btRigidBody* primaryRigidBody; 
};

}  // namespace Physics
}  // namespace Project

#endif
