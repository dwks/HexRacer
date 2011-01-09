#include "BoundingBox2D.h"
#include "Values.h"
using namespace Project;
using namespace Math;

BoundingBox2D::BoundingBox2D(Point2D corner1, Point2D corner2, Axis project_axis)
		: BoundingObject2D(project_axis) {
	setCorners(corner1, corner2);
}

BoundingBox2D::~BoundingBox2D(void)
{
}

void BoundingBox2D::setCorners(Point2D corner1, Point2D corner2) {
	for (int i = 0; i < 2; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
	}
}

Point BoundingBox2D::centroid() {
	return (minCorner + maxCorner) * 0.5f;
}

bool BoundingBox2D::isInside(BoundingObject* bounding_obj) {

	for (int i = 0; i < 4; i++) {
		if (!bounding_obj->pointInside( getCorner(i) )) {
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

bool BoundingBox2D::intersects2D(BoundingObject2D* bound_obj) {
	return false; //Implement me!
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

Point2D BoundingBox2D::getCorner(bool max_x, bool max_y) {
	double x;
	double y;

	if (max_x)
		x = maxCorner.getX();
	else
		x = minCorner.getX();

	if (max_y)
		y = maxCorner.getY();
	else
		y = minCorner.getY();

	return Point2D(x, y, projectAxis);
}