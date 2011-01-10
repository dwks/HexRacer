#pragma once

#include "Point.h"

namespace Project {
namespace Math {

class BoundingObject2D;

class BoundingObject {
public:
	BoundingObject(void) {}
	virtual ~BoundingObject(void) {}
	virtual bool pointInside(const Point& p) const = 0;
};

}  // namespace Math
}  // namespace Project