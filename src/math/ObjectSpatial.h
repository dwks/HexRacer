#pragma once

#include "Point.h"
#include "Ray.h"
#include "RayIntersection.h"

namespace Project {
namespace Math {

class BoundingObject;

/** An object that exists in two or three dimensional space
*/
class ObjectSpatial
{
public:

	enum IntersectionType { INTERSECT_NONE, INTERSECT_INSIDE, INTERSECT_INTERSECTS };
	enum ObjectType { VERTEX, BOX, TRIANGLE, PLANE, CONVEX_HULL, CIRCLE };

	virtual ~ObjectSpatial(void) {}

	virtual double minX() const = 0;
	virtual double minY() const = 0;
	virtual double minZ() const = 0;
	virtual double maxX() const = 0;
	virtual double maxY() const = 0;
	virtual double maxZ() const = 0;

	double widthX() const { return maxX() - minX(); }
	double widthY() const { return maxY() - minY(); }
	double widthZ() const { return maxZ() - minZ(); }

	double minCoord(Axis axis) const;
	double maxCoord(Axis axis) const;
	double widthCoord(Axis axis) const;

	virtual Point centroid() const = 0;
	virtual void translate(const Point& translation) = 0;
	virtual void moveCentroid(const Point& position) = 0;
	/** Returns true iff this object is entirely contained inside @a bounding obj
	*/
	virtual bool isInside(const BoundingObject& bounding_obj) const = 0;
	/** Returns true iff this object intersects @a bounding_obj
	*/
	virtual bool intersects(const BoundingObject& bounding_obj) const = 0;

	virtual IntersectionType intersectionType(const BoundingObject& bounding_obj) const;

	virtual ObjectType getObjectType() const = 0;

	virtual RayIntersection rayIntersection(Ray ray) const { return RayIntersection(); }

	virtual bool operator < (const ObjectSpatial &other) { return true; }

};

}  // namespace Math
}  // namespace Project