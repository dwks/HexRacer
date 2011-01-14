#include "Vertex3D.h"
#include "BoundingObject3D.h"
#include "Vertex2D.h"

namespace Project {
namespace Math {

	double Vertex3D::minX() const {
		return getPosition().getX();
	}
	double Vertex3D::minY() const {
		return getPosition().getY();
	}
	double Vertex3D::minZ() const {
		return getPosition().getZ();
	}
	double Vertex3D::maxX() const {
		return getPosition().getX();
	}
	double Vertex3D::maxY() const {
		return getPosition().getY();
	}
	double Vertex3D::maxZ() const {
		return getPosition().getZ();
	}
	Point Vertex3D::centroid() const {
		return getPosition();
	}
	bool Vertex3D::isInside(const BoundingObject& bounding_obj) const {
		return bounding_obj.pointInside(getPosition());
	}
	BoundingObject2D* Vertex3D::projectTo2D(Axis project_axis) const {
		return new Vertex2D(getPosition(), project_axis);
	}
	bool Vertex3D::intersects3D(const BoundingObject3D& bound_obj) const {
		return bound_obj.pointInside(getPosition());
	}
	void Vertex3D::translate(const Point& translation) {
		position += translation;
	}

}  // namespace Math
}  // namespace Project
