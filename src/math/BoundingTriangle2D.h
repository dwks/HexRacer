#ifndef PROJECT_MATH__BOUNDING_TRIANGLE2_D_H
#define PROJECT_MATH__BOUNDING_TRIANGLE2_D_H

#include "Point.h"
#include "BoundingObject2D.h"

namespace Project {
namespace Math {

/** A 2D triangle
*/
class BoundingTriangle2D
	: public BoundingObject2D {
private:
	Point vertex[3];
public:

	BoundingTriangle2D(Axis project_axis = Z_AXIS)
		: BoundingObject2D(project_axis) {}
	BoundingTriangle2D(Point vertex1, Point vertex2, Point vertex3, Axis project_axis = Z_AXIS);
	
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
	bool intersects2D(const BoundingObject2D& bound_obj) const;
	void translate(const Point& translation);

	//Class specific
	Point getVertex(short vert_index) const;

};

}  // namespace Math
}  // namespace Project

#endif
