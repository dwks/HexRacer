#include <cstdlib>

#include "Player.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "shader/ShaderParamVector4.h"
#include "map/Teams.h"

namespace Project {
namespace Object {

Player::Player() : AbstractObject(-1), teamID(-1), raceFinishIgnore(false) {
    physical = NULL;
    renderable = NULL;
	tracker = NULL;
}

Player::Player(int id, const Math::Point &origin, const Math::Point &direction)
    : AbstractObject(id), teamID(-1), raceFinishIgnore(false) {
    
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
    
    // deal with exponential backoff interpolation due to network error
    Math::Point networkError = physical->getNetworkError();
    if(networkError.length() > 5.0) {
        networkError = Math::Point();
        physical->setNetworkError(networkError);
        
        renderable->preRenderUpdate(physical->getTransformation());
    }
    else {
        //LOG(NETWORK, "error: " << networkError.length() << "\t" << networkError);
        
        networkError *= 0.85;
        physical->setNetworkError(networkError);
        
        Math::Matrix transform = physical->getTransformation();
        transform = Math::Matrix::getTranslationMatrix(networkError) * transform;
        
        renderable->preRenderUpdate(transform);
    }
    
    // other properties for rendering
    for(int wheel = 0; wheel < 4; wheel ++) {
        renderable->setSuspension(wheel, getSuspension(wheel));
    }
    
    renderable->setWheelRotation(
        physical->getLinearVelocity().dotProduct(
            physical->getFrontDirection()));
    
	renderable->updatePhysicalData(physical->getOrigin() + networkError);

	float glow_scale = (float) Math::maximum(
		GET_SETTING("render.playerglow.min", 0.6),
		getSpeedBoost()*GET_SETTING("render.playerglow.factor", 1.25f)+GET_SETTING("render.playerglow.constant", 0.0f));
	OpenGL::Color trim_color = Map::Teams::teamColor(getTeamID())*glow_scale;

    //LOG(OPENGL, "Player id: " << getID() << ", player is " << trim_color);
    
	renderable->setGlowColor(trim_color);

}

bool Player::operator < (const Player& other) const {
	return (*tracker < *other.getPathTracker());
}

std::string Player::getDefaultPlayerName(int id) {
    const char *nameList[] = {
		"Jake Gorb",
		"Chufmoney",
		"Phoenix",
		"Edgeworth",
		"Toblerone",
		"Homeslice",
		"St. Juib",
		"Rasputin",
		"The Sauce",
		"Rainbow-BRITE",
		"Pogs",
		"Creamsicle",
		"Fresh Prance",
		"Carabeener",
		"Three Sheets",
		"Speak-N-Spell"
	};
    std::size_t size = sizeof(nameList) / sizeof(*nameList);
    
    return nameList[std::rand() % size];
}

}  // namespace Object
}  // namespace Project
