#pragma once

#include "Object3D.h"
#include "RayIntersection.h"
#include "Ray.h"

namespace Project {
namespace Math {

/** A 3D triangle
*/
class Triangle3D
	: public Object3D
{
private:
	Point vertex[3];
	Point normal;
public:
	Triangle3D(void);
	Triangle3D(Point vertex1, Point vertex2, Point vertex3);
	//Triangle3D(Triangle3D& triangle);
	~Triangle3D(void);

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
	ObjectType getObjectType() const { return TRIANGLE; }

	//Class specific
	virtual Point getVertex(short vert_index) const;
	Point getNormal() const { return normal; }

	//RayIntersection getRayIntersection(Ray ray) const;
};

}  // namespace Math
}  // namespace Project