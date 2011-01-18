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
};


}  // namespace Math
}  // namespace Project