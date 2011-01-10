#pragma once

#include "BoundingObject3D.h"

namespace Project {
namespace Math {

class BoundingBox3D
	: public BoundingObject3D
{
private:
	Point minCorner;
	Point maxCorner;
	void setCorners(Point corner1, Point corner2);
public:
	BoundingBox3D(void) {}
	BoundingBox3D(Point corner1, Point corner2);
	BoundingBox3D(double width, double height, double depth, Point centroid = Point());
	BoundingBox3D(ObjectSpatial& object);
	~BoundingBox3D(void);

	//Abstract method implementations
	double minX() { return minCorner.getX(); }
	double minY() { return minCorner.getY(); }
	double minZ() { return minCorner.getZ(); }
	double maxX() { return maxCorner.getX(); }
	double maxY() { return maxCorner.getY(); }
	double maxZ() { return maxCorner.getZ(); }
	Point centroid();
	bool isInside(BoundingObject& bounding_obj);
	BoundingObject2D& projectTo2D(Axis project_axis);
	bool pointInside3D(Point p);
	bool intersects3D(BoundingObject3D& bound_obj);
	void translate(Point& translation);

	//Class specific
	Point getCorner(int index);
	Point getCorner(bool max_x, bool max_y, bool max_z);
	void expandToInclude(Point& point);
	void expandToInclude(ObjectSpatial& object);
};

}  // namespace Math
}  // namespace Project