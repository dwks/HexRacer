#pragma once

#include "ObjectSpatial.h"

namespace Project {
namespace Math {

class BoundingObject3D;
class BoundingObject2D;

class Object3D
	: public ObjectSpatial
{
public:
	Object3D(void) {}
	virtual ~Object3D(void) {}

	bool intersects(const BoundingObject& bound_obj) const;
	void moveCentroid(const Point& position);

	virtual bool intersects3D(const BoundingObject3D& bound_obj) const = 0;
	virtual BoundingObject2D* projectTo2D(Axis project_axis) const = 0;
	

};

}  // namespace Math
}  // namespace Project