#pragma once
#include "BoundingObject.h"
#include "Object2D.h"

namespace Project {
namespace Math {

/** A bounding object in 2D space
*/
class BoundingObject2D :
	public BoundingObject, public Object2D
{
private:
public:
	BoundingObject2D(Axis projectAxis)
		: Object2D(projectAxis) {}
	virtual ~BoundingObject2D(void) {}
	bool is2D() const { return true; }
	ObjectSpatial& toObjectSpatial() { return (BoundingObject2D&) *this; }
	const ObjectSpatial& toConstObjectSpatial() const { return (const BoundingObject2D&) *this; }
};


}  // namespace Math
}  // namespace Project