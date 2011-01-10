#include "BoundingBox2D.h"
#include "Values.h"
using namespace Project;
using namespace Math;

BoundingBox2D::BoundingBox2D(Point corner1, Point corner2, Axis project_axis)
	: minCorner(project_axis), maxCorner(project_axis), BoundingObject2D(project_axis) {
	setCorners(corner1, corner2);
}

BoundingBox2D::BoundingBox2D(double width, double height, Axis project_axis, Point centroid)
	: minCorner(project_axis), maxCorner(project_axis), BoundingObject2D(project_axis) {
	Point2D diag = Point2D(width*0.5f, height*0.5f, project_axis);
	setCorners(Point2D(centroid-diag, project_axis), Point2D(centroid+diag, project_axis)); 
}

BoundingBox2D::BoundingBox2D(ObjectSpatial& object, Axis project_axis)
	: minCorner(project_axis), maxCorner(project_axis), BoundingObject2D(project_axis) {

	setCorners(
		Point(object.minX(), object.minY(), object.minZ()),
		Point(object.maxX(), object.maxY(), object.maxZ())
		);
}

BoundingBox2D::~BoundingBox2D(void)
{
}

void BoundingBox2D::setCorners(Point corner1, Point corner2) {
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

	return true;
}

bool BoundingBox2D::pointInside2D(Point2D p) {

	if (p.getProjectAxis() != projectAxis)
		return false;

	return (
		p.getU() >= minCorner.getU() && p.getU() <= maxCorner.getU() &&
		p.getV() >= minCorner.getV() && p.getV() <= maxCorner.getV()
		);
}

bool BoundingBox2D::intersects2D(BoundingObject2D& bound_obj) {

	if (bound_obj.getProjectAxis() != projectAxis)
		return false;

	BoundingBox2D* box_2D = dynamic_cast<BoundingBox2D*>(&bound_obj);
	if (box_2D) {
		return (
			box_2D->minU() <= maxU() && box_2D->maxU() >= minU() &&
			box_2D->minV() <= maxV() && box_2D->maxV() >= minV()
			);
	}

	return false;
}

/*
void BoundingBox2D::translate2D(Point2D& translation) {
	minCorner += translation;
	maxCorner += translation;
}
*/

void BoundingBox2D::translate(Point& translation) {
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

void BoundingBox2D::expandToInclude(Point& point) {
	for (int i = 0; i < 3; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(minCorner.getCoord(axis), point.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(maxCorner.getCoord(axis), point.getCoord(axis)), axis);
	}
}

void BoundingBox2D::expandToInclude(ObjectSpatial& object) {
	expandToInclude(Point(object.minX(), object.minY(), object.minZ()));
	expandToInclude(Point(object.maxX(), object.maxY(), object.maxZ()));
}