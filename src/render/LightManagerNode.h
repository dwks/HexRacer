#ifndef PROJECT_RENDER__LIGHT_MANAGER_NODE_H
#define PROJECT_RENDER__LIGHT_MANAGER_NODE_H

#include "Light.h"
#include "math/BoundingSphere.h"

#define LIGHT_MANAGER_MIN_ATTENUATION_THRESHHOLD 0.001

namespace Project {
namespace Render {

class LightManagerNode 
	: public Math::BoundingSphere {
public:
	Light* light;
	bool active;

	LightManagerNode(Light* _light);
	void update();
};

}  // namespace Render
}  // namespace Project

#endif
