#include "LightManagerNode.h"
#include <math.h>
#include <float.h>

namespace Project {
namespace Render {

	LightManagerNode::LightManagerNode(Light* _light) {
		light = _light;
		active = false;
		prevQuadAttenuation = FLT_MAX;
		update();
	}

	void LightManagerNode::update() {

		//Set the radius to where the light influence passes below the threshhold
		if (!light->getHasAttenuation()) {
			setRadius(FLT_MAX);
		}
		else {
			if (light->getQuadraticAttenuation() != prevQuadAttenuation) {
				double radius = sqrt( (1.0/LIGHT_MANAGER_MIN_ATTENUATION_THRESHHOLD - 1.0) / light->getQuadraticAttenuation() );
				setRadius(radius);
				prevQuadAttenuation = light->getQuadraticAttenuation();
			}
		}

		moveCentroid(light->getPosition());
	}

}  // namespace Render
}  // namespace Project
