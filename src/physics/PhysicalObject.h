#ifndef PROJECT_PHYSICS__PHYSICAL_OBJECT_H
#define PROJECT_PHYSICS__PHYSICAL_OBJECT_H

#include "math/Point.h"
#include "math/Matrix.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicalObject {
public:
    virtual ~PhysicalObject() {}
    
    virtual Math::Point getOrigin() const = 0;
    virtual Math::Matrix getTransformation() const = 0;
    virtual Math::Point getLinearVelocity() const = 0;
    virtual Math::Point getAngularVelocity() const = 0;
    
    virtual Math::Point getFrontDirection() const = 0;
    virtual Math::Point getRightDirection() const = 0;
    virtual Math::Point getUpDirection() const = 0;
    
    virtual void setData(const Math::Matrix &transform,
        const Math::Point &linearVelocity,
        const Math::Point &angularVelocity,
        bool interpolate = true) = 0;
};

}  // namespace Physics
}  // namespace Project

#endif
