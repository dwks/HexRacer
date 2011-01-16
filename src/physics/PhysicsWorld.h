#ifndef PROJECT_PHYSICS__PHYSICS_WORLD_H
#define PROJECT_PHYSICS__PHYSICS_WORLD_H

#include <vector>
#include <string>
#include "math/Point.h"
#include "log/Logger.h"
#include "PhysicalPlayer.h"

#include "event/TypedObserver.h"
#include "event/PlayerMovement.h"

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
  
  PhysicalPlayer* createPlayer(int playerID, Math::Point origin = Math::Point());
  
  btRigidBody* createRigidStaticPlane(Math::Point planeNormal, Math::Point origin);
  btRigidBody* createRigidSphere(float radius, Math::Point origin, float mass);
  btRigidBody* createRigidBox(float width, float height, float depth, Math::Point origin, float mass);
  void createTestScene();
  
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
  
  std::vector<btRigidBody*> collisionBodies;
  std::vector<PhysicalPlayer*> playerEntities;
};

}  // namespace Physics
}  // namespace Project

#endif
