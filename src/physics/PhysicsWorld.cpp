#include "PhysicsWorld.h"

namespace Project {
namespace Physics {

  PhysicsWorld::setupWorld(){
	///collision configuration contains default setup for memory, collision setup
	collisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	collisionDispatcher = new	btCollisionDispatcher(collisionConfiguration);

	broadPhaseInterface = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	constraintSolver = sol;

	dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher,broadPhaseInterface,constraintSolver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0.0,-9.81,0.0));
  }
  
  PhysicsWorld::setGravity(float xAccel, float yAccel, float zAccel){
    dynamicsWorld->setGravity(btVector3(xAccel,yAccel,zAccel));
  }
  
}  // namespace Physics
}  // namespace Project
