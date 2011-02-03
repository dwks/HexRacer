#ifndef PROJECT_MAP__MINIMAP_H
#define PROJECT_MAP__MINIMAP_H

#include "opengl/Camera.h"
#include "paint/PaintManager.h"
#include "object/WorldManager.h"
#include "HRMap.h"

namespace Project {
namespace Map {

class Minimap {
private:

	OpenGL::Camera camera;

	GLuint trackTexture;
	Math::Point map2DCenter;
	double halfMap2DWidth;
	double halfMap2DHeight;

public:
	
	Minimap();
	~Minimap();
	void setMapInfo(HRMap* map);
	void drawMinimap(double minimap_view_height, double minimap_aspect,
		Math::Point focus_pos, Object::WorldManager* world,
		Paint::PaintManager* paint_manager);
};

}  // namespace Map
}  // namespace Project

#endif
