#include "PhysicsFactory.h"

namespace Project {
namespace Physics {

btRigidBody* PhysicsFactory::createRigidStaticPlane(Math::Point planeNormal, Math::Point origin){
    LOG2(PHYSICS, CREATE,
    "Creating Static Plane: Normal: " << planeNormal
        << " Location: " << origin);
    btCollisionShape* groundShape = new btStaticPlaneShape(Converter::toVector(planeNormal), 1);
    
    btDefaultMotionState* groundMotionState
        = new btDefaultMotionState(btTransform(
            btQuaternion(0,0,0,1),Converter::toVector(origin)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
        0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
    
    //collisionBodies.push_back(rigidBody);
    //dynamicsWorld->addRigidBody(rigidBody);
    
    return rigidBody;
}

btRigidBody* PhysicsFactory::createRigidSphere(float radius, Math::Point origin, float mass){
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

btRigidBody* PhysicsFactory::createRigidBox(float width, float height, float depth, Math::Point origin, float mass){
    /*LOG2(PHYSICS, CREATE,
        "Creating BoxShape: W: " << width << " H: " << height << " D: " << depth
        << " Origin: " << origin << " Mass: " << mass);*/
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

btRigidBody* PhysicsFactory::createRigidTriMesh(std::vector<Math::Point> tri_array){
    btTriangleMesh *mTriMesh = new btTriangleMesh();

    for(unsigned int i=0;i<tri_array.size();i+=3) {
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

btRigidBody* PhysicsFactory::createRigidTriMesh(std::vector<Math::Triangle3D> tri_array){
    btTriangleMesh *mTriMesh = new btTriangleMesh();
    
    for(std::vector<Math::Triangle3D>::size_type x = 0; x < tri_array.size();
        x ++) {
        
        mTriMesh->addTriangle(
            Converter::toVector(tri_array[x].getVertex(0)),
            Converter::toVector(tri_array[x].getVertex(1)),
            Converter::toVector(tri_array[x].getVertex(2)));
    }
    
    btCollisionShape *mTriMeshShape = new btBvhTriangleMeshShape(mTriMesh,true);
    
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,mTriMeshShape,btVector3(0,0,0));
    btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
    
    return rigidBody;
}

PhysicalPlayer *PhysicsFactory::createPhysicalPlayer(const Math::Point &origin) {
    return new Physics::PhysicalPlayer(origin);
}

}  // namespace physics
}  // namespace Project
