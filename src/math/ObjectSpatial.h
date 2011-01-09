#pragma once

#include "Point.h"
#include "BoundingObject.h"

namespace Project {
namespace Math {

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

	virtual Point centroid() = 0;
	virtual bool isInside(BoundingObject* bounding_obj) = 0;
	virtual bool intersects(BoundingObject* bound_obj) = 0;
};

}  // namespace Math
}  // namespace Project