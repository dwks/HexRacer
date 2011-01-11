#include "Object2D.h"
#include "BoundingObject3D.h"
using namespace Project;
using namespace Math;

bool Object2D::intersects(const BoundingObject& bound_obj) const {
	//Check if the bounding object is 3D
	const BoundingObject3D* bound_3D = dynamic_cast<const BoundingObject3D*>(&bound_obj);

	if (bound_3D) {
		//Convert the 3D bounding object to a 2D bounding object
		BoundingObject2D* bound_2D = bound_3D->projectTo2D(getProjectAxis());
		//Perform the intersection test on the 2D bounding object
		bool intersects = intersects2D(*bound_2D);
		delete(bound_2D); //Delete the temporary 2D object
		return intersects;
	}
	else
		return intersects2D((BoundingObject2D&) bound_obj);
}

bool Object2D::isInside(const BoundingObject& bounding_obj) const {
	//Check if the bounding object is 3D
	const BoundingObject3D* bound_3D = dynamic_cast<const BoundingObject3D*>(&bounding_obj);

	if (bound_3D) {
		//Convert the 3D bounding object to a 2D bounding object
		BoundingObject2D* bound_2D = bound_3D->projectTo2D(getProjectAxis());
		//Perform the intersection test on the 2D bounding object
		bool inside = isInside2D(*bound_2D);
		delete(bound_2D); //Delete the temporary 2D object
		return inside;
	}
	else
		return isInside2D((BoundingObject2D&) bounding_obj);
}

void Object2D::moveCentroid(const Point& position) {
	Point translation = Point::point2D(translation, projectAxis)-centroid();
	translate(translation);
	//translate2D(translation);
}