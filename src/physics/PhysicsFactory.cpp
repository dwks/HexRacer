#include "PhysicsFactory.h"

namespace Project {
namespace Physics {
    btRigidBody* createRigidStaticPlane(Math::Point planeNormal, Math::Point origin){
        LOG2(PHYSICS, CREATE,
            "Creating Static Plane: Normal: " << planeNormal
            << " Location: " << origin.getX() << " " << origin.getY()<< " " << origin.getZ());
        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0.0,0.0,0.0), 1);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
                                                                                       btVector3(origin.getX(),origin.getY(),origin.getZ())));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
        
        return rigidBody;   
    }
    
    btRigidBody* createRigidSphere(float radius, Math::Point origin, float mass){
        LOG2(PHYSICS, CREATE, "Creating Sphere: Radius: " << radius 
            << " Origin: " << origin.getX() << ", " << origin.getY() << ", " << origin.getZ() << " Mass: " << mass);
        btCollisionShape* sphereShape = new btSphereShape ( radius );
        
        btDefaultMotionState* fallMotionState
            = new btDefaultMotionState(
                btTransform(btQuaternion(0,0,0,1),
                    Converter::toVector(origin)));
        btVector3 fallInertia(0,0,0);
        sphereShape->calculateLocalInertia(mass,fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,sphereShape,fallInertia);
        btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
        
        return rigidBody;
    }
    
    btRigidBody* createRigidBox(float width, float height, float depth, Math::Point origin, float mass){
        LOG2(PHYSICS, CREATE,
            "Creating BoxShape: W: " << width << " H: " << height << " D: " << depth
            << " Origin: " << origin << " Mass: " << mass);
        btCollisionShape* boxShape = new btBoxShape ( btVector3 (width,height,depth) );
        
        btDefaultMotionState* fallMotionState
            = new btDefaultMotionState(
                btTransform(btQuaternion(0,0,0,1),
                    Converter::toVector(origin)));
        btVector3 fallInertia(0,0,0);
        boxShape->calculateLocalInertia(mass,fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,boxShape,fallInertia);
        btRigidBody* rigidBody = new btRigidBody(fallRigidBodyCI);
        
        return rigidBody;   
    }
    
    btRigidBody* createRigidTriMesh(std::vector<Math::Point> tri_array){
        btTriangleMesh *mTriMesh = new btTriangleMesh();

        for(u_int i=0;i<tri_array.size();i+=3) {
            btVector3 v0(tri_array[i].getX(),tri_array[i].getY(),tri_array[i].getZ());
            btVector3 v1(tri_array[i+1].getX(),tri_array[i+1].getY(),tri_array[i+1].getZ());
            btVector3 v2(tri_array[i+2].getX(),tri_array[i+2].getY(),tri_array[i+2].getZ());

            mTriMesh->addTriangle(v0,v1,v2);
        }

        btCollisionShape *mTriMeshShape = new btBvhTriangleMeshShape(mTriMesh,true);
        
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,mTriMeshShape,btVector3(0,0,0));
        btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
        
        return rigidBody;   
    }
}  // namespace physics
}  // namespace Project
