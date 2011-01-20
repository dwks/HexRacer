#ifndef PROJECT_PHYSICS__PHYSICS_WORLD_H
#define PROJECT_PHYSICS__PHYSICS_WORLD_H

#include <vector>
#include <string>
#include "math/Point.h"
#include "math/BoundingBox3D.h"
#include "log/Logger.h"
#include "PhysicalPlayer.h"
#include "DebugDrawer.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicsWorld {
private:
  static PhysicsWorld *instance;
public:
  static PhysicsWorld *getInstance() { return instance; }
public:
  PhysicsWorld();
  void setupPhysicsWorld();
  void stepWorld(float microseconds);
  void setGravity(float xAccel, float yAccel, float zAccel);
  
  void createTestScene();
  
  void registerRigidBody(btRigidBody *body);
  
  /** This does not automatically free the memory for @a body.
  */
  void destroyRigidBody(btRigidBody *body);
  
  void render();
private:
  btBroadphaseInterface *broadPhaseInterface;
  btCollisionDispatcher *collisionDispatcher;
  btDefaultCollisionConfiguration *collisionConfiguration;
  btConstraintSolver *constraintSolver;
  btDiscreteDynamicsWorld *dynamicsWorld;
  
  DebugDrawer debug;
  
  std::vector<btRigidBody*> collisionBodies;
  std::vector<PhysicalPlayer*> playerEntities;
};

}  // namespace Physics
}  // namespace Project

#endif
