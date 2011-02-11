#pragma once

#include "BoundingObject.h"
#include "Object3D.h"

namespace Project {
namespace Math {

/** A bounding object in 3D space
*/
class BoundingObject3D
	: public BoundingObject, public Object3D
{
public:
	BoundingObject3D(void) {}
	virtual ~BoundingObject3D(void) {}
	bool is2D() const { return false; }
	ObjectSpatial& toObjectSpatial() { return (BoundingObject3D&) *this; }
	const ObjectSpatial& toConstObjectSpatial() const { return (const BoundingObject3D&) *this; }
};

}  // namespace Math
}  // namespace Project