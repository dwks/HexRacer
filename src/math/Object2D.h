#pragma once

//#include "Point2D.h"
#include "ObjectSpatial.h"

namespace Project {
namespace Math {

class BoundingObject2D;

/** Returns an object that exists on a two dimensional plane on the X,Y or Z axis
*/
class Object2D :
	public ObjectSpatial
{
protected:
	Axis projectAxis;
public:
	Object2D(Axis projectAxis = Z_AXIS)
		: projectAxis(projectAxis) {}
	virtual ~Object2D(void) {}

	Axis getProjectAxis() const { return projectAxis; }

	bool intersects(const BoundingObject& bound_obj) const;
	void moveCentroid(const Point& position);
	bool isInside(const BoundingObject& bounding_obj) const;

	virtual bool intersects2D(const BoundingObject2D& bound_obj) const = 0;
	virtual bool isInside2D(const BoundingObject2D& bounding_obj) const = 0;

	double minU() const;
	double minV() const;
	double maxU() const;
	double maxV() const;

	double widthU() const { return maxU()-minU(); }
	double widthV() const { return maxV()-minV(); }

};

}  // namespace Math
}  // namespace Project