#pragma once

#include "BoundingObject2D.h"

namespace Project {
namespace Math {

class BoundingBox2D
	: public BoundingObject2D
{
private:
	Point2D minCorner;
	Point2D maxCorner;
	void setCorners(Point corner1, Point corner2);
public:
	BoundingBox2D(Axis project_axis = Z_AXIS)
		: minCorner(project_axis), maxCorner(project_axis) , BoundingObject2D(project_axis) {}
	BoundingBox2D(Point corner1, Point corner2, Axis project_axis = Z_AXIS);
	BoundingBox2D(double width, double height, Axis project_axis = Z_AXIS, Point centroid = Point());
	BoundingBox2D(ObjectSpatial& object, Axis project_axis = Z_AXIS);
	~BoundingBox2D(void);

	//Abstract method implementations
	double minX() { return minCorner.getX(); }
	double minY() { return minCorner.getY(); }
	double minZ() { return minCorner.getZ(); }
	double maxX() { return maxCorner.getX(); }
	double maxY() { return maxCorner.getY(); }
	double maxZ() { return maxCorner.getZ(); }
	double minU() { return minCorner.getU(); }
	double minV() { return minCorner.getV(); }
	double maxU() { return maxCorner.getU(); }
	double maxV() { return maxCorner.getV(); }
	Point centroid();
	bool isInside(BoundingObject& bounding_obj);
	bool pointInside2D(Point2D p);
	bool intersects2D(BoundingObject2D& bound_obj);
	void translate(Point& translation);

	//Class specific
	Point2D getCorner(int index);
	Point2D getCorner(bool max_u, bool max_v);

	void expandToInclude(Point& point);
	void expandToInclude(ObjectSpatial& object);

};

}  // namespace Math
}  // namespace Project