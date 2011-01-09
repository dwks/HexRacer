#pragma once

#include "BoundingObject.h"
#include "Object3D.h"

namespace Project {
namespace Math {

class BoundingObject3D
	: public BoundingObject, public Object3D
{
public:
	BoundingObject3D(void) {}
	virtual ~BoundingObject3D(void) {}

	bool pointInside(Point* p);

	virtual bool pointInside3D(Point p) = 0;
};

}  // namespace Math
}  // namespace Project