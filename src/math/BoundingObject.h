#pragma once

#include "Point.h"
#include "ObjectSpatial.h"

namespace Project {
namespace Math {

class ObjectSpatial;

/** A spatial object with a defined inside and outside
*/
class BoundingObject {
public:

	BoundingObject(void) {}
	virtual ~BoundingObject(void) {}
	virtual bool pointInside(const Point& p) const = 0;
	virtual bool is2D() const = 0;
	virtual ObjectSpatial& toObjectSpatial() = 0;
	virtual const ObjectSpatial& toConstObjectSpatial() const = 0;
};

}  // namespace Math
}  // namespace Project