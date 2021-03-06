#ifndef PROJECT_MATH__VERTEX2_D_H
#define PROJECT_MATH__VERTEX2_D_H

#include "BoundingObject2D.h"

namespace Project {
namespace Math {

/** A 2D point
*/
class Vertex2D
	: public BoundingObject2D {
private:
	Point position;
public:

	Vertex2D(Axis project_axis = Z_AXIS) : BoundingObject2D(project_axis) {}
	Vertex2D(double u, double v, Axis project_axis = Z_AXIS);
	Vertex2D(Point point, Axis project_axis = Z_AXIS);

	//Abstract method implementations
	double minX() const;
	double minY() const;
	double minZ() const;
	double maxX() const;
	double maxY() const;
	double maxZ() const;
	Point centroid() const;
	bool isInside2D(const BoundingObject2D& bounding_obj) const;
	bool pointInside(const Point& p) const;
	bool intersects2D(const BoundingObject2D& bounding_obj) const;
	void translate(const Point& translation);
	ObjectType getObjectType() const { return VERTEX; }

	//Class specific
	Point getPosition() const { return position; }

};

}  // namespace Math
}  // namespace Project

#endif
