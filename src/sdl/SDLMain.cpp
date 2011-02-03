#include <cctype>
#include <cmath>

#include "SDL.h"
#include "opengl/OpenGL.h"
#include "GL/glu.h"
#include "misc/StdVectorFunctions.h"

#include "opengl/Color.h"

#include "SDLMain.h"
#include "log/Logger.h"

#include "math/Point.h"
#include "opengl/MathWrapper.h"
#include "opengl/GeometryDrawing.h"
#include "render/TextureCube.h"
#include "map/MapLoader.h"

#include "event/ObserverList.h"

#include "physics/PhysicsFactory.h"
#include "physics/PhysicsWorld.h"

#include "render/ShaderUniformVector4.h"
#include "render/BackgroundRenderable.h"
#include "render/TransformedMesh.h"
#include "render/ColorConstants.h"

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

	viewWidth = width;
	viewHeight = height;
    
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
}

void SDLMain::initRenderer() {
    //Instantiate the rendering objects
    meshLoader = new Render::MeshLoader();
    renderer = new Render::RenderManager();
    lightManager = renderer->getLightManager();
    mapRenderable = new Render::RenderList();

    renderer->loadShadersFile("shaders.txt");
    
    //Load the vehicle model
    Render::RenderableObject *object
        = meshLoader->loadOBJ(VEHICLE_CHASSIS_MODEL_NAME, GET_SETTING("render.model.vehicle", ""));
        
    Render::RenderableObject *objectTire
        = meshLoader->loadOBJ(VEHICLE_WHEEL_MODEL_NAME, GET_SETTING("render.model.tire", ""));
        
    object->getRenderProperties()->setTransformation(
        Math::Matrix::getScalingMatrix(Math::Point(2.0, 2.0, 2.0)));
    
    objectTire->getRenderProperties()->setTransformation(
        Math::Matrix::getScalingMatrix(Math::Point(2.5, 2.5, 2.5)));
    
	//Instantiate the map
	map = new Map::HRMap();
	if (map->loadMapFile(GET_SETTING("map", "data/testtrack.hrm"))) {
		LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
	}
	else {
		LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
	}
    
	paintManager = new Paint::PaintManager();
	paintManager->setPaintCells(map->getPaintCells());
    
	//Add the map lights to the light manager
	vector<Render::Light*> map_lights = map->getLights();
	for (unsigned i = 0; i < map_lights.size(); i++) {
		lightManager->addLight(map_lights[i], !map_lights[i]->getHasAttenuation(), false);
	}
    
    //Set background shader
    // !!! this is not freed
    Render::BackgroundRenderable* background = new Render::BackgroundRenderable(cameraObject->camera);
    background->getRenderProperties()->setWantsShaderName("backgroundShader");
    mapRenderable->addRenderable(background);

	minimap = new Map::Minimap();
	minimap->setMapInfo(map);
    
    renderer->setCubeMap(map->getCubeMap());
}

void SDLMain::run() {
    initSDL();
    
    initOpenGL();
    initRenderer();
    
    accelControl = new Timing::AccelControl();
    
    // this must happen before Players are created
    physicsWorld = new Physics::PhysicsWorld();
    suspension = new Physics::Suspension(10);
    
    network = new NetworkPortal();
    bool isConnectedToNetwork;
    if(network->connectTo(
        GET_SETTING("network.host", "localhost").c_str(),
        GET_SETTING("network.port", 1820))) {
        
        Object::World *world;
        Object::PlayerList *playerList;
        
        network->waitForWorld(world, playerList);
        
        worldManager = new Object::WorldManager(world, playerList);
        clientData = new ClientData(network->getID());
        playerManager = new PlayerManager(network->getID(), worldManager);
        Settings::ProgramSettings::getInstance()->setConnected(true);
        isConnectedToNetwork = true;
    }
    else {
        worldManager = new Object::WorldManager();
        clientData = new ClientData();
        playerManager = new PlayerManager(0, worldManager);
        isConnectedToNetwork = false;
    }
    paintSubsystem = new Paint::PaintSubsystem(worldManager, paintManager, 20);
    
    inputManager = new InputManager(10, clientData);
    inputManager->init();
    
    cameraObject->setPlayerManager(playerManager);
    
    ADD_OBSERVER(new CameraObserver(simpleTrackball, cameraObject->camera));
    ADD_OBSERVER(new QuitObserver(this));
    
    Map::MapLoader().load(map, mapRenderable);
    raceManager = new Map::RaceManager(map);
    
    playerManager->setRaceManager(raceManager);
    
    if(!isConnectedToNetwork) {
        worldManager->initForClient(clientData->getPlayerID(),
            raceManager->startingPointForPlayer(clientData->getPlayerID()),
			raceManager->startingPlayerDirection());
    }
    
#ifdef HAVE_OPENAL
    Sound::SoundSystem *soundSystem = new Sound::SoundSystem();
    if(!soundSystem->initialize()) {
        delete soundSystem;
        soundSystem = NULL;
    }
#endif
    
    LOG2(GLOBAL, PROGRESS, "Entering main game loop");
    Uint32 lastTime = SDL_GetTicks();
    accelControl->setPauseSkipDirectly(lastTime);
    while(!quit) {
        handleEvents();
        
        network->checkNetwork();
        
        paintSubsystem->doStep(SDL_GetTicks());
        suspension->setData(worldManager, renderer);
        suspension->checkForWheelsOnGround();
        
        // must do paused (static) checks first, in case the game unpauses
        inputManager->doPausedChecks();
        inputManager->doStep(SDL_GetTicks());
        
        if(!Timing::AccelControl::getInstance()->getPaused()) {
            static Uint32 lastPhysicsTime = SDL_GetTicks();
            lastPhysicsTime += Timing::AccelControl::getInstance()
                ->getPauseSkip();
            Uint32 thisTime = SDL_GetTicks();
            
            physicsWorld->stepWorld((thisTime - lastPhysicsTime) * 1000);
            lastPhysicsTime = thisTime;
        }
        
        cameraObject->doStep(SDL_GetTicks());
        
        {
            render();

            // suspension does not look good when it is out of sync with rendering
            //suspension->doAction(SDL_GetTicks());
            suspension->doStep(SDL_GetTicks());

			physicsWorld->render();

			if (GET_SETTING("render.minimap.enable", true)) {
				
				

				double minimap_draw_height = viewHeight*GET_SETTING("render.minimap.drawheight", 0.2);
				double minimap_draw_width = minimap_draw_height*GET_SETTING("render.minimap.drawaspect", 1.0);

				glViewport(viewWidth-minimap_draw_width, 0, minimap_draw_width, minimap_draw_height);

				minimap->drawMinimap(
					GET_SETTING("render.minimap.height", 40.0),
					GET_SETTING("render.minimap.drawaspect", 1.0),
					worldManager->getPlayer(clientData->getPlayerID())->getPosition(),
					worldManager,
					paintManager);

				glViewport(0, 0, viewWidth, viewHeight);
			}
       
            glFlush();
            
            SDL_GL_SwapBuffers();
        }
        
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

	glDeleteTextures(1, &minimapTexture);
    
	delete map;
    delete mapRenderable;
    
    delete inputManager;
    delete network;
    delete suspension;
    
    delete accelControl;
    
    delete paintSubsystem;
    delete paintManager;
    
    delete meshLoader;
    delete physicsWorld;
    
    delete simpleTrackball;
    delete cameraObject;
    delete renderer;
    
    delete clientData;
    delete playerManager;
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

	cameraObject->camera->glProjection();

    glMatrixMode(GL_MODELVIEW);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
	OpenGL::Color::glColor(OpenGL::Color::WHITE);
    
	cameraObject->camera->glLookAt();
  
	//Activate all lights near the camera focal point
	lightManager->activateNearFocalPoint(cameraObject->camera->getLookPosition(), 10.0);
	//Activate all lights visible to the camera
	lightManager->activateIntersectingLights(*cameraObject->camera->getFrustrum());

	//Render the active lights
	lightManager->drawActiveLightSpheres(false);
    
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//Pass the camera to the renderer for culling
	renderer->setCamera(cameraObject->camera);
    
    //Render most of the world
    worldManager->getWorld()->preRender();
    worldManager->getWorld()->getRenderableObject()->render(renderer);
    
	//Render the map
	mapRenderable->render(renderer);
    
	//Reset the lights
	lightManager->resetLights();
    
	//Revert the rendering state
	//glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
    
	//Render the paint
	paintManager->render(renderer);
    
	if(GET_SETTING("render.drawpathnodes", false)) {
		renderAIDebug();
	}
}

void SDLMain::renderAIDebug() {
    // code cloned from MapEditorWidget
    
#define MAP_EDITOR_PATHNODE_LENGTH 0.4
#define MAP_EDITOR_PATHNODE_HEIGHT 0.2
#define MAP_EDITOR_PATHNODE_FLOAT_HEIGHT 0.05
#define MAP_EDITOR_PATHNODE_COLOR OpenGL::Color::BLUE
#define MAP_EDITOR_PATHNODE_LINK_WIDTH 2.0f
#define MAP_EDITOR_PATHNODE_LINK_START_COLOR OpenGL::Color::WHITE
#define MAP_EDITOR_PATHNODE_LINK_END_COLOR OpenGL::Color::BLUE
    
    std::vector<Map::PathNode *> nodeList = map->getPathNodes();
    
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        Map::PathNode* node = nodeList[i];
        
        OpenGL::Color::glColor(MAP_EDITOR_PATHNODE_COLOR);
        
        OpenGL::GeometryDrawing::drawBoundingBox3D(
            BoundingBox3D(MAP_EDITOR_PATHNODE_LENGTH,
            MAP_EDITOR_PATHNODE_HEIGHT,
            MAP_EDITOR_PATHNODE_LENGTH,
            node->getPosition()), false
            );
        
        glLineWidth(MAP_EDITOR_PATHNODE_LINK_WIDTH);
        glBegin(GL_LINES);
        const vector<Map::PathNode*>& linked_nodes = node->getNextNodes();
        for (unsigned int i = 0; i < linked_nodes.size(); i++) {
            OpenGL::Color::glColor(MAP_EDITOR_PATHNODE_LINK_START_COLOR);
            OpenGL::MathWrapper::glVertex(node->getPosition());
            OpenGL::Color::glColor(MAP_EDITOR_PATHNODE_LINK_END_COLOR);
            OpenGL::MathWrapper::glVertex(linked_nodes[i]->getPosition());
        }
        glEnd();
        glLineWidth(1.0f);
    }
}

}  // namespace SDL
}  // namespace Project
