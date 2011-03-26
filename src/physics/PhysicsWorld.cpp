#include <cstddef>  // for std::size_t
#include <cfloat>
#include "PhysicsWorld.h"
#include "Converter.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/OpenGL.h"

#include "event/PhysicsTick.h"
#include "event/PhysicsCollision.h"
#include "event/EventSystem.h"
#include "PhysicsFactory.h"

#include "settings/SettingsManager.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

PhysicsWorld *PhysicsWorld::instance;

void PhysicsWorld::setDebugDrawingHandler(Event::SetDebugDrawing *event) {
    setDebug(event->getOn());
}

PhysicsWorld::PhysicsWorld() {
    instance = this;
    debugging = false;
    setupPhysicsWorld();
    registerTickCallback();
    
    METHOD_OBSERVER(&PhysicsWorld::setDebugDrawingHandler);
}

PhysicsWorld::~PhysicsWorld() {
    for(std::vector<btRigidBody*>::iterator i = collisionBodies.begin();
        i != collisionBodies.end(); ++ i) {
        
        //destroyRigidBody(*i);
        dynamicsWorld->removeRigidBody(*i);
        delete (*i);
    }
    
    // delete in reverse order of allocation, more or less
    delete dynamicsWorld;
    delete constraintSolver;
    delete collisionConfiguration;
    delete collisionDispatcher;
    delete broadPhaseInterface;
}

void PhysicsWorld::stepWorld(unsigned long milliseconds) {
    //LOG2( PHYSICS, TIMESTEP, "Stepping simulation by: " << milliseconds << " milliseconds");
    
    if ( dynamicsWorld ) {
        // allow at most some number of physics timesteps (at 60 FPS)
        dynamicsWorld->stepSimulation ( milliseconds / 1000.0,
            GET_SETTING( "physics.maxtimesteps", 5 ));
                
        //This code detects collisions
        //This is directly from a wiki tutorial found at
        //http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Callbacks_and_Triggers#Contact_Information
        int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            //btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
            //btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
    
            int numContacts = contactManifold->getNumContacts();
            for (int j=0;j<numContacts;j++)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getDistance()<0.f)
                {
                    float collisionImpulse = pt.getAppliedImpulse();
                    const btVector3& ptA = pt.getPositionWorldOnA();
                    Math::Point collisionPoint = Converter::toPoint(ptA);
                    EMIT_EVENT(new Event::PhysicsCollision(collisionPoint,collisionImpulse));
                }
            }
        }
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
    if (debugging) {
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

bool PhysicsWorld::raycastPoint(const Math::Point &from, const Math::Point &to, Math::Point* point) {

	btVector3 fromv = Converter::toVector(from);
    btVector3 tov = Converter::toVector(to);
    
    btCollisionWorld::ClosestRayResultCallback ray(fromv, tov);
    
    dynamicsWorld->rayTest(fromv, tov, ray);
    
    if (ray.hasHit()) {
		if (point) {
			*point = Converter::toPoint(ray.m_hitPointWorld);
		}
		return true;
    }

	if (point) {
		*point = Math::Point(to);
	}

	return false;

}

void PhysicsWorld::allRaycastPoints(const Math::Point &from,
    const Math::Point &to, std::vector<Math::Point> &points) {
    
    btVector3 fromv = Converter::toVector(from);
    btVector3 tov = Converter::toVector(to);
    
    btCollisionWorld::AllHitsRayResultCallback ray(fromv, tov);
    
    dynamicsWorld->rayTest(fromv, tov, ray);
    
    for(int i = 0; i < ray.m_hitPointWorld.size(); i ++) {
        points.push_back(Converter::toPoint(ray.m_hitPointWorld[i]));
    }
}

void tickCallback(btDynamicsWorld *world, btScalar timeStep) {
    EMIT_EVENT(new Event::PhysicsTick(timeStep));
    
    //LOG(PHYSICS, "Physics ticked by " << timeStep);
}

void PhysicsWorld::registerTickCallback() {
    dynamicsWorld->setInternalTickCallback(tickCallback, NULL, true);
}

}  // namespace Physics
}  // namespace Project
