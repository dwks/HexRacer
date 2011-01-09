#include "BoundingObject3D.h"
#include "BoundingObject2D.h"
using namespace Project;
using namespace Math;

bool BoundingObject3D::pointInside(Point* p) {
	//Check if the point is a 2D point
	Point2D* p_2D = dynamic_cast<Point2D*>(p);
	
	if (p_2D) {
		//Inside test on 2D point
		BoundingObject2D* this_2D = this->projectTo2D(p_2D->getProjectAxis()); //Get the 2D equivalent of this object
		bool inside = this_2D->pointInside2D(*p_2D); //Perform the inside test on the 2D object
		delete(this_2D); //Delete the temporary 2D object
		return inside;
	}
	else
		return pointInside3D(*p);
}