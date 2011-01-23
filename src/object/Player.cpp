#include "Player.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

namespace Project {
namespace Object {

Player::Player() : AbstractObject(-1) {
    physical = NULL;
    renderable = NULL;
}

Player::Player(int id, const Math::Point &origin) : AbstractObject(id) {
    Physics::PhysicsWorld *world = Physics::PhysicsWorld::getInstance();
    
    if(world) {
		physical = Physics::PhysicsFactory::createPhysicalPlayer(origin);
    }
    else {
        physical = NULL;
    }
    
    /*renderable = new Render::RenderList(); //new Render::RenderablePlayer();
    
    Render::MeshGroup* player_cube_mesh
        = Render::MeshLoader::getInstance()->getModelByName("playerCube");
    renderable->addRenderable(player_cube_mesh);
    
    renderable->getRenderProperties()->addShaderParameter(
        new Render::ShaderUniformVector4("playerColor",
            OpenGL::Color(OpenGL::Color::BLUE)));*/
    
    renderable = NULL;
}

Player::~Player() {
    delete physical;
    delete renderable;
}

Physics::PhysicalObject *Player::getPhysicalObject() {
    return physical;
}

Render::RenderableObject *Player::getRenderableObject() {
    /*if(renderable && physical) {
        dynamic_cast<Render::RenderablePlayer *>(renderable)
            ->updatePhysicalData(physical->getOrigin());
    }*/
    
    return renderable;
}

Math::Point Player::getPosition() const {
    return physical->getOrigin();
}

Math::Matrix Player::getTransformation() const {
    return physical->getTransformation();
}

void Player::applyForce(const Math::Point &movement, const Math::Point &at) {
    physical->applyForce(movement, at);
}

}  // namespace Object
}  // namespace Project
