#ifndef PROJECT_RENDER__LIGHT_MANAGER_H
#define PROJECT_RENDER__LIGHT_MANAGER_H

#include "Light.h"
#include "opengl/OpenGL.h"
#include <vector>

#define LIGHT_MANAGER_DRAW_LIGHT_SCALE 0.25f

namespace Project {
namespace Render {

class LightManager {
private:
	std::vector<Light*> lights;
	int maxLights;
	int activeLights;
	GLuint sphereID;

public:
	LightManager(int max_lights = 8);

	void addLight(Light* light);
	bool removeLight(Light* light);
	void applyAll();
	//void unapplyAll();
	void drawLightSpheres();
	//void applyNearest();

	int getActiveLights() const { return activeLights; }
};

}  // namespace Render
}  // namespace Project

#endif
