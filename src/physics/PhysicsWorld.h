#ifndef PROJECT_PHYSICS__PHYSICS_WORLD_H
#define PROJECT_PHYSICS__PHYSICS_WORLD_H

#include <vector>
#include <string>
#include "math/Point.h"
#include "math/BoundingBox3D.h"
#include "log/Logger.h"
#include "PhysicalPlayer.h"
#include "DebugDrawer.h"

#include "event/TypedObserver.h"
#include "event/SetDebugDrawing.h"

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
    class DebugDrawingObserver
        : public Event::TypedObserver<Event::SetDebugDrawing> {
    public:
        virtual void observe(Event::SetDebugDrawing *event);
    };
    
    friend class DebugDrawingObserver;
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
    
    double raycastLength(const Math::Point &from, const Math::Point &to);
    
    DebugDrawer &getDebug() { return debug; }
private:
    void setDebug(bool on);
private:
    btBroadphaseInterface *broadPhaseInterface;
    btCollisionDispatcher *collisionDispatcher;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btConstraintSolver *constraintSolver;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
    DebugDrawer debug;
    bool debugging;
    
    std::vector<btRigidBody*> collisionBodies;
    std::vector<PhysicalPlayer*> playerEntities;
};

}  // namespace Physics
}  // namespace Project

#endif
