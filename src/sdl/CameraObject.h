#ifndef PROJECT_OBJECT__CAMERA_OBJECT_H
#define PROJECT_OBJECT__CAMERA_OBJECT_H

#include "btBulletDynamicsCommon.h"
#include "log/Logger.h"
#include "opengl/Camera.h"
#include "math/Point.h"
#include "physics/Converter.h"
#include "sdl/PlayerManager.h"
#include "object/Player.h"
#include "timing/TimedSubsystem.h"
#include "event/SetDebugCamera.h"
#include "event/TypedObserver.h"

namespace Project {
namespace SDL {

class CameraObject : public Timing::TimedSubsystem {
private:
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
    void slerpCamera();
    void setCamera(Math::Point _look, Math::Point _pos);
    void setDestinationToPlayer();
    void setCameraDestination(Math::Point _look, Math::Point _pos);
    Math::Point getCurrent();
    
    void setDebugCamera(bool debug);
    
    bool debugMode;
    SDL::PlayerManager *playerManager;
    Math::Point destinationLookAt;
    Math::Point destinationPosition;
    
};

}  // namespace object
}  // namespace Project

#endif
