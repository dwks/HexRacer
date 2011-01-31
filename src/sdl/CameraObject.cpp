#include "CameraObject.h"
#include "event/ObserverList.h"


namespace Project {
namespace SDL {
    
    void CameraObject::SetDebugCameraHandler::observe(
        Event::SetDebugCamera *event) {
        
        cameraObject->setDebugCamera(event->getOn());
    }
    
    CameraObject::CameraObject() : Timing::TimedSubsystem(10){
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        camera = new OpenGL::Camera();
        
        loadSettings();
        
        ADD_OBSERVER(new SetDebugCameraHandler(this));
    }
    
    CameraObject::CameraObject(SDL::PlayerManager *_playerManager) : Timing::TimedSubsystem(10) {
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        playerManager = _playerManager;
        camera = new OpenGL::Camera();
        
        loadSettings();
        
        ADD_OBSERVER(new SetDebugCameraHandler(this));
    }
    
    CameraObject::~CameraObject(){
        delete camera;
    }

    void CameraObject::loadSettings(){
        defaultOrientation.setX(GET_SETTING("camera.follow.x", 0.0));
        defaultOrientation.setY(GET_SETTING("camera.follow.y", 0.0));
        defaultOrientation.setZ(GET_SETTING("camera.follow.z", 0.0));
        interpolationInc = GET_SETTING("camera.step", 0.0);
    }
    
    void CameraObject::setDestinationToPlayer(){
        Math::Point cameraPosition = playerManager->getPlayer()->getTransformation()
            * Math::Point(defaultOrientation.getX(), 
                defaultOrientation.getY(),
                defaultOrientation.getZ()
                         );
        
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
        if(!debugCamera) {
            setDestinationToPlayer();
            slerpCamera();
        }
    }
    
    void CameraObject::setCamera(Math::Point _look, Math::Point _pos){
        camera->setLookPosition(_look);
        camera->setPosition(_pos);
    }
    
    void CameraObject::slerpCamera(){
        Math::Point currentLookAt,currentPos,currentViewAxis,destViewAxis,resultPos,resultAxis;
        btQuaternion result;
        
        currentLookAt = camera->getLookPosition();
        currentPos = camera->getPosition();
        
        currentViewAxis = currentLookAt - currentPos;
        destViewAxis = destinationLookAt - destinationPosition;
        
        result = btQuaternion(Physics::Converter::toVector(destViewAxis),1).slerp(
            btQuaternion(Physics::Converter::toVector(currentViewAxis),1),interpolationInc);
        
        resultPos = currentPos + interpolationInc*(destinationPosition - currentPos);
        
        resultAxis = Physics::Converter::toPoint(result.getAxis());
        
        setCamera(resultPos + resultAxis, resultPos);
    }
    
    void CameraObject::setDebugCamera(bool debug) {
        this->debugCamera = debug;
    }
}  // namespace object
}  // namespace Project
