#ifndef PROJECT_MATH__VERTEX3_D_H
#define PROJECT_MATH__VERTEX3_D_H

#include "Object3D.h"

namespace Project {
namespace Math {

/** A 3D point
*/
class Vertex3D
	: public Object3D {
private:
	Point position;
public:

	Vertex3D()
		: position() {}
	Vertex3D(Point point)
		: position(point) {}

	//Abstract method implementations
	double minX() const;
	double minY() const;
	double minZ() const;
	double maxX() const;
	double maxY() const;
	double maxZ() const;
	Point centroid() const;
	bool isInside(const BoundingObject& bounding_obj) const;
	BoundingObject2D* projectTo2D(Axis project_axis) const;
	bool intersects3D(const BoundingObject3D& bound_obj) const;
	void translate(const Point& translation);
	ObjectType getObjectType() const { return VERTEX; }

	//Class specific
	Point getPosition() const { return position; }

};

}  // namespace Math
}  // namespace Project

#endif
