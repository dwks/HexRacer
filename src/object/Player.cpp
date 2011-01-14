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
        //physical = world->createPlayer(id);
        physical = NULL;
    }
    else {
        physical = NULL;
    }
    
    renderable = NULL; //new Render::RenderablePlayer();
}

}  // namespace Object
}  // namespace Project
