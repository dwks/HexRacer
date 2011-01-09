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
	wp = w;

	switch (projectAxis) {
		case X_AXIS:
			xp = 0.0f;
			yp = u;
			zp = v;
			;break;

		case Y_AXIS:
			xp = u;
			yp = 0.0f;
			zp = v;
			;break;

		case Z_AXIS:
			xp = u;
			yp = v;
			zp = 0.0f;
			;break;	
	}
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