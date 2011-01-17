#pragma once

#include "math/Point.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace OpenGL {

class SimpleTrackball
{
private:
	float xScale;
	float yScale;
	float longitude;
	float latitude;
	Math::Point lastPoint;
	Math::Point spherePoint;

	void updateSpherePoint();

public:
	SimpleTrackball(float x_scale = 1.0f, float y_scale = 1.0f);
	~SimpleTrackball(void);

	void setMouseStartAt(Point point);
	void setMouseCurrentAt(Point point);
	Point getSpherePoint();

};

}  // namespace OpenGL
}  // namespace Project
