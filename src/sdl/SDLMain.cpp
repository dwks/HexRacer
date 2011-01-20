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

#include "render/ShaderUniformVector4.h"

#include "SDL_image.h"

#include "config.h"

namespace Project {
namespace SDL {

void SDLMain::CameraObserver::observe(Event::CameraMovement *event) {

    double x = event->getMovement().getX();
    double y = event->getMovement().getY();
    
    Math::Point translation = camera->getLookDirection()*(y * 0.25f)
        + camera->getRightDirection() * (x * 0.25f);
    
    camera->translate(translation);
    
    //LOG(SDL, "Move camera by " << translation);
}

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
    //double aspect_ratio = static_cast<double>(width) / height;
    //gluPerspective(FIELD_OF_VIEW, aspect_ratio, 0.01, 100.0);
	camera->setAspect(static_cast<double>(width)/static_cast<double>(height));
	camera->glProjection();
    
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

	//trackball = new OpenGL::Trackball();
	simpleTrackball = new OpenGL::SimpleTrackball();

	//Initialize the camera
	camera = new OpenGL::Camera();
	camera->setFieldOfViewDegrees(60.0f);
	camera->setPosition(Point(0.0f, 2.0f, -4.0f));
	camera->setFarPlane(80.0f);
	updateCamera();

    resizeGL(WIDTH, HEIGHT);
    
    joystick = new JoystickManager();
    joystick->open();

	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_RESCALE_NORMAL);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	quadric = gluNewQuadric();

	//Instantiate the rendering objects
	meshLoader = new Render::MeshLoader();
	renderer = new Render::RenderManager("renderconfig.txt");
	lightManager = renderer->getLightManager();
	rootRenderable = new Render::RenderList();

	renderer->loadShadersFile("shaders.txt");

	//Load the model
	meshLoader->loadOBJ("testTerrain", "models/testterrain.obj");
	meshLoader->loadOBJ("playerCube", "models/playercube.obj");
	//Add the test terrain
	Render::MeshGroup* test_terrain = meshLoader->getModelByName("testTerrain");

	//Paint::PaintGenerator paint_gen(test_terrain->getTriangles());
	//paintCells = paint_gen.getPaintCells();

	test_terrain->setCullingObject(camera->getFrustrum());
	test_terrain->setCullingQueryType(Math::SpatialContainer::NEARBY);
	rootRenderable->addRenderable(test_terrain);

	/*
	rootRenderable->getRenderProperties()->setColor(OpenGL::Color::VIOLET);
	rootRenderable->getRenderProperties()->setColorOverride(true);
	rootRenderable->getRenderProperties()->setShaderOverride(true);
	rootRenderable->getRenderProperties()->setTextureOverride(true);
	*/
    
	//Get the Player Cube Mesh
	//Render::MeshGroup* player_cube_mesh = meshLoader->getModelByName("playerCube");
    
	//Create some lights
	Render::Light* light = new Render::Light(Math::Point(1.0f, 2.0f, -1.0f));
	light->setStrength(20.0f);
	lightManager->addLight(light);

	light = new Render::Light(Math::Point(7.0f, 2.5f, 3.0f));
	light->setDiffuse(OpenGL::Color::INDIGO);
	light->setSpecular(OpenGL::Color::INDIGO);
	light->setStrength(10.0f);
	lightManager->addLight(light);

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
    
    ADD_OBSERVER(new CameraObserver(simpleTrackball, camera));
    
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
                /*trackball->setMouseStartAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));*/
				simpleTrackball->setMouseStartAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));
                break;
            case SDL_MOUSEMOTION:
                if(event.motion.state & SDL_BUTTON(1)) {
                    //LOG2(SDL, INPUT, "Mouse moved to " << event.motion.x << "," << event.motion.y);
                    /*trackball->setMouseCurrentAt(projector.screenToGL(
                        Point2D(event.motion.x, event.motion.y)));*/
					simpleTrackball->setMouseCurrentAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));
					updateCamera();
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
        
        {
            static Uint32 lastPhysicsTime = SDL_GetTicks();
            Uint32 thisTime = SDL_GetTicks();
            physicsWorld->stepWorld((thisTime - lastPhysicsTime) * 1000);
            lastPhysicsTime = thisTime;
        }
        
        render();
        physicsWorld->render();
        
        SDL_GL_SwapBuffers();
        
        {
            Uint32 thisTime = SDL_GetTicks();
            int timeTakenSoFar = static_cast<int>(thisTime - lastTime);
            if(timeTakenSoFar < 10) {
                SDL_Delay(10 - timeTakenSoFar);
            }
            while(lastTime < thisTime) lastTime += 10;
        }
    }
    
    LOG2(GLOBAL, PROGRESS, "Exiting main game loop");
    
    delete joystick;
    delete inputManager;
    delete network;
}

void SDLMain::handleJoystick() {
    double x = joystick->getNormalizedAxisValue(0);
    double y = joystick->getNormalizedAxisValue(1);
    
	double u = joystick->getNormalizedAxisValue(4, 0.0);
    double v = joystick->getNormalizedAxisValue(3, 0.0);
    
    const double DEADZONE = 0.2;
    
    if(std::fabs(x) > DEADZONE || std::fabs(y) > DEADZONE) {
        //LOG(SDL, "Move joystick by " << x << "," << y);
        //trackball->setMouseStartAt(Math::Point(0.0, 0.0));
        
		//Camera movement
		Math::Point translation = camera->getLookDirection()*(-y * 0.25f)
			+ camera->getRightDirection() * (x * 0.25f);
        
		camera->translate(translation);
        
        //trackball->setMouseCurrentAt(translation);
    }
    
	if(std::fabs(u) > DEADZONE || std::fabs(v) > DEADZONE) {
		simpleTrackball->setMouseStartAt(Math::Point(0.0, 0.0));

		//Look around with the camera
        Math::Point translation;
        translation.setX(u * 0.1);
        translation.setY(-v * 0.1);

		//LOG(SDL, "Joystick moving camera by " << translation);
        
        simpleTrackball->setMouseCurrentAt(translation);
        updateCamera();
    }
}

void SDLMain::updateCamera() {
	camera->setLookDirection(simpleTrackball->getSpherePoint());
}

void SDLMain::render() {
    glMatrixMode(GL_MODELVIEW);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
	OpenGL::Color::glColor(OpenGL::Color::WHITE);
    
	camera->glLookAt();
    
	lightManager->drawLightSpheres();
	lightManager->applyAll();
    
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	 // Render players
	playerManager->render(renderer);
	
	//Render the scene
	rootRenderable->render(renderer);

	glPopMatrix();
    
	//Revert the rendering state
	//glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//Draw paint cells
	glColor3f(1.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < paintCells.size(); i++) {
		glBegin(GL_TRIANGLE_FAN);
		OpenGL::MathWrapper::glVertex(paintCells[i]->center);
		for (int j = 0; j < Paint::PaintCell::CELL_VERTICES; j++) {
			OpenGL::MathWrapper::glVertex(paintCells[i]->vertex[j]);
		}
		OpenGL::MathWrapper::glVertex(paintCells[i]->vertex[0]);
		glEnd();
	}
	
    
    glFlush();
}

}  // namespace SDL
}  // namespace Project
