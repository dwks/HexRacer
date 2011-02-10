#ifndef PROJECT_RENDER__LIGHT_MANAGER_NODE_H
#define PROJECT_RENDER__LIGHT_MANAGER_NODE_H

#include "opengl/Light.h"
#include "math/BoundingSphere.h"

namespace Project {
namespace Render {

class LightManagerNode 
	: public Math::BoundingSphere {
private:
	float prevQuadAttenuation;
public:
	OpenGL::Light* light;
	bool active;
	float factorAtPoint;

	LightManagerNode(OpenGL::Light* _light);
	void update();
	void setLightFactorPoint(const Math::Point& point);

	bool operator < (const ObjectSpatial &other);
	bool operator < (const LightManagerNode &other);
};

}  // namespace Render
}  // namespace Project

#endif
