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

void GameRenderer::construct(OpenGL::Camera *camera)
{
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

	if (renderer->getRenderSettings()->getBloomEnabled())
		initBloom();
}

void GameRenderer::render(OpenGL::Camera *camera, Object::World *world) {
    
    //Init Rendering----------------------------------------------------------------------

	world->preRender();

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render the paint
	paintManager->setFadePlanes((float) (near_plane+(paint_far_plane-near_plane)*0.75), (float) paint_far_plane);
    paintManager->render(renderer.get());

	//Revert Rendering Settings
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (renderer->getRenderSettings()->getBloomEnabled()) {

		Render::RenderList bloom_render_list;
		bloom_render_list.addRenderable(world->getRenderableObject());
		bloom_render_list.addRenderable(mapRenderable.get());
		bloom_render_list.addRenderable(paintManager.get());

		//Render to the bloom buffer
		camera->setFarPlane(GET_SETTING("render.bloom.farplane", 50.0));
		renderToBloomBuffer(bloom_render_list);
		preBloomBlur();
		int blur_passes = GET_SETTING("render.bloom.blurpasses", 5);
		for (int i = 0; i < blur_passes; i++)
			bloomBlurPass();
		applyBloomBuffer();

	}

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
		/*
        percentageComplete->setText(Misc::StreamAsString() << std::setprecision(3)
            << player->getPathTracker()->getLapProgress() * 100 << "%");
		*/

    }

	hudRenderer->renderingStateReset();
	hudRenderer->resetViewport();

}

void GameRenderer::setGUI(boost::shared_ptr<GUI::GUISystem> gui) {

    this->gui = gui;
    
	/*
    percentageComplete = new Widget::TextWidget(
        "percentageComplete", OpenGL::Color::WHITE, "0%",
        Widget::NormalTextLayout::ALIGN_HCENTRE | Widget::NormalTextLayout::ALIGN_VCENTRE);
    percentageComplete->updateLayout(Widget::WidgetRect(0.0, 0.9, 0.1, 0.05));
    /*percentageComplete = new Widget::ButtonWidget("percentageComplete",
        "0%", Widget::WidgetRect(0.0, 0.1, 0.1, 0.05));*/
    //gui->getScreen("running")->addChild(percentageComplete);
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

	camera->glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->glLookAt();

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

	bloomProperties = new Render::RenderProperties();
	bloomProperties->setWantsShaderName("bloomShader");
	bloomProperties->setShaderOverride(true);
	OpenGL::Material* default_material = new OpenGL::Material("bloomDefault");
	default_material->setAmbient(OpenGL::Color::BLACK);
	bloomProperties->setMaterial(default_material);

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

	renderer->getRenderSettings()->setRedrawMode(true);
	renderer->setRenderProperties(bloomProperties);
	renderable.render(renderer.get());
	renderer->revertRenderProperties(bloomProperties);
	renderer->getRenderSettings()->setRedrawMode(false);
	
}

void GameRenderer::preBloomBlur() {

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

}  // namespace SDL
}  // namespace Project
