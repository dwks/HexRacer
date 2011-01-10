#pragma once
#include "BoundingObject.h"
#include "Object2D.h"

namespace Project {
namespace Math {

class BoundingObject2D :
	public BoundingObject, public Object2D
{
private:
public:
	BoundingObject2D(Axis projectAxis)
		: Object2D(projectAxis) {}
	virtual ~BoundingObject2D(void) {}
};


}  // namespace Math
}  // namespace Project