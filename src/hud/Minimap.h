#ifndef PROJECT_MAP__MINIMAP_H
#define PROJECT_MAP__MINIMAP_H

#include "opengl/Camera.h"
#include "paint/PaintManager.h"
#include "object/WorldManager.h"
#include "map/HRMap.h"
#include "HUDRenderer.h"
#include "math/BoundingCircle.h"

namespace Project {
namespace HUD {

class Minimap {
private:

	OpenGL::Camera camera;

	GLuint trackTexture;
	GLuint alphaMaskTexture;
	Math::Point map2DCenter;
	double halfMap2DWidth;
	double halfMap2DHeight;

	double mapViewHeight;
	Object::WorldManager* world;
	Paint::PaintManager* paintManager;

	Math::BoundingCircle boundingCircle;

public:
	
	Minimap();
	~Minimap();
	void setMapInfo(Map::HRMap* map);
	void setDrawInfo(double minimap_view_height);
	void setViewInfo(Math::Point view_position, Math::Point view_direction);
	void setWorldInfo(Object::WorldManager* _world, Paint::PaintManager* paint_manager);
	void render(HUDRenderer* renderer);
};

}  // namespace HUD
}  // namespace Project

#endif
