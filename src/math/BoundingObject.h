#pragma once

#include "Point.h"

namespace Project {
namespace Math {

class BoundingObject2D;

class BoundingObject {
public:
	BoundingObject(void) {}
	virtual ~BoundingObject(void) {}

	virtual bool pointInside(Point* p) = 0;
	//virtual bool intersects(BoundingObject* bound_obj) = 0;

	bool pointInside(Point p) { return pointInside(&p); }
};

}  // namespace Math
}  // namespace Project