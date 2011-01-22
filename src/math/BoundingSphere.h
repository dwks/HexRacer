#ifndef PROJECT_MATH__BOUNDING_SPHERE_H
#define PROJECT_MATH__BOUNDING_SPHERE_H

#include "BoundingObject3D.h"

namespace Project {
namespace Math {

class BoundingSphere 
	: public BoundingObject3D {
private:
	double radius;
	double radiusSquared;
	Point position;
public:

	BoundingSphere(Point _position = Point(), double _radius = 1.0);

	//Abstract method implementations
	double minX() const { return position.getX()-radius; }
	double minY() const { return position.getY()-radius; }
	double minZ() const { return position.getZ()-radius; }
	double maxX() const { return position.getX()+radius; }
	double maxY() const { return position.getY()+radius; }
	double maxZ() const { return position.getZ()+radius; }
	Point centroid() const { return position; }
	bool isInside(const BoundingObject& bounding_obj) const;
	BoundingObject2D* projectTo2D(Axis project_axis) const;
	bool pointInside(const Point& p) const;
	bool intersects3D(const BoundingObject3D& bounding_obj) const;
	void translate(const Point& translation) { position += translation; }
	ObjectType getObjectType() const { return CIRCLE; }

	//Class specific
	Point getPosition() const { return position; }
	double getRadius() const { return radius; }
	double getRadiusSquared() const { return radiusSquared; }
	void setRadius(double _radius);

};

}  // namespace Math
}  // namespace Project

#endif
