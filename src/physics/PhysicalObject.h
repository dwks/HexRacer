#ifndef PROJECT_PHYSICS__PHYSICAL_OBJECT_H
#define PROJECT_PHYSICS__PHYSICAL_OBJECT_H

#include "math/Point.h"
#include "math/Matrix.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicalObject {
protected:
    btRigidBody* primaryRigidBody; 
public:
    virtual ~PhysicalObject() {}
    
    virtual Math::Point getOrigin() const = 0;
    virtual Math::Matrix getTransformation() const = 0;
    
	virtual btRigidBody* getPrimaryRigidBody() { return primaryRigidBody; }
};

}  // namespace Physics
}  // namespace Project

#endif
