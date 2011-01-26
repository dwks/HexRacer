#include "Player.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "settings/ProgramSettings.h"

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
    
    initialize();
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

void Player::initialize() {
    if(Settings::ProgramSettings::getInstance()->isClient()) {
        renderable = new Render::RenderablePlayer();
        renderable->initialize(getID());
    }
}

}  // namespace Object
}  // namespace Project
