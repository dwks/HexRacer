#ifndef PROJECT_SDL__SDLMAIN_H
#define PROJECT_SDL__SDLMAIN_H

#include "opengl/Trackball.h"
#include "opengl/SimpleTrackball.h"
#include "opengl/Camera.h"
#include "Projector.h"
#include "JoystickManager.h"
#include "NetworkPortal.h"
#include "PlayerManager.h"
#include "InputManager.h"
#include "physics/PhysicsWorld.h"
#include "render/RenderManager.h"
#include "render/Shader.h"
#include "render/MeshLoader.h"
#include "render/RenderList.h"

#include "paint/PaintGenerator.h"

#include "event/ObserverList.h"
#include "event/TypedObserver.h"
#include "event/CameraMovement.h"

#define FIELD_OF_VIEW 60
#define SDL_INIT_FLAGS (SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE)

union SDL_Event;  // forward declaration

namespace Project {
namespace SDL {

class SDLMain {
private:
    //OpenGL::Trackball *trackball;
	OpenGL::SimpleTrackball *simpleTrackball;
	OpenGL::Camera  *camera;
    
    Projector projector;
    JoystickManager *joystick;
    InputManager *inputManager;
    NetworkPortal *network;
    PlayerManager *playerManager;
    Physics::PhysicsWorld *physicsWorld;
    
	Render::MeshLoader *meshLoader;
	Render::RenderManager *renderer;
	Render::RenderList* rootRenderable;
	Render::LightManager *lightManager;

	vector<Paint::PaintCell*> paintCells;
    
	GLUquadric* quadric;
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

public:
    SDLMain();
    ~SDLMain();
    
    void run();
private:
    void resizeGL(int width, int height);
    void handleJoystick();
    void render();

	void updateCamera();

};

}  // namespace SDL
}  // namespace Project

#endif
