#include "CameraObject.h"
#include "event/ObserverList.h"

namespace Project {
namespace SDL {
    
    void CameraObject::SetDebugCameraHandler::observe(
        Event::SetDebugCamera *event) {
        
        cameraObject->setDebugCamera(event->getOn());
    }
    
    CameraObject::CameraObject() : Timing::TimedSubsystem(5){
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        camera = new OpenGL::Camera();
        
        ADD_OBSERVER(new SetDebugCameraHandler(this));
    }
    
    CameraObject::CameraObject(SDL::PlayerManager *_playerManager) : Timing::TimedSubsystem(5) {
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        playerManager = _playerManager;
        camera = new OpenGL::Camera();
    }
    
    CameraObject::~CameraObject(){
        delete camera;
    }

    void CameraObject::setDestinationToPlayer(){
        Math::Point cameraPosition = playerManager->getPlayer()->getTransformation()
            * Math::Point(0.0, 3.0, -10.0);
        
        Math::Point lookAtPosition  = playerManager->getPlayer()->getPosition();
        
        CameraObject::setCameraDestination(lookAtPosition, cameraPosition);
    }
    
    void CameraObject::setCameraDestination(Math::Point _look, Math::Point _pos){
        destinationLookAt = _look;
        destinationPosition = _pos;
    }
    
    void CameraObject::setPlayerManager(SDL::PlayerManager *_playerManager) {
        playerManager = _playerManager;
    }
    
    void CameraObject::doAction(unsigned long currentTime) {
        //LOG2(CAMERA, UPDATE, "CameraObject doAction Called\n");
        setDestinationToPlayer();
        
        //Temp, should be calling slerpCamera when slerp is fixed
        
        if(!debugCamera) {
            setCamera(destinationLookAt, destinationPosition);
        }
        
        //slerpCamera();
    }
    
    void CameraObject::setCamera(Math::Point _look, Math::Point _pos){
        camera->setLookPosition(_look);
        camera->setPosition(_pos);
    }
    
    void CameraObject::slerpCamera(){
        //What I am doing wrong is that I need to have an initial orientation,
        //Then using the quaternions I apply that rotation to the original view
        //And translate it to the position I need... I believe.
        
        float stepVal = 0.01;
        Math::Point currentLookAt,currentPos,currentViewAxis,destViewAxis,resultPos,resultAxis;
        btQuaternion result;
        
        currentLookAt = camera->getLookPosition();
        currentPos = camera->getPosition();
        
        currentViewAxis = currentLookAt - currentPos;
        destViewAxis = destinationLookAt - destinationPosition;
        
        result = btQuaternion(Physics::Converter::toVector(currentViewAxis),0).slerp(
            btQuaternion(Physics::Converter::toVector(destViewAxis),0),stepVal);
        
        resultPos = (1.0)*destinationPosition - (stepVal)*currentPos;
        
        resultAxis = Physics::Converter::toPoint(result.getAxis());
        
        setCamera(resultPos + resultAxis, resultPos);
    }
    
    void CameraObject::setDebugCamera(bool debug) {
        this->debugCamera = debug;
    }
}  // namespace object
}  // namespace Project
