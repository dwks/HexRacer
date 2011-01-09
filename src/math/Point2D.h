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

	Axis getProjectAxis() {return projectAxis; }

	void setX(double x);
    void setY(double y);
	void setZ(double z);

	/*
	Point2D normalized() const {return Point2D(Point::normalized(), projectAxis);}

	Point2D rotateX(double angle) const {return Point2D(Point::rotateX(angle), projectAxis);}
	
	Point2D rotateY(double angle) const {return Point2D(Point::rotateY(angle), projectAxis);}
    
	Point2D rotateZ(double angle) const {return Point2D(Point::rotateZ(angle), projectAxis);}
    
	Point2D crossProduct(const Point &other) const {return Point2D(Point::crossProduct(other), projectAxis);}
    
	Point operator + (const Point &other) const;
	Point operator - (const Point &other) const;
    
	Point operator * (double factor) const;

	Point operator / (double factor) const;
    
	Point projectOnto(const Point &onto) const;
	*/
};

}  // namespace Math
}  // namespace Project