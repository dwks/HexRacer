#ifndef PROJECT_PHYSICS__PHYSICS_WORLD_H
#define PROJECT_PHYSICS__PHYSICS_WORLD_H

#include <vector>
#include <string>
#include "math/Point.h"
#include "math/BoundingBox3D.h"
#include "log/Logger.h"
#include "PhysicalPlayer.h"
#include "DebugDrawer.h"

#include "event/Enabler.h"
#include "event/SetDebugDrawing.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Project {
namespace Physics {

class PhysicsWorld : public Event::Enabler {
private:
    static PhysicsWorld *instance;
public:
    static PhysicsWorld *getInstance() { return instance; }
protected:
    void setDebugDrawingHandler(Event::SetDebugDrawing *event);
public:
    PhysicsWorld();
    ~PhysicsWorld();
    
    void setupPhysicsWorld();
    void stepWorld(unsigned long milliseconds);
    void setGravity(float xAccel, float yAccel, float zAccel);
    
    void registerRigidBody(btRigidBody *body);
    
    /** This does not automatically free the memory for @a body.
    */
    void destroyRigidBody(btRigidBody *body);
    
    void render();
    
    double raycastLength(const Math::Point &from, const Math::Point &to);

	bool raycastPoint(const Math::Point &from, const Math::Point &to, Math::Point* point = NULL);
    
    void allRaycastPoints(const Math::Point &from, const Math::Point &to,
        std::vector<Math::Point> &points);
    
    DebugDrawer &getDebug() { return debug; }
private:
    void setDebug(bool on);
    
    void registerTickCallback();
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
