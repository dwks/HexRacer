#ifndef PROJECT_SDL__GAME_RENDERER_H
#define PROJECT_SDL__GAME_RENDERER_H

#include <vector>

#include "boost/smart_ptr/shared_ptr.hpp"

#include "mesh/MeshLoader.h"
#include "render/RenderManager.h"
#include "render/LightManager.h"
#include "render/RenderList.h"
#include "render/BackgroundRenderable.h"
#include "render/StringTextureCache.h"

#include "FPSRateMonitor.h"

#include "paint/PaintManager.h"

#include "map/HRMap.h"
#include "map/MapLoader.h"
#include "map/RaceManager.h"

#include "hud/HUDRenderer.h"
#include "hud/Minimap.h"
#include "hud/Speedometer.h"
#include "hud/PlacingList.h"
#include "hud/PlayerPlacingText.h"
#include "hud/BonusMessages.h"
#include "hud/RaceClock.h"

#include "gui/GUISystem.h"
#include "widget/TextWidget.h"

#include "opengl/Camera.h"
#include "opengl/OpenGL.h"

#include "event/Enabler.h"
#include "event/GameStageChanged.h"

namespace Project {
namespace SDL {

#define GAME_RENDERER_SHADOW_MIN_NEAR_PLANE 0.1

class GameRenderer
	: public Event::Enabler {
private:

	GLuint bloomFBO;
	GLuint bloomColorTexture;
	GLuint bloomDepthTexture;
	Render::RenderList* bloomRenderable;
	Render::RenderParent* bloomBackground;
	Render::RenderParent* bloomScene;

	GLuint bloomBlurFBO;
	GLuint bloomBlurTexture;

	int hBlurShaderIndex;
	int vBlurShaderIndex;

	bool clockStarted;

	GLuint shadowFBO;
	GLuint shadowColorTexture;
	GLuint shadowDepthTexture;

	OpenGL::Camera* shadowCamera;
	Render::RenderProperties* shadowProperties;
	std::vector<Math::Point> shadowFocusFrustrum;

    boost::shared_ptr<Mesh::MeshLoader> meshLoader;
    boost::shared_ptr<Render::RenderManager> renderer;
    Render::LightManager *lightManager;  // not allocated here
    boost::shared_ptr<Render::RenderList> mapRenderable;
    boost::shared_ptr<Render::BackgroundRenderable> background;

	boost::shared_ptr<Render::StringTextureCache> stringTextureCache;
    
    boost::shared_ptr<Paint::PaintManager> paintManager;
    
    boost::shared_ptr<Map::HRMap> map;
	boost::shared_ptr<Map::MapLoader> mapLoader;
	boost::shared_ptr<HUD::HUDRenderer> hudRenderer;
    boost::shared_ptr<HUD::Minimap> minimap;
	boost::shared_ptr<HUD::Speedometer> speedometer;
	boost::shared_ptr<HUD::PlacingList> placingList;
	boost::shared_ptr<HUD::PlayerPlacingText> playerPlacingText;
	boost::shared_ptr<HUD::BonusMessages> bonusMessages;
	boost::shared_ptr<HUD::RaceClock> raceClock;
    
    boost::shared_ptr<GUI::GUISystem> gui;
    boost::shared_ptr<FPSRateMonitor> fpsRate;

public:
    ~GameRenderer();
    void construct(OpenGL::Camera *camera);
    
    void setGUI(boost::shared_ptr<GUI::GUISystem> gui);
    
    void render(OpenGL::Camera *camera, Object::WorldManager *worldManager);
	void renderHUD(Object::WorldManager *worldManager, Object::Player *player, Map::RaceManager *raceManager);
	void renderDebug(OpenGL::Camera *camera, Object::WorldManager *worldManager, Object::Player *player);
    
    Map::HRMap *getMap() { return map.get(); }
    Paint::PaintManager *getPaintManager() { return paintManager.get(); }
private:
    void renderAIDebug(Object::Player *player);
	void renderShadowDebug();
    void renderWorld(Object::World *world);

	void initBloom();
	void renderToBloomBuffer(Render::RenderableObject& renderable);
	void textureProjection();
	void bloomBlurPass();
	void applyBloomBuffer();
	void clearBloom();

	void drawQuad();

	void initShadowMap();
	void updateShadowCamera(const Math::Point& light_position, OpenGL::Camera* camera);
	void renderToShadowMap(Render::RenderableObject& renderable);
	void clearShadowMap();

	void handleGameStageChanged(Event::GameStageChanged* event);

};

}  // namespace SDL
}  // namespace Project

#endif
