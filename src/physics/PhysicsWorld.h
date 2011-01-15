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
private:
    class PlayerMovementHandler
        : public Event::TypedObserver<Event::PlayerMovement> {
    private:
        btRigidBody *player;
    public:
        PlayerMovementHandler(btRigidBody *player) : player(player) {}
        
        virtual void observe(Event::PlayerMovement *event);
    };
public:
  PhysicsWorld();
  void setupPhysicsWorld();
  void stepWorld(float microseconds);
  void setGravity(float xAccel, float yAccel, float zAccel);
  
  void createPlayer(int playerID);
  
  btRigidBody* createRigidStaticPlane(Math::Point planeNormal, Math::Point origin);
  btRigidBody* createRigidSphere(float radius, Math::Point origin, float mass);
  btRigidBody* createRigidBox(float width, float height, float depth, Math::Point origin, float mass);
  void createTestScene();
  
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
