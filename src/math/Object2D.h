#pragma once

#include "Point2D.h"
#include "ObjectSpatial.h"

namespace Project {
namespace Math {

class BoundingObject2D;

class Object2D :
	public ObjectSpatial
{
protected:
	Axis projectAxis;
public:
	Object2D(Axis projectAxis = Z_AXIS)
		: projectAxis(projectAxis) {}
	virtual ~Object2D(void) {}

	Axis getProjectAxis() { return projectAxis; }

	bool intersects(BoundingObject& bound_obj);
	void moveCentroid(Point& position);

	virtual bool intersects2D(BoundingObject2D& bound_obj) = 0;
	virtual double minU() = 0;
	virtual double minV() = 0;
	virtual double maxU() = 0;
	virtual double maxV() = 0;

	double widthU() { return maxU()-minU(); }
	double widthV() { return maxV()-minV(); }

};

}  // namespace Math
}  // namespace Project