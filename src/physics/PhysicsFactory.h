#ifndef PROJECT_PHYSICS__PHYSICS_FACTORY_H
#define PROJECT_PHYSICS__PHYSICS_FACTORY_H

#include <vector>
#include <string>
#include "math/Point.h"
#include "math/Triangle3D.h"
#include "log/Logger.h"
#include "PhysicalPlayer.h"
#include "Converter.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicsFactory {
public:
    static btRigidBody* createRigidStaticPlane(Math::Point planeNormal, Math::Point origin);
    static btRigidBody* createRigidSphere(float radius, Math::Point origin, float mass);
    static btRigidBody* createRigidBox(float width, float height, float depth, Math::Point origin, float mass);
    static btRigidBody* createRigidTriMesh(std::vector <Math::Triangle3D> tri_array);
    static btRigidBody* createRigidTriMesh(std::vector <Math::Point> tri_array);
    
	static PhysicalPlayer *createPhysicalPlayer(const Math::Point &origin, const Math::Point &direction = Math::Point(0.0, 0.0, 1.0));
};

}  // namespace physics
}  // namespace Project

#endif
