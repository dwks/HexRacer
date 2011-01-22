#pragma once

#include "ObjectSpatial.h"

namespace Project {
namespace Math {

class BoundingObject3D;
class BoundingObject2D;

/** Represents a object with 3D dimensions
*/
class Object3D
	: public ObjectSpatial
{
public:
	Object3D(void) {}
	virtual ~Object3D(void) {}

	bool intersects(const BoundingObject& bounding_obj) const;
	void moveCentroid(const Point& position);

	virtual bool intersects3D(const BoundingObject3D& bounding_obj) const = 0;

	/** Instantiates and returns a 2D version of this object projected to the axis @a project_axis
	*/
	virtual BoundingObject2D* projectTo2D(Axis project_axis) const = 0;
	

};

}  // namespace Math
}  // namespace Project