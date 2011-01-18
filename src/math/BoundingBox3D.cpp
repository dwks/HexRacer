#include "BoundingBox3D.h"
#include "BoundingPlane3D.h"
#include "BoundingConvexHull3D.h"
#include "Values.h"
#include "BoundingBox2D.h"
using namespace Project;
using namespace Math;

BoundingBox3D::BoundingBox3D(Point corner1, Point corner2) {
	setCorners(corner1, corner2);
}

BoundingBox3D::BoundingBox3D(double width, double height, double depth, Point centroid) {
	Point diag = Point(width, height, depth)*0.5f;
	setCorners(centroid-diag, centroid+diag);
}

BoundingBox3D::BoundingBox3D(const ObjectSpatial& object) {
	setToObject(object);
}

BoundingBox3D::~BoundingBox3D(void)
{
}

void BoundingBox3D::setCorners(Point corner1, Point corner2) {
	for (unsigned int i = 0; i < 3; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(corner1.getCoord(axis), corner2.getCoord(axis)), axis);
	}
}


Point BoundingBox3D::centroid() const {
	return (minCorner + maxCorner)*0.5f;
}
bool BoundingBox3D::isInside(const BoundingObject& bounding_obj) const {

	for (unsigned int i = 0; i < 8; i++) {
		if (!bounding_obj.pointInside( getCorner(i) )) {
			return false;
		}
	}

	return true;
}

BoundingObject2D* BoundingBox3D::projectTo2D(Axis project_axis) const {
	return new BoundingBox2D(*this, project_axis);
}

bool BoundingBox3D::pointInside(const Point& p) const {
	return (
		p.getX() >= minX() && p.getX() <= maxX() &&
		p.getY() >= minY() && p.getY() <= maxY() &&
		p.getZ() >= minZ() && p.getZ() <= maxZ()
		);
}

bool BoundingBox3D::intersects3D(const BoundingObject3D& bound_obj) const {

	switch (bound_obj.getObjectType()) {

		case BOX:
			//3D Box-Box Intersection
			return (
				bound_obj.minX() <= maxX() && bound_obj.maxX() >= minX() &&
				bound_obj.minY() <= maxY() && bound_obj.maxY() >= minY() &&
				bound_obj.minZ() <= maxZ() && bound_obj.maxZ() >= minZ()
				);

		case PLANE:
			//3D Box-Plane Intersection
			for (int i = 0; i < 8; i++) {
				if (bound_obj.pointInside(getCorner(i))) {
					return true;
				}
			}
			return false;

	
		case CONVEX_HULL: {
			//3D Box-Convex Hull Intersection
			std::vector<BoundingPlane3D> planes = ((const BoundingConvexHull3D&)bound_obj).getPlanes();
			for (unsigned int i = 0; i < planes.size(); i++) {
				if (!intersects3D(planes[i])) {
					return false;
				}
			}
			return (planes.size() > 0);
        }
        
        default:
            break;
	}

	//Defer to the other object's interesection tests
	return bound_obj.intersects3D(*this);
}

void BoundingBox3D::translate(const Point& translation) {
	minCorner += translation;
	maxCorner += translation;
}

Point BoundingBox3D::getCorner(int index) const {
	switch (index) {
		case 0: return getCorner(false, false, false);
		case 1: return getCorner(true, false, false);
		case 2: return getCorner(true, true, false);
		case 3: return getCorner(false, true, false);
		case 4: return getCorner(false, false, true);
		case 5: return getCorner(true, false, true);
		case 6: return getCorner(true, true, true);
		case 7: return getCorner(false, true, true);
	}

	return Point();
}

Point BoundingBox3D::getCorner(bool max_x, bool max_y, bool max_z) const {
	double x;
	double y;
	double z;

	if (max_x)
		x = maxX();
	else
		x = minX();

	if (max_y)
		y = maxY();
	else
		y = minY();

	if (max_z)
		z = maxZ();
	else
		z = minZ();

	return Point(x, y, z);
}

void BoundingBox3D::expandToInclude(const Point& point) {
	for (unsigned int i = 0; i < 3; i++) {
		Axis axis = (Axis) i;
		minCorner.setCoord(minimum(minCorner.getCoord(axis), point.getCoord(axis)), axis);
		maxCorner.setCoord(maximum(maxCorner.getCoord(axis), point.getCoord(axis)), axis);
	}
}

void BoundingBox3D::expandToInclude(const ObjectSpatial& object) {
	expandToInclude(Point(object.minX(), object.minY(), object.minZ()));
	expandToInclude(Point(object.maxX(), object.maxY(), object.maxZ()));
}

void BoundingBox3D::setToObject(const ObjectSpatial& object) {
	setCorners(
		Point(object.minX(), object.minY(), object.minZ()),
		Point(object.maxX(), object.maxY(), object.maxZ())
		);
}