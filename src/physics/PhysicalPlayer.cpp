#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsWorld.h"
#include "PhysicsFactory.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(const Math::Point &position) {
    primaryRigidBody = NULL;  // essential, constructRigidBody tries to delete
    
    constructRigidBody(position);
}

PhysicalPlayer::~PhysicalPlayer() {
    destroyRigidBody();
}

void PhysicalPlayer::destroyRigidBody() {
    if(primaryRigidBody) {
        PhysicsWorld::getInstance()->destroyRigidBody(primaryRigidBody);
        
        delete primaryRigidBody;  // works even if NULL
    }
}

void PhysicalPlayer::constructRigidBody(const Math::Point &position) {
    destroyRigidBody();
    
    //Get the bounding box from the mesh loader
	//Math::BoundingBox3D box(1.0, 1.0, 1.0);
	/*
	Render::MeshGroup* player_model = Render::MeshLoader::getInstance()->getModelByName("playerCube");
	if (player_model)
		box = player_model->getBoundingBox();

	primaryRigidBody = PhysicsWorld::getInstance()->createRigidBox(
		box.widthX(), box.widthY(), box.widthZ(), origin, 2.0);

	*/
	
    primaryRigidBody = Physics::PhysicsFactory
        ::createRigidBox(1.0,1.0,1.0,position,2.0);
    PhysicsWorld::getInstance()->registerRigidBody(primaryRigidBody);
}

Math::Point PhysicalPlayer::getOrigin() const {
    btTransform trans;
    primaryRigidBody->getMotionState()->getWorldTransform(trans);
    
    return Converter::toPoint(trans.getOrigin());
}

void PhysicalPlayer::applyMovement(const Math::Point &movement) {
    primaryRigidBody->activate();
    
    primaryRigidBody->applyCentralForce(Converter::toVector(
        movement * 5000.0f));
}

}  // namespace Physics
}  // namespace Project
