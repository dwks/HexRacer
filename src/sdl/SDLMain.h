#ifndef PROJECT_SDL__SDLMAIN_H
#define PROJECT_SDL__SDLMAIN_H

#include "opengl/Trackball.h"
#include "opengl/SimpleTrackball.h"
#include "opengl/Camera.h"
#include "sdl/CameraObject.h"
#include "Projector.h"
#include "NetworkPortal.h"
#include "PlayerManager.h"
#include "InputManager.h"
#include "ClientData.h"
#include "physics/PhysicsWorld.h"
#include "render/RenderManager.h"
#include "render/Shader.h"
#include "render/MeshLoader.h"
#include "render/RenderList.h"
#include "render/BackgroundRenderable.h"

#include "physics/Suspension.h"

#include "object/WorldManager.h"

#include "paint/PaintManager.h"
#include "paint/PaintGenerator.h"
#include "paint/PaintSubsystem.h"

#include "event/ObserverList.h"
#include "event/TypedObserver.h"
#include "event/CameraMovement.h"
#include "event/QuitEvent.h"

#define FIELD_OF_VIEW 60
#define SDL_INIT_FLAGS (SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE)

union SDL_Event;  // forward declaration

namespace Project {
namespace SDL {

class SDLMain {
private:
    //OpenGL::Trackball *trackball;
	OpenGL::SimpleTrackball *simpleTrackball;
	OpenGL::Camera *camera;
    SDL::CameraObject *cameraObject;
    
    bool quit;
    
    Projector projector;
    InputManager *inputManager;
    NetworkPortal *network;
    PlayerManager *playerManager;
    Object::WorldManager *worldManager;
    Physics::PhysicsWorld *physicsWorld;
    Render::MeshGroup *test_terrain;
    ClientData *clientData;
    Physics::Suspension *suspension;
    
	Render::MeshLoader *meshLoader;
	Render::RenderManager *renderer;
	Render::RenderList* rootRenderable;
	Render::LightManager *lightManager;

	int testPaintColor;

	Paint::PaintManager *paintManager;
	vector<Paint::PaintCell*> paintCells;
    Paint::PaintSubsystem *paintSubsystem;
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
    
    class QuitObserver : public Event::TypedObserver<Event::QuitEvent> {
    private:
        SDLMain *sdlMain;
    public:
        QuitObserver(SDLMain *sdlMain) : sdlMain(sdlMain) {}
        
        virtual void observe(Event::QuitEvent *event);
    };
public:
    SDLMain();
    ~SDLMain();
    
    void run();
    
    void doQuit() { quit = true; }
private:
    void resizeGL(int width, int height);
    void render();
    void renderGrid();
    
    void handleEvents();
	void updateCamera();
private:
    void initSDL();
    void initOpenGL();
    void initRenderer();
};

}  // namespace SDL
}  // namespace Project

#endif
