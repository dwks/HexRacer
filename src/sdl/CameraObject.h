#ifndef PROJECT_OBJECT__CAMERA_OBJECT_H
#define PROJECT_OBJECT__CAMERA_OBJECT_H

#include "btBulletDynamicsCommon.h"
#include "log/Logger.h"
#include "opengl/Camera.h"
#include "math/Point.h"
#include "math/Matrix.h"
#include "physics/Converter.h"
#include "sdl/PlayerManager.h"
#include "object/Player.h"

#include "event/PhysicsTick.h"
#include "event/TypedObserver.h"

#include "event/SetDebugCamera.h"

#include "settings/SettingsManager.h"
#include "settings/ProgramSettings.h"
#include "config.h"

namespace Project {
namespace SDL {

class CameraObject {
private:
    class PhysicsTickObserver
        : public Event::TypedObserver<Event::PhysicsTick> {
    private:
        CameraObject *cameraObject;
    public:
        PhysicsTickObserver(CameraObject *cameraObject) : cameraObject(cameraObject) {}
        
        virtual void observe(Event::PhysicsTick *event);
    };
    
    class SetDebugCameraHandler
        : public Event::TypedObserver<Event::SetDebugCamera> {
    private:
        CameraObject *cameraObject;
    public:
        SetDebugCameraHandler(CameraObject *cameraObject)
            : cameraObject(cameraObject) {}
        
        virtual void observe(Event::SetDebugCamera *event);
    };
public:
    CameraObject();
    CameraObject(SDL::PlayerManager *_playerManager);
    ~CameraObject();
    
    void setPlayerManager(SDL::PlayerManager *_playerManager);
    void doAction(unsigned long currentTime);
    
    bool debugCamera;
    OpenGL::Camera *camera;
    OpenGL::Camera *playerCamera;
    
private:
    void loadSettings();
    void slerpCamera();
    void setCamera(Math::Point _look, Math::Point _pos);
    void setCameraFOV(double destFOV);
    void setDestinationToPlayer();
    void setCameraDestination(Math::Point _look, Math::Point _pos, double destFOV);
    Math::Point getCurrent();
    
    void setDebugCamera(bool debug);
    
    bool debugMode;
    float interpolationInc;
    float rotationInc;
    double fovInc;
    double destinationFOV;
    double boostFOV;
    double defaultFOV;
    double slowFOV;
    SDL::PlayerManager *playerManager;
    Math::Point defaultOrientation;
    Math::Point boostOrientation;
    Math::Point slowOrientation;
    Math::Point destinationLookAt;
    Math::Point destinationPosition;
    
};

}  // namespace object
}  // namespace Project

#endif
