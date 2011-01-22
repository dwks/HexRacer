#include "LightManagerNode.h"
#include <math.h>

namespace Project {
namespace Render {

	LightManagerNode::LightManagerNode(Light* _light) {
		light = _light;
		active = false;
		update();
	}

	void LightManagerNode::update() {

		//Set the radius to where the light influence passes below the threshhold
		if (light->getHasAttenuation())
			setRadius(9999999999999999.0f);
		else
			setRadius(sqrt( (1.0/LIGHT_MANAGER_MIN_ATTENUATION_THRESHHOLD - 1.0) / light->getQuadraticAttenuation() ));

		moveCentroid(light->getPosition());
	}

}  // namespace Render
}  // namespace Project
