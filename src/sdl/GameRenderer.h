#ifndef PROJECT_SDL__GAME_RENDERER_H
#define PROJECT_SDL__GAME_RENDERER_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "render/MeshLoader.h"
#include "render/RenderManager.h"
#include "render/LightManager.h"
#include "render/RenderList.h"
#include "render/BackgroundRenderable.h"

#include "paint/PaintManager.h"

#include "map/HRMap.h"
#include "map/Minimap.h"

#include "opengl/Camera.h"

namespace Project {
namespace SDL {

class GameRenderer {
private:
    boost::shared_ptr<Render::MeshLoader> meshLoader;
    boost::shared_ptr<Render::RenderManager> renderer;
    Render::LightManager *lightManager;  // not allocated here
    boost::shared_ptr<Render::RenderList> mapRenderable;
    boost::shared_ptr<Render::BackgroundRenderable> background;
    
    boost::shared_ptr<Paint::PaintManager> paintManager;
    
    boost::shared_ptr<Map::HRMap> map;
    boost::shared_ptr<Map::Minimap> minimap;
public:
    void construct(OpenGL::Camera *camera);
    
    void render(OpenGL::Camera *camera, Object::World *world);
    
    Map::HRMap *getMap() { return map.get(); }
    Paint::PaintManager *getPaintManager() { return paintManager.get(); }
private:
    void renderAIDebug();
    void renderWorld(Object::World *world);
};

}  // namespace SDL
}  // namespace Project

#endif
