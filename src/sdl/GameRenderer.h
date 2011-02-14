#ifndef PROJECT_SDL__GAME_RENDERER_H
#define PROJECT_SDL__GAME_RENDERER_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "mesh/MeshLoader.h"
#include "render/RenderManager.h"
#include "render/LightManager.h"
#include "render/RenderList.h"
#include "render/BackgroundRenderable.h"

#include "paint/PaintManager.h"

#include "map/HRMap.h"

#include "hud/HUDRenderer.h"
#include "hud/Minimap.h"
#include "hud/Speedometer.h"

#include "gui/GUISystem.h"
#include "widget/TextWidget.h"

#include "opengl/Camera.h"
#include "opengl/OpenGL.h"

namespace Project {
namespace SDL {

class GameRenderer {
private:

	GLuint bloomFBO;
	GLuint bloomColorTexture;
	GLuint bloomDepthTexture;
	Render::RenderProperties* bloomProperties;

	GLuint bloomBlurFBO;
	GLuint bloomBlurTexture;

	int hBlurShaderIndex;
	int vBlurShaderIndex;

	GLuint shadowFBO;
	GLuint shadowColorTexture;
	GLuint shadowDepthTexture;

	OpenGL::Camera* shadowCamera;
	Render::RenderProperties* shadowProperties;

    boost::shared_ptr<Mesh::MeshLoader> meshLoader;
    boost::shared_ptr<Render::RenderManager> renderer;
    Render::LightManager *lightManager;  // not allocated here
    boost::shared_ptr<Render::RenderList> mapRenderable;
    boost::shared_ptr<Render::BackgroundRenderable> background;
    
    boost::shared_ptr<Paint::PaintManager> paintManager;
    
    boost::shared_ptr<Map::HRMap> map;
	boost::shared_ptr<HUD::HUDRenderer> hudRenderer;
    boost::shared_ptr<HUD::Minimap> minimap;
	boost::shared_ptr<HUD::Speedometer> speedometer;
    
    boost::shared_ptr<GUI::GUISystem> gui;
    Widget::TextWidget *percentageComplete;

public:
    void construct(OpenGL::Camera *camera);
    
    void setGUI(boost::shared_ptr<GUI::GUISystem> gui);
    
    void render(OpenGL::Camera *camera, Object::World *world);
    void renderHUD(Object::WorldManager *worldManager, Object::Player *player);
	void renderDebug(OpenGL::Camera *camera, Object::WorldManager *worldManager, Object::Player *player);
    
    Map::HRMap *getMap() { return map.get(); }
    Paint::PaintManager *getPaintManager() { return paintManager.get(); }
private:
    void renderAIDebug(Object::Player *player);
    void renderWorld(Object::World *world);

	void initBloom();
	void renderToBloomBuffer(Render::RenderableObject& renderable);
	void preBloomBlur();
	void bloomBlurPass();
	void applyBloomBuffer();
	void drawQuad();

	void initShadowMap();
	void updateShadowCamera(const Math::Point& light_position, OpenGL::Camera* camera);
	void renderToShadowMap(Render::RenderableObject& renderable);

};

}  // namespace SDL
}  // namespace Project

#endif
