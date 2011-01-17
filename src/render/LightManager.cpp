#include "LightManager.h"
#include <math.h>
using namespace Project;
using namespace OpenGL;

namespace Project {
namespace Render {

	LightManager::LightManager(int max_lights) {
		activeLights = 0;
		maxLights = max_lights;
		//Generate the light sphere
		GLUquadric* quadric = gluNewQuadric();
		sphereID = glGenLists(1);
		glNewList(sphereID, GL_COMPILE);
		gluSphere(quadric, 1.0f, 18, 18);
		glEndList();
		gluDeleteQuadric(quadric);
		
	}

	void LightManager::addLight(Light* light) {
		lights.push_back(light);
	}

	bool LightManager::removeLight(Light* light) {
		for (unsigned int i = 0; i < lights.size(); i++) {
			if (lights[i] == light) {
				lights[i] = lights[lights.size()-1];
				lights.resize(lights.size()-1);
				return true;
			}
		}
		return false;
	}

	void LightManager::applyAll() {
		activeLights = 0;
		for (unsigned int i = 0; i < lights.size(); i++) {
			if (i >= static_cast<unsigned int>(maxLights))
				return;
			lights[i]->glApply(GL_LIGHT0+i);
			activeLights++;
		}
	}

	void LightManager::drawLightSpheres() {
		for (unsigned int i = 0; i < lights.size(); i++) {
			glPushMatrix();

			Color::glColor(lights[i]->getDiffuse());

			Math::Point light_pos = lights[i]->getPosition();
			//float radius = sqrt(lights[i]->getStrength())*LIGHT_MANAGER_DRAW_LIGHT_SCALE;
			float radius = LIGHT_MANAGER_DRAW_LIGHT_SCALE;
			glTranslatef(light_pos.getX(), light_pos.getY(), light_pos.getZ());
			glScalef(radius, radius, radius);
			glCallList(sphereID);

			glPopMatrix();
		}
	}
	//void activateNearest();

}  // namespace Render
}  // namespace Project
