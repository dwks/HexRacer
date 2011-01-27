#include "CameraObject.h"

namespace Project {
namespace Object {
    CameraObject::CameraObject() : Timing::TimedSubsystem(5){
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        camera = new OpenGL::Camera();
    }
    
    CameraObject::CameraObject(SDL::PlayerManager *_playerManager) : Timing::TimedSubsystem(5) {
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        playerManager = _playerManager;
        camera = new OpenGL::Camera();
    }
    
    CameraObject::~CameraObject(){
        delete camera;
    }
    
    void CameraObject::setDestination(Math::Point _dest){
        destination = _dest;
    }
    
    void CameraObject::setPlayerManager(SDL::PlayerManager *_playerManager) {
        playerManager = _playerManager;
    }
    
    void CameraObject::doAction(unsigned long currentTime) {
        LOG2(CAMERA, UPDATE, "CameraObject doAction Called\n");
        updateCamera();
    }
    
    //This is a temp implementation of this fucntion
    void CameraObject::updateCamera(){
        //Modify for interpolation and to use quaternions and vectors instead of just points
        CameraObject::setDestination(playerManager->getPlayer()->getPosition());
        
        camera->setLookPosition(this->destination);
    }
    
    
}  // namespace object
}  // namespace Project
