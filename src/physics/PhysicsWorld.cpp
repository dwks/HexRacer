#include <cstddef>  // for std::size_t
#include "PhysicsWorld.h"

namespace Project {
namespace Physics {

PhysicsWorld::PhysicsWorld() {
    setupPhysicsWorld();
}

void PhysicsWorld::stepWorld(float microseconds) {
    LOG2( PHYSICS, TIMESTEP, "Stepping simulation by: " << microseconds << " microseconds");
    if ( dynamicsWorld ) {
        dynamicsWorld->stepSimulation ( microseconds / 1000000.f );
    }
    for(int i=0;i<collisionShapes->getNumCollisionObjects();i++){
        collisionShapes[i].
    }
}

void PhysicsWorld::createTestScene(){
    //The "Plane"
    createRigidGenericBoxShape(50.0,5.0,50.0,Math::Point(0.0,-5.0,0.0),0.0);
    
    //Two Boxes
    createRigidGenericBoxShape(2.0,2.0,2.0,Math::Point(0.0,5.0,0.0),1.0);
    createRigidGenericBoxShape(2.0,2.0,2.0,Math::Point(4.0,5.0,0.0),1.0);
}

void PhysicsWorld::setupPhysicsWorld() {
    LOG2 ( PHYSICS, INIT, "Physics Setup Initialized..." );
    collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    collisionDispatcher = new   btCollisionDispatcher ( collisionConfiguration );

    broadPhaseInterface = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    constraintSolver = sol;

    dynamicsWorld = new btDiscreteDynamicsWorld ( collisionDispatcher,broadPhaseInterface,constraintSolver,collisionConfiguration );

    dynamicsWorld->setGravity ( btVector3 ( 0.0,-9.81,0.0 ) );
    LOG2 ( PHYSICS, INIT, "Physics Setup Completed!" );
}

void PhysicsWorld::setGravity ( float xAccel, float yAccel, float zAccel ) {
    dynamicsWorld->setGravity ( btVector3 ( xAccel,yAccel,zAccel ) );
}

void PhysicsWorld::createRigidGenericBoxShape ( float width, float height, float depth, Math::Point origin, float _mass ) {
    LOG2(PHYSICS, INITBOX, "Creating Generic Box Shape: W: " << width << " H: " << height << " D: " << depth << " Origin: " << origin.getX() << ", " << origin.getY() << ", " << origin.getZ() << " Mass: " << _mass);
    btCollisionShape* groundShape = new btBoxShape ( btVector3 ( btScalar ( width ),btScalar ( height ),btScalar ( depth ) ) );
    collisionShapes.push_back ( groundShape );

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin ( btVector3 ( origin.getX(),origin.getY(),origin.getZ() ) );

    btScalar mass ( _mass );

    bool isDynamic = ( mass != 0.f );

    btVector3 localInertia ( 0,0,0 );
    if ( isDynamic )
        groundShape->calculateLocalInertia ( mass,localInertia );

    btDefaultMotionState* myMotionState = new btDefaultMotionState ( groundTransform );
    btRigidBody::btRigidBodyConstructionInfo rbInfo ( mass,myMotionState,groundShape,localInertia );
    btRigidBody* body = new btRigidBody ( rbInfo );

    dynamicsWorld->addRigidBody ( body );
    LOG2(PHYSICS, INITBOX, "Finished Making the generic box.");
}

void PhysicsWorld::render() {
    for(std::size_t x = 0; x < collisionShapes.size(); x ++) {
        
    }
}

}  // namespace Physics
}  // namespace Project
