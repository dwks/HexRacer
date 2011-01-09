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

	double minZ() { return 0.0f; }
	double maxZ() { return 0.0f; }
	Axis getProjectAxis() { return projectAxis; }

	bool intersects(BoundingObject& bound_obj);
	void translate(Point& translation);
	void moveCentroid(Point& position);

	virtual void translate2D(Point2D& translation) = 0;
	virtual bool intersects2D(BoundingObject2D& bound_obj) = 0;


};

}  // namespace Math
}  // namespace Project