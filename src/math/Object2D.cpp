#include "Object2D.h"
#include "BoundingObject3D.h"
using namespace Project;
using namespace Math;

bool Object2D::intersects(BoundingObject* bound_obj) {
	//Check if the bounding object is 3D
	BoundingObject3D* bound_3D = dynamic_cast<BoundingObject3D*>(bound_obj);

	if (bound_3D) {
		//Convert the 3D bounding object to a 2D bounding object
		BoundingObject2D* bound_2D = bound_3D->projectTo2D(getProjectAxis());
		//Perform the intersection test on the 2D bounding object
		bool intersects = intersects2D(bound_2D);
		delete(bound_2D); //Delete the 2D bounding object
		return intersects;
	}
	else
		return intersects2D((BoundingObject2D*) bound_obj);
}