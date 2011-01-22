#include "Vertex2D.h"

namespace Project {
namespace Math {

	Vertex2D::Vertex2D(double u, double v, Axis project_axis)
		: BoundingObject2D(project_axis) {
		position = Point::point2D(u, v, project_axis);
	}

	Vertex2D::Vertex2D(Point point, Axis project_axis)
		: BoundingObject2D(project_axis) {
		position = Point::point2D(point, project_axis);
	}

	double Vertex2D::minX() const {
		return getPosition().getX();
	}
	double Vertex2D::minY() const {
		return getPosition().getY();
	}
	double Vertex2D::minZ() const {
		return getPosition().getZ();
	}
	double Vertex2D::maxX() const {
		return getPosition().getX();
	}
	double Vertex2D::maxY() const {
		return getPosition().getY();
	}
	double Vertex2D::maxZ() const {
		return getPosition().getZ();
	}
	Point Vertex2D::centroid() const {
		return getPosition();
	}
	bool Vertex2D::isInside2D(const BoundingObject2D& bounding_obj) const {
		return bounding_obj.pointInside(getPosition());
	}

	bool Vertex2D::pointInside(const Point& p) const {
		return (getPosition() == p);
	}

	bool Vertex2D::intersects2D(const BoundingObject2D& bounding_obj) const {
		return bounding_obj.pointInside(getPosition());
	}
	void Vertex2D::translate(const Point& translation) {
		position += Point::point2D(translation, projectAxis);
	}

}  // namespace Math
}  // namespace Project
