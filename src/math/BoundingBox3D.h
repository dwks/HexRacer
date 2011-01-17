#pragma once

#include "BoundingObject3D.h"

namespace Project {
namespace Math {

/** A 3D axis-aligned bounding box
*/
class BoundingBox3D
	: public BoundingObject3D
{
private:
	Point minCorner;
	Point maxCorner;
public:
	BoundingBox3D(void) {}
	BoundingBox3D(Point corner1, Point corner2);
	BoundingBox3D(double width, double height, double depth, Point centroid = Point());
	BoundingBox3D(const ObjectSpatial& object);
	~BoundingBox3D(void);

	//Abstract method implementations
	double minX() const { return minCorner.getX(); }
	double minY() const { return minCorner.getY(); }
	double minZ() const { return minCorner.getZ(); }
	double maxX() const { return maxCorner.getX(); }
	double maxY() const { return maxCorner.getY(); }
	double maxZ() const { return maxCorner.getZ(); }
	Point centroid() const;
	bool isInside(const BoundingObject& bounding_obj) const;
	BoundingObject2D* projectTo2D(Axis project_axis) const;
	bool pointInside(const Point& p) const;
	bool intersects3D(const BoundingObject3D& bound_obj) const;
	void translate(const Point& translation);

	//Class specific

	/** Set the dimensions of this box to the envelope of @a object
	*/
	void setToObject(const ObjectSpatial& object);
	/** Set the dimensions of this box to just contain 2 points
	*/
	void setCorners(Point corner1, Point corner2);
	/** Returns one of the corner points of this box (0 - 7)
	*/
	Point getCorner(int index) const;
	/** Returns a corner with the specified max or min axis coordinates
	*/
	Point getCorner(bool max_x, bool max_y, bool max_z) const;
	/** Expand the dimensions of this box to include the point @a point
	*/
	void expandToInclude(const Point& point);
	/** Expand the dimensions of this box to include the spatial object @a object
	*/
	void expandToInclude(const ObjectSpatial& object);
};

}  // namespace Math
}  // namespace Project