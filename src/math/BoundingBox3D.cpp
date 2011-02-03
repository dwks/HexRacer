#include "BoundingBox3D.h"
#include "BoundingPlane3D.h"
#include "BoundingConvexHull3D.h"
#include "Values.h"
#include "BoundingBox2D.h"
#include <float.h>
using namespace Project;
using namespace Math;
using namespace std;

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

	if (!bounding_obj.is2D()) {
	
		const BoundingObject3D& bound_3D = (BoundingObject3D&) bounding_obj;

		switch (bound_3D.getObjectType()) {

			case BOX:
				return (
					bound_3D.minX() <= minX() && bound_3D.maxX() >= maxX() &&
					bound_3D.minY() <= minY() && bound_3D.maxY() >= maxY() &&
					bound_3D.minZ() <= minZ() && bound_3D.maxZ() >= maxZ()
					);

			default: break;
		}
	}

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

bool BoundingBox3D::intersects3D(const BoundingObject3D& bounding_obj) const {

	switch (bounding_obj.getObjectType()) {

		case BOX:
			//3D Box-Box Intersection
			return (
				bounding_obj.minX() <= maxX() && bounding_obj.maxX() >= minX() &&
				bounding_obj.minY() <= maxY() && bounding_obj.maxY() >= minY() &&
				bounding_obj.minZ() <= maxZ() && bounding_obj.maxZ() >= minZ()
				);

		case PLANE: case CONVEX_HULL:
			return intersectionType(bounding_obj) != INTERSECT_NONE;
	
        default:
            break;
	}

	//Defer to the other object's interesection tests
	return bounding_obj.intersects3D(*this);
}

ObjectSpatial::IntersectionType BoundingBox3D::intersectionType(const BoundingObject& bounding_obj) const {

	bool intersects = false;
	bool inside = true;

	if (!bounding_obj.is2D()) {

		switch (((const BoundingObject3D&)bounding_obj).getObjectType()) {

			case PLANE:
				for (int i = 0; i < 8; i++) {
					bool corner_inside = bounding_obj.pointInside(getCorner(i));
					intersects = (intersects || corner_inside);
					inside = (inside && corner_inside);
				}

				if (inside)
					return INTERSECT_INSIDE;
				else if (intersects)
					return INTERSECT_INTERSECTS;
				else
					return INTERSECT_NONE;

			case CONVEX_HULL: {
				//3D Box-Convex Hull Intersection
				intersects = true;
				const std::vector<BoundingPlane3D>& planes = ((const BoundingConvexHull3D&)bounding_obj).getPlanes();
				for (unsigned int i = 0; i < planes.size(); i++) {
					IntersectionType intersect_type = intersectionType(planes[i]);
					inside = (inside && intersect_type == INTERSECT_INSIDE);
					intersects = (intersects && intersect_type != INTERSECT_NONE);
				}
				
				if (planes.empty() || !intersects)
					return INTERSECT_NONE;
				else if (inside)
					return INTERSECT_INSIDE;
				else
					return INTERSECT_INTERSECTS;
			}

		}

	}

	return ObjectSpatial::intersectionType(bounding_obj);
}
void BoundingBox3D::translate(const Point& translation) {
	minCorner += translation;
	maxCorner += translation;
}

RayIntersection BoundingBox3D::rayIntersection(Ray ray) const {

	double tnear = FLT_MIN;
	double tfar = FLT_MAX;

	double min_plane;
	double max_plane;
	double origin;
	double dir;

	for (short i = 0; i < 3; i++) {

		min_plane = minCorner[i];
		max_plane = maxCorner[i];
		origin = ray.origin[i];
		dir = ray.direction[i];

		if (dir == 0.0) {
			if (!(origin >= min_plane && origin <= max_plane)) {
				return RayIntersection();
			}
		}
		else {
			double t1 = (min_plane-origin)/dir;
			double t2 = (max_plane-origin)/dir;
			
			if (t1 > t2) {
				double temp = t2;
				t2 = t1;
				t1 = temp;
			}

			if (t1 > tnear)
				tnear = t1;
			if (t2 < tfar)
				tfar = t2;

			if (tnear > tfar)
				return RayIntersection();
			if (ray.maxBounded && tnear > ray.maxT)
				return RayIntersection();
			if (ray.minBounded && tfar < ray.minT)
				return RayIntersection();
		}

	}

	if (ray.insideRange(tnear))
		return RayIntersection(tnear);
	if (ray.insideRange(tfar))
		return RayIntersection(tfar);

	return RayIntersection();

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

void BoundingBox3D::expand(double expansion) {
	expansion = fabs(expansion);
	Point exp_p(expansion, expansion, expansion);
	minCorner -= exp_p;
	maxCorner += exp_p;
}