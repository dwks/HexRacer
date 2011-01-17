#include "PhysicalPlayer.h"
#include "Converter.h"
#include "PhysicsWorld.h"
#include "render/MeshLoader.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

PhysicalPlayer::PhysicalPlayer(const Math::Point &origin)
    : primaryRigidBody(NULL) {
    
    constructRigidBody(origin);
}

PhysicalPlayer::~PhysicalPlayer() {
    PhysicsWorld::getInstance()->destroyRigidBody(primaryRigidBody);
    
    delete primaryRigidBody;
}

void PhysicalPlayer::constructRigidBody(const Math::Point &origin) {
    delete primaryRigidBody;  // works even if NULL
    
	Math::BoundingBox3D box(1.0, 1.0, 1.0);
	//Get the bounding box from the mesh loader
	Render::MeshGroup* player_model = Render::MeshLoader::getInstance()->getModelByName("playerCube");
	if (player_model)
		box = player_model->getBoundingBox();

	primaryRigidBody = PhysicsWorld::getInstance()->createRigidBox(
		box.widthX(), box.widthY(), box.widthZ(), origin, 2.0);
	
    /*primaryRigidBody = PhysicsWorld::getInstance()->createRigidBox(
        1.0, 1.0, 1.0, origin, 2.0);*/
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
