#include "BoundingBox2D.h"
#include "Values.h"
using namespace Project;
using namespace Math;

BoundingBox2D::BoundingBox2D(Point2D corner1, Point2D corner2, Axis project_axis)
		: minCorner(project_axis), maxCorner(project_axis), BoundingObject2D(project_axis) {
	setCorners(corner1, corner2);
}

BoundingBox2D::BoundingBox2D(double width, double height, Axis project_axis, Point2D centroid)
		: minCorner(project_axis), maxCorner(project_axis), BoundingObject2D(project_axis) {
	Point2D diag = Point2D(width*0.5f, height*0.5f, project_axis);
	setCorners(Point2D(centroid-diag, project_axis), Point2D(centroid+diag, project_axis)); 
}

BoundingBox2D::~BoundingBox2D(void)
{
}

void BoundingBox2D::setCorners(Point2D corner1, Point2D corner2) {
	for (int i = 0; i < 3; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
	}
}

Point BoundingBox2D::centroid() {
	return (minCorner + maxCorner) * 0.5f;
}

bool BoundingBox2D::isInside(BoundingObject& bounding_obj) {

	for (int i = 0; i < 4; i++) {
		if (!bounding_obj.pointInside( getCorner(i) )) {
			return false;
		}
	}

	return false;
}

bool BoundingBox2D::pointInside2D(Point2D p) {

	return (
		p.getX() >= minCorner.getX() && p.getX() <= maxCorner.getX() &&
		p.getY() >= minCorner.getY() && p.getY() <= maxCorner.getY()
		);
}

bool BoundingBox2D::intersects2D(BoundingObject2D& bound_obj) {
	return false; //Implement me!
}

void BoundingBox2D::translate2D(Point2D& translation) {
	minCorner += translation;
	maxCorner += translation;
}

Point2D BoundingBox2D::getCorner(int index) {
	switch (index) {
		case 0: return getCorner(false, false);
		case 1: return getCorner(true, false);
		case 2: return getCorner(true, true);
		case 3: return getCorner(false, true);
	}

	return Point2D();
}

Point2D BoundingBox2D::getCorner(bool max_u, bool max_v) {
	double u;
	double v;

	Axis u_axis;
	Axis v_axis;

	switch (projectAxis) {
		case X_AXIS:
			u_axis = Y_AXIS;
			v_axis = Z_AXIS;
			break;
		case Y_AXIS:
			u_axis = X_AXIS;
			v_axis = Z_AXIS;
			break;
		case Z_AXIS:
			u_axis = X_AXIS;
			v_axis = Y_AXIS;
			break;
	}

	if (max_u)
		u = maxCorner.getCoord(u_axis);
	else
		u = minCorner.getCoord(u_axis);

	if (max_v)
		v = maxCorner.getCoord(v_axis);
	else
		v = minCorner.getCoord(v_axis);

	return Point2D(u, v, projectAxis);
}