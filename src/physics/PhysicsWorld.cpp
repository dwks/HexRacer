#include <cstddef>  // for std::size_t
#include "PhysicsWorld.h"
#include "Converter.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/OpenGL.h"

#include "event/ObserverList.h"
#include "PhysicsSerializer.h"
#include "PhysicsFactory.h"

namespace Project {
namespace Physics {

PhysicsWorld *PhysicsWorld::instance;

PhysicsWorld::PhysicsWorld() {
    instance = this;
    setupPhysicsWorld();
}

void PhysicsWorld::stepWorld(float microseconds) {
    //LOG2( PHYSICS, TIMESTEP, "Stepping simulation by: " << microseconds << " microseconds");
    
    if ( dynamicsWorld ) {
        dynamicsWorld->stepSimulation ( microseconds / 1000000.f );
    }
}

void PhysicsWorld::createTestScene(){
    //The "Plane"
    //createRigidStaticPlane(Math::Point(0.0,1.0,0.0), Math::Point(0.0,-1.0,0.0));
    
    registerRigidBody(PhysicsFactory::createRigidStaticPlane(
        Math::Point(0.0,1.0,0.0), Math::Point(0.0,-1.0,0.0)));
    
    //A player
    //createPlayer(10);
}

void PhysicsWorld::registerRigidBody(btRigidBody *body) {
    dynamicsWorld->addRigidBody(body);
    
    collisionBodies.push_back(body);
}

void PhysicsWorld::destroyRigidBody(btRigidBody *body) {
    dynamicsWorld->removeRigidBody(body);
    
    std::vector<btRigidBody*>::iterator found = std::find(
        collisionBodies.begin(), collisionBodies.end(), body);
    if(found != collisionBodies.end()) {
        collisionBodies.erase(found);
    }
}

void PhysicsWorld::setupPhysicsWorld() {
    LOG2 ( PHYSICS, INIT, "Physics Setup Initialized..." );
    collisionConfiguration = new btDefaultCollisionConfiguration();
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    collisionDispatcher = new btCollisionDispatcher ( collisionConfiguration );
    
    broadPhaseInterface = new btDbvtBroadphase();
    
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver();
    constraintSolver = sol;
    
    dynamicsWorld = new btDiscreteDynamicsWorld ( collisionDispatcher,broadPhaseInterface,constraintSolver,collisionConfiguration );
    
    dynamicsWorld->setGravity ( btVector3 ( 0.0,-9.81,0.0 ) );
    LOG2 ( PHYSICS, INIT, "Physics Setup Completed!" );
}

void PhysicsWorld::setGravity ( float xAccel, float yAccel, float zAccel ) {
    dynamicsWorld->setGravity ( btVector3 ( xAccel,yAccel,zAccel ) );
}

void PhysicsWorld::render() {
    //collisionBodies[1]->applyCentralForce(btVector3(2.0f, 0.0f, 0.0f));
    
    static bool first = true;
    if(first) {
        first = false;
        
        PhysicsSerializer().serialize(collisionBodies[1]);
    }
    
    stepWorld(10 * 1000);  // step world by 10 ms
}

}  // namespace Physics
}  // namespace Project
