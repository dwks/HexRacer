#include <iomanip>  // for std::setprecision()
#include <cmath>

#include "SDL.h"  // for SDL_GetVideoSurface()

#include "GameRenderer.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/MathWrapper.h"
#include "math/Geometry.h"
#include "math/BoundingBox3D.h"
#include "timing/AccelControl.h"
#include "world/TimeElapsed.h"

#include "map/MapOptions.h"

#include "hud/LapProgressBar.h"

#include "MenuLoop.h"

#include "widget/CompositeWidget.h"

#include "log/Logger.h"
#include "config.h"

#include "event/EventSystem.h"

namespace Project {
namespace SDL {

GameRenderer::~GameRenderer() {
    if(fpsRate) {
        dynamic_cast<Widget::CompositeWidget *>(gui->getScreen("running"))
            ->removeChild(fpsRate->getWidget()->getName());
    }

	if (renderer->getRenderSettings()->getBloomEnabled())
		clearBloom();

	if (renderer->getRenderSettings()->getShadowMappingEnabled())
		clearShadowMap();
}

void GameRenderer::construct(OpenGL::Camera *camera)
{

	METHOD_OBSERVER(&GameRenderer::handleGameStageChanged);

    //Instantiate the rendering objects
    meshLoader = boost::shared_ptr<Mesh::MeshLoader>(
        new Mesh::MeshLoader());
    renderer = boost::shared_ptr<Render::RenderManager>(
        new Render::RenderManager());
    lightManager = renderer->getLightManager();
    mapRenderable = boost::shared_ptr<Render::RenderList>(
        new Render::RenderList());

	stringTextureCache = boost::shared_ptr<Render::StringTextureCache>(
        new Render::StringTextureCache());
    
    renderer->getShaderManager()->loadShadersFile("shaders.txt");
    
    //Load the vehicle model
    meshLoader->loadOBJ(VEHICLE_CHASSIS_MODEL_NAME, GET_SETTING("render.model.chassis", ""));
    meshLoader->loadOBJ(VEHICLE_NONCHASSIS_MODEL_NAME, GET_SETTING("render.model.nonChassis", ""));
    meshLoader->loadOBJ(VEHICLE_WHEEL_MODEL_NAME, GET_SETTING("render.model.tire", ""));
    
    //Instantiate the map
    map = boost::shared_ptr<Map::HRMap>(new Map::HRMap());
    if (map->loadMapFile(GET_SETTING("map", "data/testtrack.hrm"), SDL::MenuLoop::getLoadingProgressTracker())) {
        LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    else {
        LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    
    //Add the map lights to the light manager
    std::vector<OpenGL::Light*> map_lights = map->getMapObjects().getLights();
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
	placingList = boost::shared_ptr<HUD::PlacingList>(new HUD::PlacingList());
	playerPlacingText = boost::shared_ptr<HUD::PlayerPlacingText>(new HUD::PlayerPlacingText());
	bonusMessages = boost::shared_ptr<HUD::BonusMessages>(new HUD::BonusMessages());
	raceClock = boost::shared_ptr<HUD::RaceClock>(new HUD::RaceClock());
    
    renderer->setCubeMap(map->getCubeMap());
    
    paintManager = boost::shared_ptr<Paint::PaintManager>(
        new Paint::PaintManager(true));

	mapLoader = boost::shared_ptr<Map::MapLoader>(new Map::MapLoader());
	mapLoader->load(map.get(), SDL::MenuLoop::getLoadingProgressTracker(), mapRenderable.get(), paintManager.get());

	shadowProperties = NULL;
	shadowCamera = NULL;

	clockStarted = false;

	if (renderer->getRenderSettings()->getBloomEnabled())
		initBloom();

	if (renderer->getRenderSettings()->getShadowMappingEnabled())
		initShadowMap();
}

void GameRenderer::render(OpenGL::Camera *camera, Object::WorldManager *worldManager) {
    if(fpsRate) fpsRate->countFrame();
    
	//Activate all lights visible to the camera
    lightManager->activateIntersectingLights(*camera->getFrustrum());

	Render::RenderList scene_render_list;
	scene_render_list.addRenderable(worldManager->getWorld()->getRenderableObject());
	scene_render_list.addRenderable(mapRenderable.get());
    
    //Init Rendering----------------------------------------------------------------------

	worldManager->getWorld()->preRender();

	OpenGL::Light* strongest_light = renderer->getLightManager()->getActiveLight(0);

	if (renderer->getRenderSettings()->getShadowMappingEnabled() &&
		GET_SETTING("render.shadow.enable", false) && strongest_light) {
		updateShadowCamera(strongest_light->getPosition(), camera);
		renderToShadowMap(scene_render_list);
		renderer->setShadowMapTexture(shadowDepthTexture);
		renderer->getRenderSettings()->setApplyToShadowMatrix(true);
	}
	else
		renderer->setShadowMapTexture(0);

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

	renderer->getLightManager()->reapplyActiveLights();

    OpenGL::Color::glColor(OpenGL::Color::WHITE);
    renderer->setCamera(camera);

	 //Render the background
    background->render(renderer.get());

	//Render the active lights
    if(GET_SETTING("render.drawlightspheres", false))
        lightManager->drawActiveLightSpheres(false);
    
	renderWorld(worldManager->getWorld());

	//Paint Rendering----------------------------------------------------------------------

	//Set the camera frustrum far plane to the paint threshhold
	camera->setFarPlane(paint_far_plane);
	camera->setFrustrumFarPlaneEnabled(true);

	//Cull the backfaces of paint
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render the paint
	paintManager->setFadePlanes((float) (near_plane+(paint_far_plane-near_plane)*0.75), (float) paint_far_plane);
    paintManager->render(renderer.get());

	//Render the erasing effects
	Object::WorldManager::PlayerIteratorType iterator = worldManager->getPlayerIterator();
	while (iterator.hasNext()) {
		Object::Player* player = iterator.next();

		if (player->getOnGround() && player->getPaintType() == Event::TogglePainting::ERASING) {
			paintManager->renderEraseEffect(player->getPosition(), PAINTING_RADIUS);
		}
	}

	//Revert Rendering Settings
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (renderer->getRenderSettings()->getBloomEnabled() && GET_SETTING("render.bloom.enable", false)) {

		//Render to the bloom buffer
		camera->setFarPlane(GET_SETTING("render.bloom.farplane", 50.0));
		bloomScene->setChild(&scene_render_list);
		renderToBloomBuffer(*bloomRenderable);
		textureProjection();
		int blur_passes = GET_SETTING("render.bloom.blurpasses", 5);
		for (int i = 0; i < blur_passes; i++)
			bloomBlurPass();
		applyBloomBuffer();

		bloomScene->setChild(NULL);

	}

	renderer->getRenderSettings()->setApplyToShadowMatrix(false);

	camera->setFarPlane(far_plane);
	camera->setFrustrumFarPlaneEnabled(false);

	//Reset the lights
    lightManager->resetLights();

	renderer->cleanup();
}

void GameRenderer::renderHUD(Object::WorldManager *worldManager, Object::Player *player, Map::RaceManager *raceManager) {

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
    }

	//-Placing List ----------------------------------------------------------------------------
    if (GET_SETTING("hud.placinglist.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.placinglist.drawheight", 0.5);
		int draw_width = GET_SETTING("hud.placinglist.drawwidth", 400);
		int entry_height = Math::maximum(static_cast<int>(viewHeight*GET_SETTING("hud.placinglist.entryheight", 0.5)),
			static_cast<int>(GET_SETTING("hud.placinglist.minentryheight", 20)));
		entry_height = Math::minimum(entry_height, GET_SETTING("hud.placinglist.maxentryheight", 20));

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MIN,
			HUD::HUDRenderer::ALIGN_MID,
			draw_width,
			draw_height,
			10,
			0);
	
		placingList->setTotalWidth(draw_width);
		placingList->setTotalHeight(draw_height);
		placingList->setEntryHeight(entry_height);
		placingList->render(raceManager);
    }

	//-Player Placing Text ----------------------------------------------------------------------------
    if (!GET_SETTING("internal.practicemode", false) && GET_SETTING("hud.playerplacingtext.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.playerplacingtext.drawheight", 0.2);

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MIN,
			HUD::HUDRenderer::ALIGN_MIN,
			draw_height,
			draw_height,
			10,
			0);
	
		playerPlacingText->setPlacing(player->getPathTracker()->getRanking());
		playerPlacingText->render();
    }

	//-Bonus Messages ----------------------------------------------------------------------------
    if (!GET_SETTING("internal.practicemode", false) && GET_SETTING("bonus.enable", true) && GET_SETTING("hud.bonusmessages.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.bonusmessages.drawheight", 0.5);
		int draw_width = viewWidth*GET_SETTING("hud.bonusmessages.drawwidth", 0.5);
		int entry_height = Math::maximum(static_cast<int>(viewHeight*GET_SETTING("hud.bonusmessages.entryheight", 0.5)),
			static_cast<int>(GET_SETTING("hud.bonusmessages.minentryheight", 20)));
		entry_height = Math::minimum(entry_height, GET_SETTING("hud.bonusmessages.maxentryheight", 20));

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MID,
			HUD::HUDRenderer::ALIGN_MAX,
			draw_width,
			draw_height,
			10,
			0);
	
		bonusMessages->setTotalWidth(draw_width);
		bonusMessages->setTotalHeight(draw_height);
		bonusMessages->setEntryHeight(entry_height);
		bonusMessages->setWorldManager(worldManager);
		bonusMessages->setPlayerID(player->getID());
		bonusMessages->render();
    }

	//-Race Clock ----------------------------------------------------------------------------
    if (GET_SETTING("hud.raceclock.enable", true)) {

        int draw_height = viewHeight*GET_SETTING("hud.raceclock.drawheight", 0.5);
		int draw_width = GET_SETTING("hud.raceclock.drawwidth", 400);

		hudRenderer->setupViewport(
			HUD::HUDRenderer::ALIGN_MIN,
			HUD::HUDRenderer::ALIGN_MAX,
			draw_width,
			draw_height,
			50,
			viewHeight*GET_SETTING("hud.raceclock.verticalpadding", 0.1));
	
		raceClock->setWidth(draw_width);
		raceClock->setHeight(draw_height);

		if (!Timing::AccelControl::getInstance()->getPaused() && clockStarted)
			raceClock->setTotalTime(World::TimeElapsed::getInstance().getGameTime());

		raceClock->addTimeOffset(Timing::AccelControl::getInstance()->getPauseSkip());
		/*
		raceClock->setSeconds((World::TimeElapsed::getInstance().getGameTime()
			-Timing::AccelControl::getInstance()->getPauseSkip())/1000);
		*/
		raceClock->render();

    }

	

	hudRenderer->renderingStateReset();
	hudRenderer->resetViewport();

}

void GameRenderer::setGUI(boost::shared_ptr<GUI::GUISystem> gui) {
    this->gui = gui;
    
    if(GET_SETTING("render.showfps", 1)) {
        fpsRate = boost::shared_ptr<FPSRateMonitor>(new FPSRateMonitor());
        gui->getScreen("running")->addChild(fpsRate->getWidget());
    }

}

void GameRenderer::renderWorld(Object::World *world) {
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    //Render most of the world
    world->getRenderableObject()->render(renderer.get());
    
    //Render the map
    mapRenderable->render(renderer.get());
    
    //Revert the rendering state
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void GameRenderer::renderDebug(OpenGL::Camera *camera, Object::WorldManager *worldManager, Object::Player *player) {

	if (GET_SETTING("render.drawshadowcamera", false))
		renderShadowDebug();

	camera->glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->glLookAt();

	if (GET_SETTING("render.drawpathnodes", false))
		renderAIDebug(player);

	
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
    
    std::vector<Map::PathNode *> nodeList = map->getMapObjects().getPathNodes();
    
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

void GameRenderer::renderShadowDebug() {

	glViewport(0, 0, SDL_GetVideoSurface()->w/4, SDL_GetVideoSurface()->h/4);

	textureProjection();
	//glClear(GL_COLOR_BUFFER_BIT);

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
	drawQuad();

	OpenGL::Color::glColor(OpenGL::Color::RED);
	glBegin(GL_LINES);
	glVertex2f(0.5, 0.0);
	glVertex2f(0.5, 1.0);
	glVertex2f(0.0, 0.5);
	glVertex2f(1.0, 0.5);
	glEnd();


	shadowCamera->glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	shadowCamera->glLookAt();
	
	if (shadowFocusFrustrum.size() >= 5) {
		
		glBegin(GL_LINES);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[0]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[1]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[0]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[2]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[0]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[3]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[0]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[4]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[1]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[2]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[2]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[3]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[3]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[4]);

		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[4]);
		OpenGL::MathWrapper::glVertex(shadowFocusFrustrum[1]);
		glEnd();
	}
	

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);

}
void GameRenderer::initBloom() {

	//Texture for storing color information

	glGenTextures(1, &bloomColorTexture);
	glBindTexture(GL_TEXTURE_2D, bloomColorTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		renderer->getRenderSettings()->getBloomWidth(),
		renderer->getRenderSettings()->getBloomHeight(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		0);

	//Texture for storing depth information

	glGenTextures(1, &bloomDepthTexture);
	glBindTexture(GL_TEXTURE_2D, bloomDepthTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		renderer->getRenderSettings()->getBloomWidth(),
		renderer->getRenderSettings()->getBloomHeight(),
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_BYTE,
		0);

	//Texture for performing blur passes

	glGenTextures(1, &bloomBlurTexture);
	glBindTexture(GL_TEXTURE_2D, bloomBlurTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		renderer->getRenderSettings()->getBloomWidth(),
		renderer->getRenderSettings()->getBloomHeight(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		0);

	//Main bloom FBO

	glGenFramebuffersEXT(1, &bloomFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER, bloomFBO);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		bloomColorTexture,
		0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		bloomDepthTexture,
		0);

	//FBO for performing blur passes

	glGenFramebuffersEXT(1, &bloomBlurFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER, bloomBlurFBO);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		bloomBlurTexture,
		0);

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	bloomRenderable = new Render::RenderList();
	bloomScene = new Render::RenderParent();

	bloomRenderable->addRenderable(bloomScene);
	bloomRenderable->addRenderable(paintManager.get());

	if (map->getMapOptions().getBGBloomEnable()) {
		bloomBackground = new Render::RenderParent(background.get());
		bloomRenderable->addRenderable(bloomBackground);
		bloomBackground->getRenderProperties()->setWantsShaderName("backgroundBloomShader");
		bloomBackground->getRenderProperties()->setShaderOverride(true);
	}

	OpenGL::Material* default_material = new OpenGL::Material("bloomDefault");
	default_material->setAmbient(OpenGL::Color::BLACK);

	bloomScene->getRenderProperties()->setWantsShaderName("bloomShader");
	bloomScene->getRenderProperties()->setShaderOverride(true);
	bloomScene->getRenderProperties()->setMaterial(default_material);
	
	hBlurShaderIndex = renderer->getShaderManager()->shaderIndexFromName("hBlurShader");
	vBlurShaderIndex = renderer->getShaderManager()->shaderIndexFromName("vBlurShader");
}

void GameRenderer::renderToBloomBuffer(Render::RenderableObject& renderable) {

	glViewport(0, 0,
		renderer->getRenderSettings()->getBloomWidth(), renderer->getRenderSettings()->getBloomHeight());
	glBindFramebufferEXT(GL_FRAMEBUFFER, bloomFBO);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderer->getRenderSettings()->setRedrawMode(true);
	renderable.render(renderer.get());
	renderer->getRenderSettings()->setRedrawMode(false);

	glDisable(GL_BLEND);
	
}

void GameRenderer::textureProjection() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

}
void GameRenderer::bloomBlurPass() {

	glActiveTextureARB(GL_TEXTURE0);

	//Perform horizontal pass

	glBindFramebufferEXT(GL_FRAMEBUFFER, bloomBlurFBO);
	glBindTexture(GL_TEXTURE_2D, bloomColorTexture);

	renderer->getShaderManager()->enableShader(hBlurShaderIndex);
	renderer->getShaderManager()->getShaderParamSetter().setParamInt(
		Shader::ShaderParameter::UNIFORM,
		"viewWidth",
		renderer->getRenderSettings()->getBloomWidth());
	renderer->getShaderManager()->getShaderParamSetter().setStandardParamInt(
		Shader::ShaderParameter::UNIFORM,
		static_cast<int>(Shader::ShaderManager::UINT_COLOR_MAP),
		0);

	drawQuad();

	//Perform vertical pass

	glBindFramebufferEXT(GL_FRAMEBUFFER, bloomFBO);
	glBindTexture(GL_TEXTURE_2D, bloomBlurTexture);

	renderer->getShaderManager()->enableShader(vBlurShaderIndex);
	renderer->getShaderManager()->getShaderParamSetter().setParamInt(
		Shader::ShaderParameter::UNIFORM,
		"viewHeight",
		renderer->getRenderSettings()->getBloomHeight());
	renderer->getShaderManager()->getShaderParamSetter().setStandardParamInt(
		Shader::ShaderParameter::UNIFORM,
		static_cast<int>(Shader::ShaderManager::UINT_COLOR_MAP),
		0);

	drawQuad();

	renderer->getShaderManager()->disableShader();
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

void GameRenderer::applyBloomBuffer() {

	glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bloomColorTexture);

	OpenGL::Color::glColor(OpenGL::Color::WHITE);

	drawQuad();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

}

void GameRenderer::clearBloom() {
	glDeleteTextures(1, &bloomColorTexture);
	glDeleteTextures(1, &bloomDepthTexture);
	glDeleteTextures(1, &bloomBlurTexture);
	glDeleteFramebuffers(1, &bloomBlurFBO);
	glDeleteFramebuffers(1, &bloomFBO);

	delete bloomRenderable;
	bloomRenderable = NULL;
	delete bloomScene->getRenderProperties()->getMaterial();
	delete bloomScene;
	bloomScene = NULL;
	//delete bloomBackground;
	bloomBackground = NULL;
}
void GameRenderer::drawQuad() {

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);

	glEnd();

}

void GameRenderer::initShadowMap() {

	glGenTextures(1, &shadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);

	GLfloat border_color [4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		renderer->getRenderSettings()->getShadowMapWidth(),
		renderer->getRenderSettings()->getShadowMapHeight(),
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_BYTE,
		0);


	glGenFramebuffersEXT(1, &shadowFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER, shadowFBO);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		shadowDepthTexture,
		0);

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	shadowCamera = new OpenGL::Camera();
	shadowCamera->setFrustrumFarPlaneEnabled(true);

	shadowProperties = new Render::RenderProperties();
	shadowProperties->setShaderOverride(true);
	shadowProperties->setTexturePackOverride(true);
	shadowProperties->setMaterialOverride(true);
	shadowProperties->setColorOverride(true);

}

void GameRenderer::updateShadowCamera(const Math::Point& light_position, OpenGL::Camera* camera) {

	double far_plane = GET_SETTING("render.shadow.farplane", 100.0);

	shadowCamera->setPosition(light_position);
	shadowCamera->setLookPosition(camera->cameraToWorld(0.5, 0.5, far_plane*0.5));

	Math::Point up_dir;
	if (std::fabs(shadowCamera->getLookDirection().dotProduct(camera->getRightDirection())) >
		std::fabs(shadowCamera->getLookDirection().dotProduct(camera->getUpDirection())) ) {
		shadowCamera->setUpDirection(camera->getRightDirection());
	}
	else {
		shadowCamera->setUpDirection(camera->getUpDirection());
	}

	shadowFocusFrustrum.clear();
	shadowFocusFrustrum.push_back(camera->getPosition());
	shadowFocusFrustrum.push_back(camera->cameraToWorld(0.0, 0.0, far_plane));
	shadowFocusFrustrum.push_back(camera->cameraToWorld(1.0, 0.0, far_plane));
	shadowFocusFrustrum.push_back(camera->cameraToWorld(1.0, 1.0, far_plane));
	shadowFocusFrustrum.push_back(camera->cameraToWorld(0.0, 1.0, far_plane));

	double x_angle_min = 0.0;
	double y_angle_min = 0.0;
	double x_angle_max = 0.0;
	double y_angle_max = 0.0;

	for (unsigned int i = 0; i < shadowFocusFrustrum.size(); i++) {
		Math::Point to_vector = shadowFocusFrustrum[i]-shadowCamera->getPosition();
		double x = to_vector.dotProduct(shadowCamera->getRightDirection());
		double y = to_vector.dotProduct(shadowCamera->getUpDirection());
		double z = to_vector.dotProduct(shadowCamera->getLookDirection());
		double x_angle = std::atan(x/z);
		double y_angle = std::atan(y/z);
		if (i == 0) {
			x_angle_min = x_angle;
			x_angle_max = x_angle;
			y_angle_min = y_angle;
			y_angle_max = y_angle;
		}
		else {
			x_angle_min = Math::minimum(x_angle_min, x_angle);
			x_angle_max = Math::maximum(x_angle_max, x_angle);
			y_angle_min = Math::minimum(y_angle_min, y_angle);
			y_angle_max = Math::maximum(y_angle_max, y_angle);
		}
	}

#if 1
	double x_rotate = std::tan((x_angle_max+x_angle_min)*0.5);
	double y_rotate = std::tan((y_angle_max+y_angle_min)*0.5);
	shadowCamera->setLookPosition(
		shadowCamera->getPosition()+shadowCamera->getLookDirection()
		+shadowCamera->getRightDirection()*x_rotate
		);
	shadowCamera->setLookPosition(
		shadowCamera->getPosition()+shadowCamera->getLookDirection()
		+shadowCamera->getUpDirection()*y_rotate
		);
#endif

	bool near_plane_set = false;
	double shadow_near_plane = GAME_RENDERER_SHADOW_MIN_NEAR_PLANE;
	Math::BoundingBox3D map_bound = map->getMapBoundingBox();

	Math::Point shadow_cam_dir = shadowCamera->getLookDirection();

	if (!map_bound.pointInside(shadowCamera->getPosition())) {

		//Find the plane of the map bounding box closest to the shadow camera
		Math::Axis longest_axis = shadow_cam_dir.longestAxis();
		Math::Point plane_point;
		Math::Point plane_normal;
		double longest_axis_sign = Math::sign(shadow_cam_dir.getCoord(longest_axis));

		plane_normal.setCoord(-1.0*longest_axis_sign, longest_axis);
		if (longest_axis_sign > 0.0)
			plane_point.setCoord(map_bound.minCoord(longest_axis), longest_axis);
		else
			plane_point.setCoord(map_bound.maxCoord(longest_axis), longest_axis);

		for (unsigned int i = 0; i < 4; i++) {
			Math::Ray ray;
			switch (i) {
				case 0: ray = shadowCamera->cameraRay(0.0, 0.0); break;
				case 1: ray = shadowCamera->cameraRay(1.0, 0.0); break;
				case 2: ray = shadowCamera->cameraRay(1.0, 1.0); break;
				case 3: ray = shadowCamera->cameraRay(0.0, 1.0); break;
				default: ray = shadowCamera->cameraRay(0.5, 0.5); break;
			}
			Math::RayIntersection intersection = Math::Geometry::rayPlaneIntersection(ray, plane_point, plane_normal);
			if (intersection.intersects) {
				double intersect_near = intersection.t*(ray.direction.dotProduct(shadow_cam_dir));
				if (!near_plane_set) {
					shadow_near_plane = intersect_near;
					near_plane_set = true;
				}
				else {
					shadow_near_plane = Math::minimum(shadow_near_plane, intersect_near);
				}
			}
		}
	}

	double y_half_fov = 0.0;
    double x_half_fov = 0.0;

	double shadow_far_plane = 0.0;
	for (unsigned int i = 0; i < shadowFocusFrustrum.size(); i++) {
		Math::Point to_vector = shadowFocusFrustrum[i]-shadowCamera->getPosition();
		double x = to_vector.dotProduct(shadowCamera->getRightDirection());
		double y = to_vector.dotProduct(shadowCamera->getUpDirection());
		double z = to_vector.dotProduct(shadow_cam_dir);

		double x_angle = std::atan(x/z);
		double y_angle = std::atan(y/z);

		x_half_fov = Math::maximum(std::fabs(x_angle), x_half_fov);
		y_half_fov = Math::maximum(std::fabs(y_angle), y_half_fov);

		shadow_far_plane = Math::maximum(shadow_far_plane, z);
		if (z > GAME_RENDERER_SHADOW_MIN_NEAR_PLANE)
			shadow_near_plane = Math::minimum(shadow_near_plane, z);
	}

	shadowCamera->setNearPlane(shadow_near_plane);
	shadowCamera->setFarPlane(shadow_far_plane);
#if 0
	double y_fov = y_angle_max-y_angle_min;
    double x_fov = x_angle_max-x_angle_min;
	shadowCamera->setFieldOfViewDegrees(Math::radiansToDegrees(y_fov));
	shadowCamera->setAspect(std::tan(x_fov*0.5)/std::tan(y_fov*0.5));
#else
    double aspect = std::tan(x_half_fov)/std::tan(y_half_fov);
    shadowCamera->setFieldOfViewDegrees(2.0 * Math::radiansToDegrees(y_half_fov));
    shadowCamera->setAspect(aspect);
#endif
}

void GameRenderer::renderToShadowMap(Render::RenderableObject& renderable) {

	glViewport(0, 0,
		renderer->getRenderSettings()->getShadowMapWidth(), renderer->getRenderSettings()->getShadowMapHeight());

	glBindFramebufferEXT(GL_FRAMEBUFFER, shadowFBO);

	//Set up the camera and matrices
	shadowCamera->glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	shadowCamera->glLookAt();

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Save the matrices to the texture matrix

	const GLdouble bias[16] = {	
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0};

	double modelView[16];
	double projection[16];

	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	//Set the texture matrix
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(Render::RenderManager::SHADOW_MAP_TEXTURE_UNIT);
	glLoadIdentity();
	glLoadMatrixd(bias);
	glMultMatrixd(projection);
	glMultMatrixd(modelView);

	glMatrixMode(GL_MODELVIEW);

	//Cull front faces
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	renderer->setCamera(shadowCamera);
	renderer->setRenderProperties(shadowProperties);

	//Render the scene from the shadow camera
	renderable.render(renderer.get());

	renderer->revertRenderProperties(shadowProperties);

	//Revert to standard rendering settings
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);

}

void GameRenderer::clearShadowMap() {
	glDeleteTextures(1, &shadowDepthTexture);
	glDeleteFramebuffers(1, &shadowFBO);
	delete shadowCamera;
	delete shadowProperties;
}

void GameRenderer::handleGameStageChanged(Event::GameStageChanged* event) {
	if (event->getStage() == World::WorldSetup::RUNNING_GAME) {
		if (!clockStarted) {
			raceClock->addTimeOffset(World::TimeElapsed::getInstance().getGameTime());
			clockStarted = true;
		}
	}
}

}  // namespace SDL
}  // namespace Project
