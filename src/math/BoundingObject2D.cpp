#include "BoundingObject2D.h"
#include "BoundingObject3D.h"
using namespace Project;
using namespace Math;

bool BoundingObject2D::pointInside(Point* p) {
	Point2D* p_2D = dynamic_cast<Point2D*>(p);
	
	if (p_2D)
		return pointInside2D(*p_2D);
	else
		return pointInside2D(Point2D(*p, projectAxis));
}