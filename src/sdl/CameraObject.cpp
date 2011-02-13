#include "CameraObject.h"
#include "event/ObserverList.h"

#include "physics/PhysicsWorld.h"

namespace Project {
namespace SDL {
    
    void CameraObject::PhysicsTickObserver::observe(Event::PhysicsTick *event) {
        cameraObject->doAction(event->getElapsed() * 1000.0);
    }
    
    void CameraObject::SetDebugCameraHandler::observe(
        Event::SetDebugCamera *event) {
        
        cameraObject->setDebugCamera(event->getOn());
    }
    
    CameraObject::CameraObject(){
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        camera = new OpenGL::Camera();
        debugCamera = false;
        
        loadSettings();
        
        ADD_OBSERVER(new PhysicsTickObserver(this));
        ADD_OBSERVER(new SetDebugCameraHandler(this));
    }
    
    CameraObject::CameraObject(SDL::PlayerManager *_playerManager){
        LOG2(CAMERA, INIT, "Camera Object has been initialized\n");
        
        playerManager = _playerManager;
        camera = new OpenGL::Camera();
        
        loadSettings();
        
        ADD_OBSERVER(new PhysicsTickObserver(this));
        ADD_OBSERVER(new SetDebugCameraHandler(this));
    }
    
    CameraObject::~CameraObject(){
        delete camera;
    }

    void CameraObject::loadSettings(){
        defaultFOV = GET_SETTING("render.camera.fieldofview", 0.0);
        slowFOV = GET_SETTING("render.camera.fovslow", 0.0);
        boostFOV = GET_SETTING("render.camera.fovboost", 0.0);
        
        defaultOrientation.setX(GET_SETTING("camera.followdefault.x", 0.0));
        defaultOrientation.setY(GET_SETTING("camera.followdefault.y", 0.0));
        defaultOrientation.setZ(GET_SETTING("camera.followdefault.z", 0.0));
        
        boostOrientation.setX(GET_SETTING("camera.followboost.x", 0.0));
        boostOrientation.setY(GET_SETTING("camera.followboost.y", 0.0));
        boostOrientation.setZ(GET_SETTING("camera.followboost.z", 0.0));
        
        slowOrientation.setX(GET_SETTING("camera.followslow.x", 0.0));
        slowOrientation.setY(GET_SETTING("camera.followslow.y", 0.0));
        slowOrientation.setZ(GET_SETTING("camera.followslow.z", 0.0));
        
        interpolationInc = GET_SETTING("camera.step", 0.0);
        rotationInc = GET_SETTING("camera.rotationstep", 0.0);
        fovInc = GET_SETTING("camera.fovstep", 0.0);
    }
    
    void CameraObject::setDestinationToPlayer(){
        Math::Point lookAtPosition  = playerManager->getPlayer()->getPosition();
        Math::Point lookAtOffset = Math::Point(0.0,1.0,0.0);
        Math::Point cameraPosition, desiredOrientation;
        double intendedLength, length, desiredFOV;
        double zOffset = 1.0;
        
        lookAtPosition += lookAtOffset;
        
         if(playerManager->getPlayer()->getSpeedBoost()>1.1){
            desiredFOV = boostFOV;
            desiredOrientation = boostOrientation;
        } else if(playerManager->getPlayer()->getSpeedBoost()<0.9){
            desiredFOV = slowFOV;
            desiredOrientation = slowOrientation;
        } else {
            desiredFOV = defaultFOV;
            desiredOrientation = defaultOrientation;
        }
        
        do {
            cameraPosition = playerManager->getPlayer()->getTransformation()
            * Math::Point(desiredOrientation.getX(), 
                desiredOrientation.getY(),
                desiredOrientation.getZ()*zOffset
                         );
        
            length = Physics::PhysicsWorld::getInstance()
                ->raycastLength(cameraPosition, lookAtPosition);

                zOffset -= interpolationInc;
                intendedLength = cameraPosition.distance(lookAtPosition);
                
        } while(length < intendedLength && zOffset >= 0.0);
        
        CameraObject::setCameraDestination(lookAtPosition, cameraPosition, desiredFOV);
    }
    
    void CameraObject::setCameraDestination(Math::Point _look, Math::Point _pos, double destFOV){
        destinationLookAt = _look;
        destinationPosition = _pos;
        destinationFOV = destFOV;
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
    void CameraObject::setCameraFOV(double destFOV){
        camera->setFieldOfViewDegrees(destFOV);
    }
    void CameraObject::slerpCamera(){
        Math::Point currentLookAt,currentPos,currentViewAxis,destViewAxis,resultPos,resultAxis;
        double resultFOV;
        btQuaternion result;
        
        currentPos = camera->getPosition();
        
        resultPos = currentPos + interpolationInc*(destinationPosition - currentPos);
        
        resultFOV = camera->getFieldOfViewDegrees() + fovInc*(destinationFOV - camera->getFieldOfViewDegrees());
        
        resultAxis = Physics::Converter::toPoint(result.getAxis());
        
        setCamera(destinationLookAt, resultPos);
        setCameraFOV(resultFOV);
    }
    
    void CameraObject::setDebugCamera(bool debug) {
        this->debugCamera = debug;
    }
}  // namespace object
}  // namespace Project
