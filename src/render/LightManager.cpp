#include "LightManager.h"
#include "opengl/GeometryDrawing.h"
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
		priorityStaticLightTree = new BSPTree3D(bounding_box, BSPTree3D::FAIR_XSTART, 8);
		staticLightTree = new BSPTree3D(bounding_box);
	}

	void LightManager::addLight(OpenGL::Light* light, bool high_priorty, bool dynamic) {

		LightManagerNode* new_node = new LightManagerNode(light);
		if (dynamic) {
			if (high_priorty)
				priorityDynamicLights.push_back(new_node);
			else
				dynamicLights.push_back(new_node);
		}
		else {
			if (high_priorty)
				addStaticLight(priorityStaticLightTree, new_node);
			else
				addStaticLight(staticLightTree, new_node);
		}
	}

	void LightManager::drawActiveLightSpheres(bool show_influence_radius) {

		for (unsigned int i = 0; i < activeLights.size(); i++) {

			Light* light = ((LightManagerNode*)activeLights[i])->light;

			glPushMatrix();

			Color::glColor(light->getDiffuse());

			Math::Point light_pos = light->getPosition();
			float radius = LIGHT_MANAGER_DRAW_LIGHT_SCALE;
			glTranslatef(light_pos.getX(), light_pos.getY(), light_pos.getZ());
			glScalef(radius, radius, radius);

			glCallList(sphereID);

			if (show_influence_radius) {
				radius = activeLights[i]->getRadius()/radius;
				glScalef(radius, radius, radius);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glCallList(sphereID);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glPopMatrix();
		}

	}

	void LightManager::activateStaticLights(Math::BSPTree3D* light_tree, const Math::BoundingObject& object) {
		if (activeLights.size() >= maxActiveLights)
			return;

		vector<ObjectSpatial*> visible_lights;
		light_tree->appendQuery(visible_lights, object, SpatialContainer::INTERSECT);

		if (visible_lights.empty())
			return;

		Point light_factor_point;
		if (object.is2D())
			light_factor_point = ((const BoundingObject2D&)object).centroid();
		else
			light_factor_point = ((const BoundingObject3D&)object).centroid();

		//Sort the lights by ascending strength at the centroid of the bounding object
		for (unsigned int i = 0; i < visible_lights.size(); i++) {
			((LightManagerNode*)visible_lights[i])->setLightFactorPoint(light_factor_point);
		}
		Misc::vectorPointerMergeSort(visible_lights);

		//Activate the lights in descending order
		for (int i = visible_lights.size()-1; i >= 0 && activeLights.size() < maxActiveLights; i--) {
			LightManagerNode* light_node = (LightManagerNode*) visible_lights[i];
			if (!light_node->active) {
				activateLight(light_node);
			}
		}
	}

	void LightManager::activateDynamicLights(std::vector<LightManagerNode*>& lights, const Math::BoundingObject& object) {
		if (activeLights.size() >= maxActiveLights || lights.empty())
			return;

		Point light_factor_point;
		if (object.is2D())
			light_factor_point = ((const BoundingObject2D&)object).centroid();
		else
			light_factor_point = ((const BoundingObject3D&)object).centroid();

		//Sort the lights by ascending strength at the centroid of the bounding object
		for (unsigned int i = 0; i < lights.size(); i++) {
			((LightManagerNode*)lights[i])->setLightFactorPoint(light_factor_point);
		}
		Misc::vectorPointerMergeSort(lights);

		//Activate the lights in descending order
		for (int i = lights.size()-1; i >= 0 && activeLights.size() < maxActiveLights; i--) {
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
		if (activeLights.size() >= maxActiveLights)
			return;
		activateStaticLights(priorityStaticLightTree, bounding_obj);
		activateDynamicLights(priorityDynamicLights, bounding_obj);
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

	void LightManager::clear() {

		resetLights();
		
		for (unsigned int i = 0; i < priorityDynamicLights.size(); i++)
			delete(priorityDynamicLights[i]);
		for (unsigned int i = 0; i < dynamicLights.size(); i++)
			delete(dynamicLights[i]);
		vector<ObjectSpatial*> static_list = priorityStaticLightTree->all();
		for (unsigned int i = 0; i < static_list.size(); i++)
			delete(static_list[i]);
		static_list = staticLightTree->all();
		for (unsigned int i = 0; i < static_list.size(); i++)
			delete(static_list[i]);
	
		priorityDynamicLights.clear();
		dynamicLights.clear();
		priorityStaticLightTree->clear();
		staticLightTree->clear();

	}

	void LightManager::addStaticLight(Math::BSPTree3D* light_tree, LightManagerNode* light_node) {

		BoundingBox3D light_bounding_box = BoundingBox3D((const BoundingBox3D&)light_tree->getBoundingObject());
		if (!light_node->isInside(light_bounding_box)) {
			light_bounding_box.expandToInclude(*light_node);
			light_tree->resize(light_bounding_box);
		}

		light_tree->add(light_node);

	}

	OpenGL::Light* LightManager::getActiveLight(int index) const {
		if (index < 0 || index >= static_cast<int>(activeLights.size()))
			return NULL;
		else
			return activeLights[index]->light;
	}

	void LightManager::reapplyActiveLights() {
		for (unsigned int i = 0; i < activeLights.size(); i++) {
			activeLights[i]->light->glApply(GL_LIGHT0+i);
		}
	}


}  // namespace Render
}  // namespace Project
