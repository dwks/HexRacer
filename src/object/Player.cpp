#include "Player.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "shader/ShaderParamVector4.h"
#include "render/ColorConstants.h"

namespace Project {
namespace Object {

Player::Player() : AbstractObject(-1) {
    physical = NULL;
    renderable = NULL;
	tracker = NULL;
}

Player::Player(int id, const Math::Point &origin, const Math::Point &direction) : AbstractObject(id) {
    Physics::PhysicsWorld *world = Physics::PhysicsWorld::getInstance();
    
    if(world) {
		physical = Physics::PhysicsFactory::createPhysicalPlayer(origin, direction);
    }
    else {
        physical = NULL;
    }

	tracker = NULL;
    
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
    renderable = NULL;
}

void Player::preRender() {
    if(!renderable && Settings::ProgramSettings::getInstance()->isClient()) {
        renderable = new Render::RenderablePlayer();
        renderable->initialize(getID());
    }
    
    AbstractObject::preRender();
    
    for(int wheel = 0; wheel < 4; wheel ++) {
        renderable->setSuspension(wheel, getSuspension(wheel));
    }
    
    renderable->setWheelRotation(
        physical->getLinearVelocity().dotProduct(
            physical->getFrontDirection()));

	renderable->updatePhysicalData(physical->getOrigin());

	float glow_scale = (float) Math::maximum(
		GET_SETTING("render.playerglow.min", 0.6),
		getSpeedBoost()*GET_SETTING("render.playerglow.factor", 1.25f)+GET_SETTING("render.playerglow.constant", 0.0f));
	OpenGL::Color trim_color = Render::ColorConstants::playerColor(getID())*glow_scale;

    //LOG(OPENGL, "Player id: " << getID() << ", player is " << trim_color);
    
	renderable->setGlowColor(trim_color);

}

}  // namespace Object
}  // namespace Project
