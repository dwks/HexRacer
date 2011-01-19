#include <cstddef>  // for std::size_t
#include "PhysicsWorld.h"
#include "Converter.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/OpenGL.h"

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

//void PhysicsWorld::createTestScene(){
    //The "Plane"
    //createRigidStaticPlane(Math::Point(0.0,1.0,0.0), Math::Point(0.0,-2.0,0.0));
    
    //A player
    //createPlayer(0);
//}

void PhysicsWorld::setupPhysicsWorld() {
    LOG2 ( PHYSICS, INIT, "Physics Setup Initialized..." );
    collisionConfiguration = new btDefaultCollisionConfiguration();
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    collisionDispatcher = new btCollisionDispatcher ( collisionConfiguration );
    
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

//void PhysicsWorld::createPlayer(int playerID){
    //LOG2(PHYSICS, CREATE, "Creating Player. ID: " << playerID);
    //Physics::PhysicalPlayer* player = new Physics::PhysicalPlayer(PhysicsWorld::createRigidBox(2.0,2.0,2.0,Math::Point(0.0,0.0,0.0),2.0));
    
    //playerEntities.push_back(player);
//}

void PhysicsWorld::render() {
    //collisionBodies[1]->applyCentralForce(btVector3(2.0f, 0.0f, 0.0f));
    
    stepWorld(10 * 1000);  // step world by 10 ms
    
    for(std::size_t x = 0; x < collisionBodies.size(); x ++) {
        btTransform transform;
        collisionBodies[x]->getMotionState()->getWorldTransform(transform);
        
        /*LOG(PHYSICS, "body " << collisionBodies[x] << " at "
            << Converter::toPoint(transform.getOrigin()));*/
    }
    
    Math::BoundingBox3D box(2.0, 2.0, 2.0,
        Converter::toPoint(collisionBodies[1]->getCenterOfMassPosition()));
    OpenGL::GeometryDrawing::drawObject(box, true);
}

}  // namespace Physics
}  // namespace Project
