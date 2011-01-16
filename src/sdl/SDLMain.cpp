#include <cctype>
#include <cmath>

#include "SDL.h"
#include "opengl/OpenGL.h"
#include "GL/glu.h"

#include "SDLMain.h"
#include "log/Logger.h"

#include "math/Point.h"
#include "opengl/MathWrapper.h"

#include "event/ObserverList.h"
#include "event/PlayerMovement.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"

#include "SDL_image.h"

#include "config.h"

namespace Project {
namespace SDL {

SDLMain::SDLMain() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        LOG2(SDL, ERROR, "Can't init SDL: " << SDL_GetError());
    }
}

SDLMain::~SDLMain() {
    SDL_Quit();
}

void SDLMain::resizeGL(int width, int height) {
    // in case of divide by zero
    if(height == 0) height = 1;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // set repaint rect to encompass entire window
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // use perspective projection
    double aspect_ratio = static_cast<double>(width) / height;
    gluPerspective(FIELD_OF_VIEW, aspect_ratio, 0.01, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void SDLMain::run() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_WM_SetCaption("The Project", NULL);
    SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_INIT_FLAGS);
    
    projector.setCurrentDimensions(Point2D(WIDTH, HEIGHT));
    resizeGL(WIDTH, HEIGHT);
    
    trackball = new OpenGL::Trackball();
    
    joystick = new JoystickManager();
    //joystick->open();

	//Initialize a light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);

	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_RESCALE_NORMAL);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	meshLoader = new Render::MeshLoader();
	renderer = new Render::RenderManager();

	//Load the test shader
	renderer->loadShader("testShader", "test.frag", "test.vert");
	//Load the model
	meshLoader->loadOBJ("glassdagger", "glassdagger.obj");
	testMesh = meshLoader->getModelByName("glassdagger");
	testMesh->getRenderProperties()->setWantsShaderName("testShader");

    inputManager = new InputManager();
    
    // this must happen before Players are created
    physicsWorld = new Physics::PhysicsWorld();
    physicsWorld->createTestScene();
    
    network = new NetworkPortal();
    if(network->connectTo("localhost", 1820)) {
        network->waitForWorld();
        playerManager = new PlayerManager(network->getID());
    }
    else {
        playerManager = new PlayerManager(0);
    }
    
    LOG2(GLOBAL, PROGRESS, "Entering main game loop");
    
    bool quit = false;
    Uint32 lastTime = SDL_GetTicks();
    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_VIDEORESIZE:
                SDL_SetVideoMode(event.resize.w, event.resize.h,
                    0, SDL_INIT_FLAGS);
                resizeGL(event.resize.w, event.resize.h);
                projector.setCurrentDimensions(
                    Point2D(event.resize.w, event.resize.h));
                break;
            case SDL_MOUSEBUTTONDOWN:
                /*LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " pressed "
                    << "at " << event.button.x << "," << event.button.y);*/
                trackball->setMouseStartAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));
                break;
            case SDL_MOUSEMOTION:
                if(event.motion.state & SDL_BUTTON(1)) {
                    //LOG2(SDL, INPUT, "Mouse moved to " << event.motion.x << "," << event.motion.y);
                    trackball->setMouseCurrentAt(projector.screenToGL(
                        Point2D(event.motion.x, event.motion.y)));
                }
                break;
            case SDL_MOUSEBUTTONUP:
                //LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " released");
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                inputManager->handleEvent(&event);
                break;
            }
        }
        
        network->checkNetwork();
        
        handleJoystick();
        inputManager->advanceToNextFrame();
        
        Uint32 thisTime = SDL_GetTicks();
        physicsWorld->stepWorld((thisTime - lastTime) * 1000);
        lastTime = thisTime;
        
        render();
        playerManager->render();
        physicsWorld->render();
        
        SDL_GL_SwapBuffers();
        
        SDL_Delay(10);
    }
    
    LOG2(GLOBAL, PROGRESS, "Exiting main game loop");
    
    delete trackball;
    delete joystick;
    delete inputManager;
    delete network;
}

void SDLMain::handleJoystick() {
    double x = joystick->getNormalizedAxisValue(0);
    double y = joystick->getNormalizedAxisValue(1);
    
    if(std::fabs(x) > 1e-3 || std::fabs(y) > 1e-3) {
        //LOG(SDL, "Move joystick by " << x << "," << y);
        trackball->setMouseStartAt(Math::Point(0.0, 0.0));
        
        Math::Point translation;
        translation.setX(x * 0.1);
        translation.setY(-y * 0.1);
        
        trackball->setMouseCurrentAt(translation);
    }
}

void SDLMain::render() {
    glMatrixMode(GL_MODELVIEW);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslated(0.0, 0.0, -10.0);
    
    trackball->applyTransform();

	glPushMatrix();
	glScalef(3.0f, 3.0f, 3.0f);

	Math::Point lightp(1.0f, 1.0f, -0.5f);
	GLfloat light_pos[4] = {lightp.getX(), lightp.getY(), lightp.getZ(), 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	//Render the test mesh
	glEnable(GL_TEXTURE_2D);
	testMesh->render(renderer);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
    
    glFlush();
}

}  // namespace SDL
}  // namespace Project
