#include <cctype>
#include <cmath>

#include "SDL.h"
#include "opengl/OpenGL.h"
#include "GL/glu.h"

#include "SDLMain.h"
#include "log/Logger.h"

#include "math/Point.h"
#include "opengl/MathWrapper.h"
#include "render/TextureCube.h"

#include "event/ObserverList.h"

#include "physics/PhysicsFactory.h"
#include "physics/PhysicsWorld.h"

#include "render/ShaderUniformVector4.h"
#include "render/BackgroundRenderable.h"

#include "sound/SoundSystem.h"

#include "SDL_image.h"

#include "settings/SettingsManager.h"
#include "settings/ProgramSettings.h"
#include "config.h"

namespace Project {
namespace SDL {

void SDLMain::CameraObserver::observe(Event::CameraMovement *event) {
    double x = event->getMovement().getX();
    double y = event->getMovement().getY();
    
    Math::Point translation = camera->getLookDirection()*(y * 0.25f)
        + camera->getRightDirection() * (x * 0.25f);
    
    camera->translate(translation);
}

void SDLMain::QuitObserver::observe(Event::QuitEvent *event) {
    LOG2(GLOBAL, PROGESS, "Quit requested");
    sdlMain->doQuit();
}

SDLMain::SDLMain() {
    quit = false;
    
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
    if(cameraObject) {
        cameraObject->camera->setAspect(
            static_cast<double>(width)/static_cast<double>(height));
        cameraObject->camera->glProjection();
    }
    
    glMatrixMode(GL_MODELVIEW);
}

void SDLMain::initSDL() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_WM_SetCaption("The Project", NULL);
    
    int width = GET_SETTING("display.width", 0);
    int height = GET_SETTING("display.height", 0);
    int flags = SDL_INIT_FLAGS;
    if(GET_SETTING("display.fullscreen", 0)) flags |= SDL_FULLSCREEN;
    
    int bpp = GET_SETTING("display.bpp", 0);
    
    SDL_SetVideoMode(width, height, bpp, flags);
    
    projector.setCurrentDimensions(Point2D(width, height));
    
    // camera stuff follows
    
    simpleTrackball = new OpenGL::SimpleTrackball();
    
    cameraObject = new SDL::CameraObject();
    cameraObject->camera->setFieldOfViewDegrees(60.0f);
    cameraObject->camera->setPosition(Point(0.0f, 2.0f, -4.0f));
    cameraObject->camera->setFarPlane(VIEW_DISTANCE);
    updateCamera();
    
    // this should be called after the camera has been created so that it gets
    // the correct aspect ratio
    resizeGL(width, height);
}

void SDLMain::initOpenGL() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_RESCALE_NORMAL);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}

void SDLMain::initRenderer() {
    //Instantiate the rendering objects
    meshLoader = new Render::MeshLoader();
    renderer = new Render::RenderManager();
    lightManager = renderer->getLightManager();
    rootRenderable = new Render::RenderList();
    
    renderer->loadShadersFile("shaders.txt");
    
    //Load the model
    meshLoader->loadOBJ("testTerrain", GET_SETTING("map", "models/testterrain.obj"));
    Render::RenderableObject *object
        = meshLoader->loadOBJ("playerCube", "models/vehicle01.obj");
    object->getRenderProperties()->setTransformation(
        Math::Matrix::getScalingMatrix(Math::Point(2.0, 2.0, 2.0)));
    
    //Add the test terrain
    test_terrain = meshLoader->getModelByName("testTerrain");
    
    paintManager = new Paint::PaintManager();
    
    testPaintColor = 0;
    if(GET_SETTING("render.paint.enabled", 1)) {
        Paint::PaintGenerator paint_gen(test_terrain->getTriangles());
        paintCells = paint_gen.getPaintCells();
        paintManager->setPaintCells(paintCells);
    }
    
    Render::BackgroundRenderable* background = new Render::BackgroundRenderable(cameraObject->camera);
    background->getRenderProperties()->setWantsShaderName("backgroundShader");
    
    rootRenderable->addRenderable(test_terrain);
    rootRenderable->addRenderable(background);
    
    Render::TextureCube* background_texture = new Render::TextureCube(
        "models/starfield.png",
        "models/starfield.png",
        "models/starfield.png",
        "models/starfield.png",
        "models/starfield.png",
        "models/starfield.png");

    renderer->setCubeMap(background_texture);
    
    //Create some lights
    Render::Light* light = new Render::Light(Math::Point(1.0f, 2.0f, -1.0f));
    light->setHasAttenuation(false);
    light->setAmbient(OpenGL::Color(0.5, 0.5, 0.5));
    light->setStrength(20.0f);
    lightManager->addLight(light);

    light = new Render::Light(Math::Point(7.0f, 2.5f, 3.0f));
    light->setDiffuse(OpenGL::Color::INDIGO);
    light->setSpecular(OpenGL::Color::INDIGO);
    light->setStrength(10.0f);
    lightManager->addLight(light);
}

void SDLMain::run() {
    initSDL();
    
    initOpenGL();
    initRenderer();
    
    accelControl = new Timing::AccelControl();
    
    // this must happen before Players are created
    physicsWorld = new Physics::PhysicsWorld();
    suspension = new Physics::Suspension(10);
    
    worldManager = new Object::WorldManager();
    
    network = new NetworkPortal();
    if(network->connectTo(
        GET_SETTING("network.host", "localhost").c_str(),
        GET_SETTING("network.port", 1820))) {
        
        network->waitForWorld();
        clientData = new ClientData(network->getID());
        playerManager = new PlayerManager(network->getID(), worldManager);
        Settings::ProgramSettings::getInstance()->setConnected(true);
    }
    else {
        clientData = new ClientData();
        playerManager = new PlayerManager(0, worldManager);
    }
    paintSubsystem = new Paint::PaintSubsystem(worldManager, paintManager, 20);
    
    inputManager = new InputManager(10, clientData);
    inputManager->init();
    
    cameraObject->setPlayerManager(playerManager);
    
    ADD_OBSERVER(new CameraObserver(simpleTrackball, cameraObject->camera));
    ADD_OBSERVER(new QuitObserver(this));
    
    Physics::PhysicsWorld::getInstance()->registerRigidBody(
        Physics::PhysicsFactory::createRigidTriMesh(
            test_terrain->getTriangles()));
    
#ifdef HAVE_OPENAL
    Sound::SoundSystem *soundSystem = new Sound::SoundSystem();
    if(!soundSystem->initialize()) {
        delete soundSystem;
        soundSystem = NULL;
    }
#endif
    
    LOG2(GLOBAL, PROGRESS, "Entering main game loop");
    
    Uint32 lastTime = SDL_GetTicks();
    while(!quit) {
        handleEvents();
        
        network->checkNetwork();
        
        paintSubsystem->doStep(SDL_GetTicks());
        suspension->setData(worldManager->getPlayerList(), renderer);
        suspension->checkForWheelsOnGround();
        
        inputManager->doStep(SDL_GetTicks());
        inputManager->doPausedChecks();
        
        if(!Timing::AccelControl::getInstance()->getPaused()) {
            static Uint32 lastPhysicsTime = SDL_GetTicks();
            lastPhysicsTime += Timing::AccelControl::getInstance()
                ->getPauseSkip();
            Uint32 thisTime = SDL_GetTicks();
            physicsWorld->stepWorld((thisTime - lastPhysicsTime) * 1000);
            lastPhysicsTime = thisTime;
        }
        
        cameraObject->doStep(SDL_GetTicks());
        
        render();
        suspension->doStep(SDL_GetTicks());
        physicsWorld->render();
        glFlush();
        
        SDL_GL_SwapBuffers();
        
#ifdef HAVE_OPENAL
        if(soundSystem) soundSystem->doAction(SDL_GetTicks());
#endif
        
        {
            static const int RATE = 10;
            Uint32 thisTime = SDL_GetTicks();
            int timeTakenSoFar = static_cast<int>(thisTime - lastTime);
            if(timeTakenSoFar < RATE) {
                SDL_Delay(RATE - timeTakenSoFar);
            }
            while(lastTime < thisTime) lastTime += RATE;
        }
        
        accelControl->clearPauseSkip();
    }
    
    LOG2(GLOBAL, PROGRESS, "Exiting main game loop");
    
#ifdef HAVE_OPENAL
    delete soundSystem;
#endif
    
    delete rootRenderable;
    
    delete inputManager;
    delete network;
    delete suspension;
    
    delete accelControl;
    
    delete meshLoader;
    delete physicsWorld;
}

void SDLMain::handleEvents() {
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
            
            if(event.button.button == 1) {  // left
                simpleTrackball->setMouseStartAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));
            }
            break;
        case SDL_MOUSEMOTION:
            if(event.motion.state & SDL_BUTTON(1)) {
                if(event.button.button == 1) {
                    simpleTrackball->setMouseCurrentAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));
                    updateCamera();
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            inputManager->handleEvent(&event);
            break;
        }
    }
}

void SDLMain::updateCamera() {
	cameraObject->camera->setLookDirection(simpleTrackball->getSpherePoint());
}

void SDLMain::render() {
    glMatrixMode(GL_MODELVIEW);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
	OpenGL::Color::glColor(OpenGL::Color::WHITE);
    
	cameraObject->camera->glLookAt();
    
	//Activate all lights near the camera's focal point
	lightManager->activateNearFocalPoint(cameraObject->camera->getLookPosition());
	//Activate all lights visible to the camera
	lightManager->activateIntersectingLights(*cameraObject->camera->getFrustrum());

	//Render the active lights
	lightManager->drawActiveLightSpheres();
    
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//Pass the camera to the renderer for culling
	renderer->setCamera(cameraObject->camera);

    //Render most of the world
    worldManager->getWorld()->preRender();
    worldManager->getWorld()->getRenderableObject()->render(renderer);
	
	//Render the scene
	rootRenderable->render(renderer);

	//Render the paint
	paintManager->render(renderer);

	//Reset the lights
	lightManager->resetLights();
    
	//Revert the rendering state
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

}  // namespace SDL
}  // namespace Project
