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

	BoundingObject2D* projectTo2D(Axis project_axis) { return this; }
	bool pointInside(Point* p);

	virtual bool pointInside2D(Point2D p) = 0;
};


}  // namespace Math
}  // namespace Project