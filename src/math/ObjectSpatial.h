#pragma once

#include "Point.h"

namespace Project {
namespace Math {

class BoundingObject;

class ObjectSpatial
{
public:
	virtual ~ObjectSpatial(void) {}

	virtual double minX() = 0;
	virtual double minY() = 0;
	virtual double minZ() = 0;
	virtual double maxX() = 0;
	virtual double maxY() = 0;
	virtual double maxZ() = 0;

	double widthX() { return maxX() - minX(); }
	double widthY() { return maxY() - minY(); }
	double widthZ() { return maxZ() - minZ(); }

	double minCoord(Axis axis);
	double maxCoord(Axis axis);
	double widthCoord(Axis axis);

	virtual Point centroid() = 0;
	virtual void translate(Point& translation) = 0;
	virtual void moveCentroid(Point& position) = 0;
	virtual bool isInside(BoundingObject& bounding_obj) = 0;
	virtual bool intersects(BoundingObject& bound_obj) = 0;

};

}  // namespace Math
}  // namespace Project