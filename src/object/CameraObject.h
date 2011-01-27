#ifndef PROJECT_OBJECT__CAMERA_OBJECT_H
#define PROJECT_OBJECT__CAMERA_OBJECT_H

#include "log/Logger.h"
#include "opengl/Camera.h"
#include "math/Point.h"
#include "sdl/PlayerManager.h"
#include "object/Player.h"
#include "timing/TimedSubsystem.h"

namespace Project {
namespace Object {

class CameraObject : public Timing::TimedSubsystem {
public:
    CameraObject();
    CameraObject(SDL::PlayerManager *_playerManager);
    ~CameraObject();
    
    void setDestination(Math::Point _dest);
    void setPlayerManager(SDL::PlayerManager *_playerManager);
    Math::Point getCurrent();
    void updateCamera();
    void doAction(unsigned long currentTime);
    
    OpenGL::Camera *camera;
    OpenGL::Camera *playerCamera;
    
private:
    SDL::PlayerManager *playerManager;
    Math::Point destination;

};

}  // namespace object
}  // namespace Project

#endif
