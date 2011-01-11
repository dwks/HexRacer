/*

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
	Point2D(double u, double v, Axis project_axis = Z_AXIS, double w = 1.0);
	Point2D(Point& p, Axis project_axis = Z_AXIS);

	double getX() const;
	double getY() const;
	double getZ() const;

	double getU() const;
	double getV() const;

	Axis getProjectAxis() {return projectAxis; }

	void setX(double x);
    void setY(double y);
	void setZ(double z);

	void setU(double u);
	void setV(double v);

	virtual void operator = (Point &other);
};

}  // namespace Math
}  // namespace Project

*/