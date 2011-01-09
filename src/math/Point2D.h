#pragma once

#include "Point.h"

namespace Project {
namespace Math {

class Point2D :
	public Point
{
private:
	Axis projectAxis;
public:
	Point2D(Axis project_axis = Z_AXIS)
		: Point(0.0f, 0.0f, 0.0f, 0.0f), projectAxis(project_axis) {}
	Point2D(double x, double y, Axis project_axis = Z_AXIS)
		: Point(x, y, 0.0f), projectAxis(project_axis) {}
	Point2D(double x, double y, double w = 1.0, Axis project_axis = Z_AXIS)
		: Point(x, y, 0.0f, w), projectAxis(project_axis) {}
	Point2D(Point p, Axis project_axis = Z_AXIS);

	double getZ() const { return 0.0f; }
	void setZ(double z) { return; }
	Axis getProjectAxis() {return projectAxis; }
};

}  // namespace Math
}  // namespace Project