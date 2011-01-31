#include "BoundingPlane3D.h"
#include "Vertex2D.h"
#include "Vertex3D.h"
#include "BoundingBox3D.h"
#include "Triangle3D.h"
#include "Geometry.h"
#include <istream>
#include <ostream>

namespace Project {
namespace Math {

	bool BoundingPlane3D::isInside(const BoundingObject& bounding_obj) const {
		return false;
	}

	BoundingObject2D* BoundingPlane3D::projectTo2D(Axis project_axis) const {
		return new Vertex2D(); //2D Plane equivalent does not exist yet
	}

	bool BoundingPlane3D::pointInside(const Point& p) const {
		return Geometry::frontOfPlane(origin, normal, p);
	}

	bool BoundingPlane3D::intersects3D(const BoundingObject3D& bounding_obj) const {
		return bounding_obj.intersects3D(*this);
	}

	RayIntersection BoundingPlane3D::rayIntersection(Ray ray) const {
		return Geometry::rayPlaneIntersection(ray, origin, normal);
	}

	void BoundingPlane3D::translate(const Point& translation) {
		origin += translation;
	}

	std::ostream &operator << (std::ostream &stream, const BoundingPlane3D &plane) {
		stream << plane.centroid() << ' ' << plane.getNormal();
		return stream;
	}

	std::istream &operator >> (std::istream &stream, BoundingPlane3D &plane) {
		Point origin;
		Point normal;
		stream >> origin >> normal;
		plane = BoundingPlane3D(origin, normal);
		return stream;
	}


}  // namespace Math
}  // namespace Project
