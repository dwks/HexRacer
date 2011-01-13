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
  void setupPhysicsWorld();
  void stepWorld(float microseconds);
  void setGravity(float xAccel, float yAccel, float zAccel);
  void createRigidGenericBoxShape(float width, float height, float depth, Math::Point origin, float _mass);
  void createTestScene();
  
  void render();
  
  std::vector<btRigidBody*> collisionShapes;
  
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
