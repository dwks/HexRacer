#include "Point2D.h"
using namespace Project;
using namespace Math;

Point2D::Point2D(Point p, Axis project_axis) {
	switch (project_axis) {
		case X_AXIS:
			setX(p.getY());
			setY(p.getZ());
			;break;
		case Y_AXIS:
			setX(p.getX());
			setY(p.getZ());
			;break;
		case Z_AXIS:
			setX(p.getX());
			setZ(p.getY());
			;break;
	}

	setZ(0.0f);
	projectAxis = project_axis;
}