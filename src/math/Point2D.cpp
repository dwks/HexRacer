/*

#include "Point2D.h"
using namespace Project;
using namespace Math;

Point2D::Point2D(Point& p, Axis project_axis) {
	
	projectAxis = project_axis;
	setX(p.getX());
	setY(p.getY());
	setZ(p.getZ());
	
}

Point2D::Point2D(double u, double v, Axis project_axis, double w) {
	projectAxis = project_axis;
	setW(w);
	setU(u);
	setV(v);
}

double Point2D::getX() const {
	if (projectAxis == X_AXIS)
		return 0.0f;
	else
		return xp;
}

double Point2D::getY() const {
	if (projectAxis == Y_AXIS)
		return 0.0f;
	else
		return yp;
}

double Point2D::getZ() const {
	if (projectAxis == Z_AXIS)
		return 0.0f;
	else
		return zp;
}


double Point2D::getU() const {
	switch (projectAxis) {
		case X_AXIS:
			return getY();
		case Y_AXIS: case Z_AXIS:
			return getX();
	}
	return 0.0f;
}

double Point2D::getV() const {
	switch (projectAxis) {
		case X_AXIS: case Y_AXIS:
			return getZ();
		case Z_AXIS:
			return getY();
	}
	return 0.0f;
}

void Point2D::setX(double x) {
	if (projectAxis != X_AXIS)
		xp = x;
}

void Point2D::setY(double y) {
	if (projectAxis != Y_AXIS)
		yp = y;
}
void Point2D::setZ(double z) {
	if (projectAxis != Z_AXIS)
		zp = z;
}


void Point2D::setU(double u) {
	switch (projectAxis) {
		case X_AXIS:
			setY(u); break;
		case Y_AXIS: case Z_AXIS:
			setX(u); break;
	}
}

void Point2D::setV(double v) {
	switch (projectAxis) {
		case X_AXIS: case Y_AXIS:
			setZ(v); break;
		case Z_AXIS:
			setY(v); break;
	}
}

void Point2D::operator = (Point &other) {
	Point2D* p_2D = dynamic_cast<Point2D*>(&other);
	if (p_2D)
		projectAxis = p_2D->getProjectAxis();

	setX(other.getX());
	setY(other.getY());
	setZ(other.getZ());
	setW(other.getW());
}

*/