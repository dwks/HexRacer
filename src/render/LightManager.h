#ifndef PROJECT_RENDER__LIGHT_MANAGER_H
#define PROJECT_RENDER__LIGHT_MANAGER_H

#include "Light.h"
#include "opengl/OpenGL.h"
#include "math/BSPTree3D.h"
#include "LightManagerNode.h"
#include <vector>

#define LIGHT_MANAGER_DRAW_LIGHT_SCALE 0.1f
#define LIGHT_MANAGER_ATTENUATION_THRESHHOLD 0.01

namespace Project {
namespace Render {

class LightManager {
private:

	Math::BSPTree3D* priorityStaticLightTree;
	Math::BSPTree3D* staticLightTree;
	std::vector<LightManagerNode*> priorityDynamicLights;
	std::vector<LightManagerNode*> dynamicLights;
	std::vector<LightManagerNode*> activeLights;
	unsigned int maxActiveLights;
	GLuint sphereID;

	void activateStaticLights(Math::BSPTree* light_tree, const Math::BoundingObject& object);
	void activateDynamicLights(std::vector<LightManagerNode*>& lights, const Math::BoundingObject& object);
	void activateLight(LightManagerNode* light_node);

public:
	LightManager(int max_active_lights = 8);

	void addLight(Light* light, bool high_priorty = false, bool dynamic = false);
	bool removeLight(Light* light, bool high_priorty = false, bool dynamic = false);
	void drawActiveLightSpheres();
	void setSceneBoundingBox(Math::BoundingBox3D bounding_box);
	void activateNearFocalPoint(Math::Point focal_point, double radius = 0.0);
	void activateIntersectingLights(const Math::BoundingObject& bounding_obj);
	void resetLights();

	int getNumActiveLights() const { return activeLights.size(); }
};

}  // namespace Render
}  // namespace Project

#endif
