#ifndef PROJECT_MATH__BOUNDING_PLANE3_D_H
#define PROJECT_MATH__BOUNDING_PLANE3_D_H

#include "BoundingObject3D.h"
#include <float.h>

namespace Project {
namespace Math {

class BoundingPlane3D 
	: public BoundingObject3D {
private:
	Point origin;
	Point normal;
public:

	BoundingPlane3D(Point _origin = Point(), Point _normal = Point(0.0, 0.0, 1.0))
		: origin(_origin), normal(_normal.normalized()) {}

	//Abstract method implementations
	double minX() const { return FLT_MIN; }
	double minY() const { return FLT_MIN; }
	double minZ() const { return FLT_MIN; }
	double maxX() const { return FLT_MAX; }
	double maxY() const { return FLT_MAX; }
	double maxZ() const { return FLT_MAX; }
	Point centroid() const { return origin; }
	bool isInside(const BoundingObject& bounding_obj) const;
	BoundingObject2D* projectTo2D(Axis project_axis) const;
	bool pointInside(const Point& p) const;
	bool intersects3D(const BoundingObject3D& bounding_obj) const;
	void translate(const Point& translation);
	ObjectType getObjectType() const { return PLANE; }

	void moveCentroid(const Point& position) { this->origin = position; }

	//Class specific
	void setNormal(Point _normal) { normal = _normal.normalized(); }
	Point getNormal() const { return normal; }

};

}  // namespace Math
}  // namespace Project

#endif
