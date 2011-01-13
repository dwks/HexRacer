#ifndef PROJECT_PHYSICS__PHYSICS_WORLD_H
#define PROJECT_PHYSICS__PHYSICS_WORLD_H

#include <vector>
#include "math/Point.h"
#include "log/Logger.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicsWorld {
public:
  PhysicsWorld();
  setupPhysicsWorld();
  stepWorld(float microseconds);
  setGravity(float xAccel, float yAccel, float zAccel);
  createRigidGenericBoxShape(float width, float height, float depth, Math::Point origin, float mass);
  createTestScene();
  
  btAlignedObjectArray<btCollisionShape*> collisionShapes;
  
private:
  btBroadphaseInterface *broadPhaseInterface;
  btCollisionDispatcher *collisionDispatcher;
  btDefaultCollisionConfiguration *collisionConfiguration;
  btConstraintSolver *constraintSolver;
  btDiscreteDynamicsWorld *dynamicsWorld;
};

}  // namespace Physics
}  // namespace Project

#endif
