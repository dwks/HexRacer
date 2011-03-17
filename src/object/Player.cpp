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
	delete tracker;
}

Physics::PhysicalPlayer *Player::getPhysicalObject() {
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
	OpenGL::Color trim_color = Render::ColorConstants::playerColor(getTeamID())*glow_scale;

    //LOG(OPENGL, "Player id: " << getID() << ", player is " << trim_color);
    
	renderable->setGlowColor(trim_color);

}

bool Player::operator < (const Player& other) const {
	return (*tracker < *other.getPathTracker());
}

std::string Player::getPlayerName() const {
	switch (getID()) {
		case 0: return "Jake Gorb"; break;
		case 1: return "Chufmoney"; break;
		case 2: return "Phoenix"; break;
		case 3: return "Edgeworth"; break;
		case 4: return "Toblerone"; break;
		case 5: return "Homeslice"; break;
		case 6: return "St. Juib"; break;
		case 7: return "Rasputin"; break;
		case 8: return "The Sauce"; break;
		case 9: return "Rainbow-BRITE"; break;
		case 10: return "Pogs"; break;
		case 11: return "Creamsicle"; break;
		case 12: return "Fresh Prance"; break;
		case 13: return "Carabeener"; break;
		case 14: return "Three Sheets"; break;
		case 15: return "Speak-N-Spell"; break;
		default: return "..."; break;
	}
}

}  // namespace Object
}  // namespace Project
