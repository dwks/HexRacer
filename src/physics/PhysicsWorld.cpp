#include <cstddef>  // for std::size_t
#include <cfloat>
#include "PhysicsWorld.h"
#include "Converter.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/OpenGL.h"

#include "event/ObserverList.h"
#include "PhysicsFactory.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace Physics {

PhysicsWorld *PhysicsWorld::instance;

void PhysicsWorld::DebugDrawingObserver::observe(Event::SetDebugDrawing *event) {
    PhysicsWorld::getInstance()->setDebug(event->getOn());
}

PhysicsWorld::PhysicsWorld() {
    instance = this;
    debugging = false;
    setupPhysicsWorld();
}

PhysicsWorld::~PhysicsWorld() {
    /*for(std::vector<btRigidBody*>::iterator i = collisionBodies.begin();
        i != collisionBodies.end(); ++ i) {
        
        destroyRigidBody(*i);
        delete (*i);
    }*/
    
    // delete in reverse order of allocation, more or less
    delete dynamicsWorld;
    delete constraintSolver;
    delete collisionConfiguration;
    delete collisionDispatcher;
    delete broadPhaseInterface;
}

void PhysicsWorld::stepWorld(float microseconds) {
    //LOG2( PHYSICS, TIMESTEP, "Stepping simulation by: " << microseconds << " microseconds");
    
    if ( dynamicsWorld ) {
        dynamicsWorld->stepSimulation ( microseconds / 1000000.f );
    }
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
    
    double gravity = GET_SETTING("physics.constant.gravity", 9.81);
    dynamicsWorld->setGravity ( btVector3 ( 0.0,-gravity,0.0 ) );
    
    LOG2 ( PHYSICS, INIT, "Physics Setup Completed!" );
    
    ADD_OBSERVER(new DebugDrawingObserver());
}

void PhysicsWorld::setGravity ( float xAccel, float yAccel, float zAccel ) {
    dynamicsWorld->setGravity ( btVector3 ( xAccel,yAccel,zAccel ) );
}

void PhysicsWorld::setDebug(bool on) {
    if(on) {
        debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        dynamicsWorld->setDebugDrawer(&debug);
    }
    else {
        dynamicsWorld->setDebugDrawer(NULL);
    }
    
    debugging = on;
}

void PhysicsWorld::render() {
    if(debugging) {
        dynamicsWorld->debugDrawWorld();
    }
}

double PhysicsWorld::raycastLength(const Math::Point &from,
    const Math::Point &to) {
    
    btVector3 fromv = Converter::toVector(from);
    btVector3 tov = Converter::toVector(to);
    
    btCollisionWorld::ClosestRayResultCallback ray(fromv, tov);
    
    dynamicsWorld->rayTest(fromv, tov, ray);
    
    if(ray.hasHit()) {
        return (from - Converter::toPoint(ray.m_hitPointWorld)).length();
    }

	return DBL_MAX;
}

}  // namespace Physics
}  // namespace Project
