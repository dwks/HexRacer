#include "Player.h"
#include "physics/PhysicsWorld.h"
#include "render/RenderablePlayer.h"

namespace Project {
namespace Object {

Player::Player() : AbstractObject(-1) {
    physical = NULL;
    renderable = NULL;
}

Player::Player(int id) : AbstractObject(id) {
    Physics::PhysicsWorld *world = Physics::PhysicsWorld::getInstance();
    
    if(world) {
        physical = world->createPlayer(id);
    }
    else {
        physical = NULL;
    }
    
    renderable = NULL; //new Render::RenderablePlayer();
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

void Player::applyMovement(const Math::Point &movement) {
    physical->applyMovement(movement);
}

}  // namespace Object
}  // namespace Project
