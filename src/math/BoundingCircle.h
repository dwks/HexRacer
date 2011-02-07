#ifndef PROJECT_MATH__BOUNDING_CIRCLE_H
#define PROJECT_MATH__BOUNDING_CIRCLE_H

#include "BoundingObject2D.h"

namespace Project {
namespace Math {

class BoundingCircle 
	: public BoundingObject2D {
private:

	Point position;
	double radius;
	double radiusSquared;

public:
	BoundingCircle(Point _position = Point(), double _radius = 1.0, Axis project_axis = Z_AXIS);

	//Abstract method implementations
	double minX() const;
	double minY() const;
	double minZ() const;
	double maxX() const;
	double maxY() const;
	double maxZ() const;
	Point centroid() const { return position;}
	bool isInside2D(const BoundingObject2D& bounding_obj) const;
	bool pointInside(const Point& p) const;
	bool intersects2D(const BoundingObject2D& bounding_obj) const;
	void translate(const Point& translation);
	ObjectType getObjectType() const { return CIRCLE; }

	//Class specific
	Point getPosition() const { return position; }
	double getRadius() const { return radius; }
	double getRadiusSquared() const { return radiusSquared; }
	//void expandToInclude(const Point& p);
	void setRadius(double _radius);


};

}  // namespace Math
}  // namespace Project

#endif
