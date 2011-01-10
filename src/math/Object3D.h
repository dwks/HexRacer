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

	bool intersects(BoundingObject& bound_obj);
	void moveCentroid(Point& position);

	virtual bool intersects3D(BoundingObject3D& bound_obj) = 0;
	virtual BoundingObject2D& projectTo2D(Axis project_axis) = 0;
	

};

}  // namespace Math
}  // namespace Project