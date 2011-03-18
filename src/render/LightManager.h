#ifndef PROJECT_RENDER__LIGHT_MANAGER_H
#define PROJECT_RENDER__LIGHT_MANAGER_H

#include "opengl/Light.h"
#include "opengl/OpenGL.h"
#include "math/BSPTree3D.h"
#include "LightManagerNode.h"
#include <vector>

#define LIGHT_MANAGER_DRAW_LIGHT_SCALE 0.1f

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

	void activateStaticLights(Math::BSPTree3D* light_tree, const Math::BoundingObject& object);
	void activateDynamicLights(std::vector<LightManagerNode*>& lights, const Math::BoundingObject& object);
	void activateLight(LightManagerNode* light_node);
	void addStaticLight(Math::BSPTree3D* light_tree, LightManagerNode* light_node);

public:
	LightManager(int max_active_lights = 8);
	~LightManager();

	void addLight(OpenGL::Light* light, bool high_priorty = false, bool dynamic = true);
	void drawActiveLightSpheres(bool show_influence_radius = false);
	void setSceneBoundingBox(Math::BoundingBox3D bounding_box);
	void activateNearFocalPoint(Math::Point focal_point, double radius = 0.0);
	void activateIntersectingLights(const Math::BoundingObject& bounding_obj);
	void resetLights();
	void clear();
	OpenGL::Light* getActiveLight(int index) const;

	void reapplyActiveLights();

	int getNumActiveLights() const { return activeLights.size(); }
};

}  // namespace Render
}  // namespace Project

#endif
