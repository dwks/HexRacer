#include "Object2D.h"
#include "BoundingObject2D.h"
#include "BoundingObject3D.h"
using namespace Project;
using namespace Math;

double Object2D::minU() const {

	switch (projectAxis) {
		case X_AXIS:
			return minY();
		case Y_AXIS: case Z_AXIS:
			return minX();
	}

	return 0.0f;
}

double Object2D::minV() const {

	switch (projectAxis) {
		case Z_AXIS:
			return minY();
		case X_AXIS: case Y_AXIS: 
			return minZ();
	}

	return 0.0f;
}

double Object2D::maxU() const {

	switch (projectAxis) {
		case X_AXIS:
			return maxY();
		case Y_AXIS: case Z_AXIS:
			return maxX();
	}

	return 0.0f;
}
double Object2D::maxV() const {

	switch (projectAxis) {
		case Z_AXIS:
			return maxY();
		case X_AXIS: case Y_AXIS: 
			return maxZ();
	}

	return 0.0f;
}

bool Object2D::intersects(const BoundingObject& bounding_obj) const {
	//Check if the bounding object is 3D
	if (!bounding_obj.is2D()) {
		//Convert the 3D bounding object to a 2D bounding object
		BoundingObject2D* bound_2D = ((const BoundingObject3D&)bounding_obj).projectTo2D(getProjectAxis());
		//Perform the intersection test on the 2D bounding object
		bool intersects = intersects2D(*bound_2D);
		delete(bound_2D); //Delete the temporary 2D object
		return intersects;
	}
	else {
		const BoundingObject2D& obj_2D = (const BoundingObject2D&) bounding_obj;
		return (obj_2D.getProjectAxis() == getProjectAxis() && intersects2D(obj_2D));
	}
}

bool Object2D::isInside(const BoundingObject& bounding_obj) const {
	//Check if the bounding object is 3D
	if (!bounding_obj.is2D()) {
		//Convert the 3D bounding object to a 2D bounding object
		BoundingObject2D* bound_2D = ((const BoundingObject3D&)bounding_obj).projectTo2D(getProjectAxis());
		//Perform the intersection test on the 2D bounding object
		bool inside = isInside2D(*bound_2D);
		delete(bound_2D); //Delete the temporary 2D object
		return inside;
	}
	else {
		const BoundingObject2D& obj_2D = (const BoundingObject2D&) bounding_obj;
		return (obj_2D.getProjectAxis() == getProjectAxis() && isInside2D(obj_2D));
	}
}

void Object2D::moveCentroid(const Point& position) {
	Point translation = Point::point2D(position, projectAxis)-centroid();
	translate(translation);
}