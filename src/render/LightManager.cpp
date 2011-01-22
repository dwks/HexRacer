#include "LightManager.h"
#include "misc/StdVectorFunctions.h"
#include <math.h>
using namespace Project;
using namespace Math;
using namespace OpenGL;

namespace Project {
namespace Render {

	LightManager::LightManager(int max_lights) {

		maxActiveLights = max_lights;
		//Generate the light sphere
		GLUquadric* quadric = gluNewQuadric();
		sphereID = glGenLists(1);
		glNewList(sphereID, GL_COMPILE);
		gluSphere(quadric, 1.0f, 18, 18);
		glEndList();
		gluDeleteQuadric(quadric);

		BoundingBox3D bounding_box(100.0, 100.0, 100.0);
		priorityStaticLightTree = new BSPTree3D(bounding_box);
		staticLightTree = new BSPTree3D(bounding_box);
	}

	void LightManager::addLight(Light* light, bool high_priorty, bool dynamic) {
		if (dynamic) {
			if (high_priorty)
				priorityDynamicLights.push_back(new LightManagerNode(light));
			else
				dynamicLights.push_back(new LightManagerNode(light));
		}
		else {
			if (high_priorty)
				priorityStaticLightTree->add(new LightManagerNode(light));
			else
				staticLightTree->add(new LightManagerNode(light));
		}
	}

	bool LightManager::removeLight(Light* light, bool high_priorty, bool dynamic) {
		/*
		if (dynamic) {
			if (high_priorty)
				return Misc::vectorRemoveOneElement(priorityDynamicLights, light);
			else {
				return Misc::vectorRemoveOneElement(dynamicLights, light);
			}
		}
		else {
			if (high_priorty)
				return priorityStaticLightTree->remove(light);
			else
				return staticLightTree->remove(light);
		}
		*/
		return false;
	}

	void LightManager::drawActiveLightSpheres() {

		for (unsigned int i = 0; i < activeLights.size(); i++) {

			Light* light = ((LightManagerNode*)activeLights[i])->light;

			glPushMatrix();

			Color::glColor(light->getDiffuse());

			Math::Point light_pos = light->getPosition();
			float radius = LIGHT_MANAGER_DRAW_LIGHT_SCALE;
			glTranslatef(light_pos.getX(), light_pos.getY(), light_pos.getZ());
			glScalef(radius, radius, radius);
			glCallList(sphereID);

			glPopMatrix();
		}

	}

	void activateIntersectingLights(Math::BoundingObject& bounding_obj);

	void LightManager::activateStaticLights(Math::BSPTree* light_tree, const Math::BoundingObject& object) {
		if (activeLights.size() >= maxActiveLights)
			return;

		vector<ObjectSpatial*> visible_lights = light_tree->query(object, SpatialContainer::INTERSECT);
		for (unsigned int i = 0; i < visible_lights.size() && activeLights.size() < maxActiveLights; i++) {
			LightManagerNode* light_node = (LightManagerNode*) visible_lights[i];
			if (!light_node->active) {
				activateLight(light_node);
			}
		}
	}

	void LightManager::activateDynamicLights(std::vector<LightManagerNode*>& lights, const Math::BoundingObject& object) {
		if (activeLights.size() >= maxActiveLights)
			return;

		for (unsigned int i = 0; i < lights.size() && activeLights.size() < maxActiveLights; i++) {
			LightManagerNode* light_node = (LightManagerNode*) lights.at(i);
			if (!light_node->active) {
				light_node->update();
				if (light_node->intersects(object)) {
					activateLight(light_node);
				}
			}
		}
	}

	void LightManager::setSceneBoundingBox(Math::BoundingBox3D bounding_box) {
		priorityStaticLightTree->resize(bounding_box);
		staticLightTree->resize(bounding_box);
	}

	void LightManager::activateLight(LightManagerNode* light_node) {
		if (!light_node->active) {
			light_node->light->glApply(GL_LIGHT0+activeLights.size());
			light_node->active = true;
			activeLights.push_back(light_node);
		}
	}

	void LightManager::activateNearFocalPoint(Point focal_point, double radius) {
		activateIntersectingLights(BoundingSphere(focal_point, radius));
	}

	void LightManager::activateIntersectingLights(const Math::BoundingObject& bounding_obj) {
		activateDynamicLights(priorityDynamicLights, bounding_obj);
		activateStaticLights(priorityStaticLightTree, bounding_obj);
		activateDynamicLights(dynamicLights, bounding_obj);
		activateStaticLights(staticLightTree, bounding_obj);
	}

	void LightManager::resetLights() {
		for (unsigned int i = 0; i < activeLights.size(); i++) {
			activeLights[i]->active = false;
			glDisable(GL_LIGHT0+i);
		}
		activeLights.clear();
	}

}  // namespace Render
}  // namespace Project
