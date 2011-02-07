#include "BoundingCircle.h"
#include <math.h>
#include "Values.h"

namespace Project {
namespace Math {

	BoundingCircle::BoundingCircle(Point _position, double _radius, Axis project_axis)
		: BoundingObject2D(project_axis)
	{
		
		position = Point::point2D(_position, project_axis);
		setRadius(_radius);
		
	}

	double BoundingCircle::minX() const {
		if (projectAxis == X_AXIS)
			return 0.0;
		else
			return position.getX()-radius;
	}
	double BoundingCircle::minY() const {
		if (projectAxis == Y_AXIS)
			return 0.0;
		else
			return position.getY()-radius;
	}

	double BoundingCircle::minZ() const {
		if (projectAxis == Z_AXIS)
			return 0.0;
		else
			return position.getZ()-radius;
	}
	double BoundingCircle::maxX() const {
		if (projectAxis == X_AXIS)
			return 0.0;
		else
			return position.getX()+radius;
	}
	double BoundingCircle::maxY() const {
		if (projectAxis == Y_AXIS)
			return 0.0;
		else
			return position.getY()+radius;
	}

	double BoundingCircle::maxZ() const {
		if (projectAxis == Z_AXIS)
			return 0.0;
		else
			return position.getZ()+radius;
	}

	bool BoundingCircle::isInside2D(const BoundingObject2D& bounding_obj) const {

		switch (bounding_obj.getObjectType()) {
			case BOX:
				return (
					bounding_obj.minU() <= minU() && bounding_obj.maxU() >= maxU() &&
					bounding_obj.minV() <= minV() && bounding_obj.maxV() >= maxV()
					);

			case CIRCLE:
				return (
					(position.distance(bounding_obj.centroid())+
					((const BoundingCircle&)bounding_obj).getRadius()) <= radius
					);

			default:
				break;
		}

		return false;


	}

	bool BoundingCircle::pointInside(const Point& p) const {
		return (Point::point2D(p, projectAxis).distanceSquared(position) <= radiusSquared);
	}

	bool BoundingCircle::intersects2D(const BoundingObject2D& bounding_obj) const {

		switch (bounding_obj.getObjectType()) {

			case BOX: {
				Point p;
				p.setU( minimum ( 
					maximum( position.getU(projectAxis), bounding_obj.minU() ),
					bounding_obj.maxU() ), projectAxis
					);
				p.setV( minimum ( 
					maximum( position.getV(projectAxis), bounding_obj.minV() ),
					bounding_obj.maxV() ), projectAxis
					);
				return pointInside(p);
			}

			case CIRCLE:
				return (
					(position.distance(bounding_obj.centroid())-
					((const BoundingCircle&)bounding_obj).getRadius()) <= radius
					);

			case TRIANGLE:
				//NOT IMPLEMENTED
				return false;

			case PLANE:
				//NOT IMPLEMENTED
				return false;

			case CONVEX_HULL:
				//NOT IMPLEMENTED
				return false;

			default:
				break;
		}

		return bounding_obj.intersects2D(*this);

	}

	void BoundingCircle::translate(const Point& translation) {
		position += Point::point2D(translation, projectAxis);
	}

	//void BoundingCircle::expandToInclude(const Point& p);
	void BoundingCircle::setRadius(double _radius) {
		radius = std::abs(_radius);
		radiusSquared = radius*radius;
	}

}  // namespace Math
}  // namespace Project
