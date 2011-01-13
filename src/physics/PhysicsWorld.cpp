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
}

void PhysicsWorld::createTestScene(){
    //The "Plane"
    createRigidStaticPlane(Math::Point(0.0,1.0,0.0), Math::Point(0.0,0.0,0.0));
    
    //A player
    createPlayer(0);
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

void PhysicsWorld::createPlayer(int playerID){
    LOG2(PHYSICS, CREATE, "Creating Player. ID: " << playerID);
    Physics::PhysicalPlayer* player = new Physics::PhysicalPlayer(PhysicsWorld::createRigidBox(2.0,2.0,2.0,Math::Point(0.0,2.0,0.0),2.0));
    
    playerEntities.push_back(player);
}


btRigidBody* PhysicsWorld::createRigidStaticPlane(Math::Point planeNormal, Math::Point origin){
    LOG2(PHYSICS, CREATE, "Creating Static Plane: Normal: " << planeNormal.getX() << " " << planeNormal.getY() << " " << planeNormal.getZ() << " Location: " << origin.getX() << " " << origin.getY()<< " " << origin.getZ());
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(planeNormal.getX(),planeNormal.getY(),planeNormal.getZ()),1);

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(origin.getX(),origin.getY(),origin.getZ())));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
    
    collisionBodies.push_back(rigidBody);
    dynamicsWorld->addRigidBody(rigidBody);
    
    return rigidBody;
}

btRigidBody* PhysicsWorld::createRigidSphere(float radius, Math::Point origin, float mass){
    LOG2(PHYSICS, CREATE, "Creating Sphere: Radius: " << radius << " Origin: " << origin.getX() << ", " << origin.getY() << ", " << origin.getZ() << " Mass: " << mass);
    btCollisionShape* sphereShape = new btSphereShape ( radius );
    
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(origin.getX(),origin.getY(),origin.getZ())));
    btVector3 fallInertia(0,0,0);
    sphereShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,sphereShape,fallInertia);
    btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
        
    collisionBodies.push_back(rigidBody);
    dynamicsWorld->addRigidBody(rigidBody);
    
    return rigidBody;
}

btRigidBody* PhysicsWorld::createRigidBox(float width, float height, float depth, Math::Point origin, float mass){
    LOG2(PHYSICS, CREATE, "Creating BoxShape: W: " << width << " H: " << height << " D: " << depth << " Origin: " << origin.getX() << ", " << origin.getY() << ", " << origin.getZ() << " Mass: " << mass);
    btCollisionShape* boxShape = new btBoxShape ( btVector3 (width,height,depth) );
    
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(origin.getX(),origin.getY(),origin.getZ())));
    btVector3 fallInertia(0,0,0);
    boxShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,boxShape,fallInertia);
    btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
        
    collisionBodies.push_back(rigidBody);
    dynamicsWorld->addRigidBody(rigidBody);
    
    return rigidBody;
}

void PhysicsWorld::render() {
    for(std::size_t x = 0; x < collisionBodies.size(); x ++) {
        
    }
}

}  // namespace Physics
}  // namespace Project
