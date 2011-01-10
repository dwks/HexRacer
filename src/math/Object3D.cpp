#include "Object3D.h"
#include "BoundingObject2D.h"
using namespace Project;
using namespace Math;

bool Object3D::intersects(const BoundingObject& bound_obj) const {
	//Check if the bounding object is a 2D object
	const BoundingObject2D* bound_2D = dynamic_cast<const BoundingObject2D*>(&bound_obj);
	
	if (bound_2D) {
		//Intersection test on 2D object
		BoundingObject2D& this_2D = projectTo2D(bound_2D->getProjectAxis()); //Get the 2D equivalent of this object
		bool intersect = this_2D.intersects2D(*bound_2D); //Perform the intersection test in 2D
		delete(&this_2D); //Delete the temporary 2D object
		return intersect;
	}
	else
		return intersects3D((BoundingObject3D&) bound_obj);
}

void Object3D::moveCentroid(const Point& position) {
	Point translation = position-centroid();
	translate(translation);
}