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
	GLuint alphaMaskTexture;
	Math::Point map2DCenter;
	double halfMap2DWidth;
	double halfMap2DHeight;

	Render::Shader* alphaMaskShader;
	int alphaMaskLoc;
	int renderTextureLoc;
	int usingTextureLoc;
	int viewSizeLoc;

public:
	
	Minimap();
	~Minimap();
	void setMapInfo(HRMap* map);
	void drawMinimap(double minimap_view_height, double minimap_aspect,
		Math::Point focus_pos, Object::WorldManager* world,
		Paint::PaintManager* paint_manager, bool alpha_mask = true);
};

}  // namespace Map
}  // namespace Project

#endif
