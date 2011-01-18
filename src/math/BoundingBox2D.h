#pragma once

#include "BoundingObject2D.h"

namespace Project {
namespace Math {

/** A 2D axis-aligned bounding box
*/
class BoundingBox2D
	: public BoundingObject2D
{
private:
	Point minCorner;
	Point maxCorner;
public:
	BoundingBox2D(Axis project_axis = Z_AXIS)
		: BoundingObject2D(project_axis) {}
	BoundingBox2D(Point corner1, Point corner2, Axis project_axis = Z_AXIS);
	BoundingBox2D(double width, double height, Axis project_axis = Z_AXIS, Point centroid = Point());
	BoundingBox2D(const ObjectSpatial& object, Axis project_axis = Z_AXIS);
	~BoundingBox2D(void);

	//Abstract method implementations
	double minX() const { return minCorner.getX(); }
	double minY() const { return minCorner.getY(); }
	double minZ() const { return minCorner.getZ(); }
	double maxX() const { return maxCorner.getX(); }
	double maxY() const { return maxCorner.getY(); }
	double maxZ() const { return maxCorner.getZ(); }
	Point centroid() const;
	bool isInside2D(const BoundingObject2D& bounding_obj) const;
	bool pointInside(const Point& p) const;
	bool intersects2D(const BoundingObject2D& bound_obj) const;
	void translate(const Point& translation);
	ObjectType getObjectType() const { return BOX; }

	//Class specific

	/** Set the dimensions of this box to just contain 2 points
	*/
	void setCorners(Point corner1, Point corner2);

	/** Returns one of the corner points of this box (0 - 3)
	*/
	Point getCorner(int index) const;

	/** Returns a corner with the specified max or min axis coordinates
	*/
	Point getCorner(bool max_u, bool max_v) const;

	/** Expand the dimensions of this box to include the point @a point
	*/
	void expandToInclude(const Point& point);

	/** Expand the dimensions of this box to include the spatial object @a object
	*/
	void expandToInclude(const ObjectSpatial& object);

};

}  // namespace Math
}  // namespace Project