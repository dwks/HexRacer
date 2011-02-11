#include <iomanip>  // for std::setprecision()
#include <vector>

#include "SDL.h"  // for SDL_GetVideoSurface()

#include "GameRenderer.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/MathWrapper.h"

#include "map/MapLoader.h"

#include "hud/LapProgressBar.h"

#include "log/Logger.h"
#include "config.h"

namespace Project {
namespace SDL {

void GameRenderer::construct(OpenGL::Camera *camera) {
    //Instantiate the rendering objects
    meshLoader = boost::shared_ptr<Mesh::MeshLoader>(
        new Mesh::MeshLoader());
    renderer = boost::shared_ptr<Render::RenderManager>(
        new Render::RenderManager());
    lightManager = renderer->getLightManager();
    mapRenderable = boost::shared_ptr<Render::RenderList>(
        new Render::RenderList());
    
    renderer->getShaderManager()->loadShadersFile("shaders.txt");
    
    //Load the vehicle model
    meshLoader->loadOBJ(VEHICLE_CHASSIS_MODEL_NAME, GET_SETTING("render.model.vehicle", ""));
	meshLoader->loadOBJ(VEHICLE_GLOW_MODEL_NAME, GET_SETTING("render.model.vehicleglow", ""));
    meshLoader->loadOBJ(VEHICLE_WHEEL_MODEL_NAME, GET_SETTING("render.model.tire", ""));
    
    //Instantiate the map
    map = boost::shared_ptr<Map::HRMap>(new Map::HRMap());
    if (map->loadMapFile(GET_SETTING("map", "data/testtrack.hrm"))) {
        LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    else {
        LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    
    //Add the map lights to the light manager
    std::vector<OpenGL::Light*> map_lights = map->getLights();
    for (unsigned i = 0; i < map_lights.size(); i++) {
        lightManager->addLight(map_lights[i], !map_lights[i]->getHasAttenuation(), false);
    }
    
    //Set background shader
    background = boost::shared_ptr<Render::BackgroundRenderable>(
        new Render::BackgroundRenderable(camera));
    background->getRenderProperties()->setWantsShaderName("backgroundShader");
    
	hudRenderer = boost::shared_ptr<HUD::HUDRenderer>(new HUD::HUDRenderer());

    minimap = boost::shared_ptr<HUD::Minimap>(new HUD::Minimap());
    minimap->setMapInfo(map.get());

	speedometer = boost::shared_ptr<HUD::Speedometer>(new HUD::Speedometer());
    
    renderer->setCubeMap(map->getCubeMap());
    
    paintManager = boost::shared_ptr<Paint::PaintManager>(
        new Paint::PaintManager());
    paintManager->setPaintCells(map->getPaintCells());
    
    Map::MapLoader().load(map.get(), mapRenderable.get());
}

void GameRenderer::render(OpenGL::Camera *camera, Object::World *world) {
    
    //Init Rendering----------------------------------------------------------------------

    double near_plane = GET_SETTING("render.camera.nearplane", 0.1);
    double far_plane = GET_SETTING("render.camera.farplane", 300.0);
    double view_length = far_plane-near_plane;
	double paint_far_plane = near_plane+GET_SETTING("render.camera.paintthreshhold", 0.5)*view_length;

    camera->setNearPlane(near_plane);
    camera->setFarPlane(far_plane);
    camera->setFrustrumNearPlaneEnabled(false);
    camera->setFrustrumFarPlaneEnabled(false);
    camera->glProjection();

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera->glLookAt();

    OpenGL::Color::glColor(OpenGL::Color::WHITE);
    renderer->setCamera(camera);
  
    //Activate all lights visible to the camera
    lightManager->activateIntersectingLights(*camera->getFrustrum());
    
    //Render the background
    background->render(renderer.get());

    //Render the active lights
    if(GET_SETTING("render.drawlightspheres", false))
        lightManager->drawActiveLightSpheres(false);
    
    renderWorld(world);

	//Paint Rendering----------------------------------------------------------------------

	//Set the camera frustrum far plane to the paint threshhold
	camera->setFarPlane(paint_far_plane);
	camera->setFrustrumFarPlaneEnabled(true);

	//Cull backfaces of paint cells
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//Render the paint
    paintManager->render(renderer.get());

	//Revert Rendering Settings
	glDisable(GL_CULL_FACE);

	camera->setFarPlane(far_plane);
	camera->setFrustrumFarPlaneEnabled(false);

    //Reset the lights
    lightManager->resetLights();
}

void GameRenderer::renderHUD(Object::WorldManager *worldManager, Object::Player *player) {

    int viewWidth = SDL_GetVideoSurface()->w;
    int viewHeight = SDL_GetVideoSurface()->h;

	hudRenderer->setViewSize(viewWidth, viewHeight);
	hudRenderer->renderingStateSetup();
    
	//-Minimap----------------------------------------------------------------------------
    if (GET_SETTING("hud.minimap.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.minimap.drawheight", 0.2);

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MAX,
			HUD::HUDRenderer::ALIGN_MAX,
			draw_height,
			draw_height,
			10,
			10);

		minimap->setWorldInfo(worldManager, paintManager.get());
		minimap->setDrawInfo(GET_SETTING("hud.minimap.height", 40.0));
		minimap->setViewInfo(player->getPosition(), player->getPhysicalObject()->getFrontDirection());

		minimap->render(hudRenderer.get());
    }

	//-Speedometer----------------------------------------------------------------------------
    if (GET_SETTING("hud.speedometer.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.speedometer.drawheight", 0.2);

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MAX,
			HUD::HUDRenderer::ALIGN_MIN,
			draw_height,
			draw_height,
			10,
			10);

		speedometer->setColor(OpenGL::Color::WHITE);
		speedometer->setMaxSpeed(GET_SETTING("hud.speedometer.maxspeed", 20.0));
		speedometer->setSpeed(
			Math::Point::point2D(player->getPhysicalObject()->getLinearVelocity(), Y_AXIS).length()
			);

		speedometer->render(hudRenderer.get());

    }

	//-Lap Progress----------------------------------------------------------------------------
    if (GET_SETTING("hud.lapprogress.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.lapprogress.drawheight", 0.5);

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MIN,
			HUD::HUDRenderer::ALIGN_MID,
			20,
			draw_height,
			10,
			0);

		HUD::LapProgressBar bar;
		bar.setProgress(player->getPathTracker()->getLapProgress());
		bar.render();
        percentageComplete->setText(Misc::StreamAsString() << std::setprecision(3)
            << player->getPathTracker()->getLapProgress() * 100 << "%");

    }

	hudRenderer->renderingStateReset();
	hudRenderer->resetViewport();

}

void GameRenderer::setGUI(boost::shared_ptr<GUI::GUISystem> gui) {

    this->gui = gui;
    
    percentageComplete = new Widget::TextWidget(
        "percentageComplete", OpenGL::Color::WHITE, "0%",
        Widget::NormalTextLayout::ALIGN_HCENTRE | Widget::NormalTextLayout::ALIGN_VCENTRE);
    percentageComplete->updateLayout(Widget::WidgetRect(0.0, 0.9, 0.1, 0.05));
    /*percentageComplete = new Widget::ButtonWidget("percentageComplete",
        "0%", Widget::WidgetRect(0.0, 0.1, 0.1, 0.05));*/
    gui->getScreen("running")->addChild(percentageComplete);
}

void GameRenderer::renderWorld(Object::World *world) {
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    //Render most of the world
    world->preRender();
    world->getRenderableObject()->render(renderer.get());
    
    //Render the map
    mapRenderable->render(renderer.get());
    
    //Revert the rendering state
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void GameRenderer::renderDebug(Object::WorldManager *worldManager, Object::Player *player) {

	/*
	camera->glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->glLookAt();
	*/
	if (GET_SETTING("render.drawpathnodes", false)) {
		renderAIDebug(player);
	}

}

void GameRenderer::renderAIDebug(Object::Player *player) {
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
        
		if (node == player->getPathTracker()->getCurrentNode())
			OpenGL::Color::glColor(OpenGL::Color::RED);
		else
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

	Point p = player->getPathTracker()->getProgressPosition();
	OpenGL::Color::glColor(OpenGL::Color::WHITE);
	OpenGL::GeometryDrawing::drawBoundingBox3D(BoundingBox3D(0.5, 0.5, 0.5, p), true);

}

}  // namespace SDL
}  // namespace Project
