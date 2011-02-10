#include "LightManagerNode.h"
#include <math.h>
#include <float.h>
#include <typeinfo>
#include "config.h"

namespace Project {
namespace Render {

	LightManagerNode::LightManagerNode(OpenGL::Light* _light) {
		light = _light;
		active = false;
		prevQuadAttenuation = FLT_MAX;
		update();
	}

	void LightManagerNode::update() {

		//Set the radius to where the light influence passes below the threshhold
		if (!light->getHasAttenuation()) {
			setRadius(FLT_MAX);
			prevQuadAttenuation = light->getQuadraticAttenuation();
		}
		else {
			if (light->getQuadraticAttenuation() != prevQuadAttenuation) {
				double radius = sqrt( (1.0/LIGHT_MANAGER_MIN_ATTENUATION_THRESHHOLD - light->getConstantAttenuation()) / light->getQuadraticAttenuation() );
				setRadius(radius);
				prevQuadAttenuation = light->getQuadraticAttenuation();
			}
		}

		moveCentroid(light->getPosition());
	}

	void LightManagerNode::setLightFactorPoint(const Math::Point& point) {
		if (light->getHasAttenuation()) {
			factorAtPoint = 1.0f/(
				light->getConstantAttenuation() +
				light->getQuadraticAttenuation()*light->getPosition().distanceSquared(point)
				);
		}
		else {
			factorAtPoint = 1.0f;
		}
	}

	bool LightManagerNode::operator < (const ObjectSpatial &other) {
		if (typeid(other) == typeid(LightManagerNode&))
			return *this < (const LightManagerNode&) other;
		else
			return true;
	}

	bool LightManagerNode::operator < (const LightManagerNode &other) {
		return factorAtPoint < other.factorAtPoint;
	}

}  // namespace Render
}  // namespace Project
