#include "BoundingBox2D.h"
#include "Values.h"
using namespace Project;
using namespace Math;

BoundingBox2D::BoundingBox2D(Point corner1, Point corner2, Axis project_axis)
	: BoundingObject2D(project_axis) {
	setCorners(corner1, corner2);
}

BoundingBox2D::BoundingBox2D(double width, double height, Axis project_axis, Point centroid)
	: BoundingObject2D(project_axis) {
	Point diag = Point::point2D(width*0.5f, height*0.5f, project_axis);
	setCorners(Point::point2D(centroid-diag, project_axis), Point::point2D(centroid+diag, project_axis)); 
}

BoundingBox2D::BoundingBox2D(const ObjectSpatial& object, Axis project_axis)
	: BoundingObject2D(project_axis) {

	setCorners(
		Point(object.minX(), object.minY(), object.minZ()),
		Point(object.maxX(), object.maxY(), object.maxZ())
		);
}

BoundingBox2D::~BoundingBox2D(void)
{
}

void BoundingBox2D::setCorners(Point corner1, Point corner2) {

	for (unsigned int i = 0; i < 3; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
	}

	corner1 = Point::point2D(corner1, projectAxis);
	corner2 = Point::point2D(corner2, projectAxis);
}

Point BoundingBox2D::centroid() const {
	return (minCorner + maxCorner) * 0.5f;
}

bool BoundingBox2D::isInside2D(const BoundingObject2D& bounding_obj) const {

	for (unsigned int i = 0; i < 4; i++) {
		if (!bounding_obj.pointInside( getCorner(i) )) {
			return false;
		}
	}

	return true;
}

bool BoundingBox2D::pointInside(const Point& p) const {

	return (
		p.getU(projectAxis) >= minU() && p.getU(projectAxis) <= maxU() &&
		p.getV(projectAxis) >= minV() && p.getV(projectAxis) <= maxV()
		);
}

bool BoundingBox2D::intersects2D(const BoundingObject2D& bound_obj) const {

	if (bound_obj.getProjectAxis() != projectAxis)
		return false;

	const BoundingBox2D* box_2D = dynamic_cast<const BoundingBox2D*>(&bound_obj);
	if (box_2D) {
		return (
			box_2D->minU() <= maxU() && box_2D->maxU() >= minU() &&
			box_2D->minV() <= maxV() && box_2D->maxV() >= minV()
			);
	}

	return false;
}

void BoundingBox2D::translate(const Point& translation) {
	setCorners(minCorner + translation, maxCorner + translation);
}

Point BoundingBox2D::getCorner(int index) const {
	switch (index) {
		case 0: return getCorner(false, false);
		case 1: return getCorner(true, false);
		case 2: return getCorner(true, true);
		case 3: return getCorner(false, true);
	}

	return Point();
}

Point BoundingBox2D::getCorner(bool max_u, bool max_v) const {

	double u;
	double v;

	if (max_u)
		u = maxU();
	else
		u = minU();

	if (max_v)
		v = maxV();
	else
		v = minV();

	return Point::point2D(u, v, projectAxis);
}

void BoundingBox2D::expandToInclude(const Point& point) {
	for (unsigned int i = 0; i < 3; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(minCorner.getCoord(axis), point.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(maxCorner.getCoord(axis), point.getCoord(axis)), axis);
	}
}

void BoundingBox2D::expandToInclude(const ObjectSpatial& object) {
	expandToInclude(Point(object.minX(), object.minY(), object.minZ()));
	expandToInclude(Point(object.maxX(), object.maxY(), object.maxZ()));
}