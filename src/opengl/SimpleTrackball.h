#pragma once

#include "math/Point.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace OpenGL {

class SimpleTrackball
{
private:
	double xScale;
	double yScale;
	double longitude;
	double latitude;
	Math::Point lastPoint;
	Math::Point spherePoint;

	void updateSpherePoint();

public:
	SimpleTrackball(double x_scale = 1.0f, double y_scale = 1.0f);
	~SimpleTrackball(void);

	void setMouseStartAt(Point point);
	void setMouseCurrentAt(Point point);
	Point getSpherePoint();
	void setSpherePoint(Point point);
	void setLongitude(double _longitude);
	void setLatitude(double _latitude);

};

}  // namespace OpenGL
}  // namespace Project
