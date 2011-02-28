#ifndef PROJECT_SDL__GAME_VIEWPORT_H
#define PROJECT_SDL__GAME_VIEWPORT_H

#include "SDL_events.h"

#include "Point2D.h"

#include "event/CameraMovement.h"
#include "event/TypedObserver.h"
#include "event/Enabler.h"

#include "opengl/SimpleTrackball.h"
#include "opengl/Camera.h"

#include "CameraObject.h"

namespace Project {
namespace SDL {

class GameViewport : public Event::Enabler {
private:
    class CameraObserver : public Event::TypedObserver<Event::CameraMovement> {
    private:
        OpenGL::SimpleTrackball *trackball;
        OpenGL::Camera *camera;
    public:
        CameraObserver(OpenGL::SimpleTrackball *trackball, 
            OpenGL::Camera *camera) : trackball(trackball), camera(camera) {}
        
        virtual void observe(Event::CameraMovement *event);
    };
private:
    OpenGL::SimpleTrackball *trackball;
    SDL::CameraObject *cameraObject;
public:
    GameViewport();
    ~GameViewport();
    
    void setPlayerManager(World::PlayerManager *playerManager);
    
    OpenGL::Camera *getCamera() { return cameraObject->camera; }
    
    virtual void setProjection(const Point2D &size);
    
    void checkForDebugCameraEvents(SDL_Event *event);
    void doCamera(unsigned long milliseconds);
private:
    void updateCamera();
};

}  // namespace SDL
}  // namespace Project

#endif
