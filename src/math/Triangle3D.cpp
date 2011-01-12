#include "Triangle3D.h"
#include "BoundingTriangle2D.h"
#include "BoundingObject2D.h"
#include "BoundingBox3D.h"
#include "Values.h"

namespace Project {
namespace Math {

	Triangle3D::Triangle3D(void)
	{
	}

	Triangle3D::Triangle3D(Point vertex1, Point vertex2, Point vertex3) {
		vertex[0] = vertex1;
		vertex[1] = vertex2;
		vertex[2] = vertex3;
	}

	Triangle3D::~Triangle3D(void)
	{
	}

	double Triangle3D::minX() const {
		return minimum(minimum(getVertex(0).getX(), getVertex(1).getX()), getVertex(2).getX());
	}
	double Triangle3D::minY() const {
		return minimum(minimum(getVertex(0).getY(), getVertex(1).getY()), getVertex(2).getY());
	}
	double Triangle3D::minZ() const {
		return minimum(minimum(getVertex(0).getZ(), getVertex(1).getZ()), getVertex(2).getZ());
	}
	double Triangle3D::maxX() const {
		return maximum(maximum(getVertex(0).getX(), getVertex(1).getX()), getVertex(2).getX());
	}
	double Triangle3D::maxY() const {
		return maximum(maximum(getVertex(0).getY(), getVertex(1).getY()), getVertex(2).getY());
	}
	double Triangle3D::maxZ() const {
		return maximum(maximum(getVertex(0).getZ(), getVertex(1).getZ()), getVertex(2).getZ());
	}

	Point Triangle3D::centroid() const {
		return ( getVertex(0) + getVertex(1) + getVertex(2) )/3.0f;
	}

	bool Triangle3D::isInside(const BoundingObject& bounding_obj) const {
		return (
			bounding_obj.pointInside(getVertex(0)) &&
			bounding_obj.pointInside(getVertex(1)) &&
			bounding_obj.pointInside(getVertex(2))
			);
	}

	BoundingObject2D* Triangle3D::projectTo2D(Axis project_axis) const {
		return new BoundingTriangle2D(getVertex(0), getVertex(1), getVertex(2), project_axis);
	}

	bool Triangle3D::intersects3D(const BoundingObject3D& bound_obj) const {

		const BoundingBox3D* box_3D = dynamic_cast<const BoundingBox3D*>(&bound_obj);
		if (box_3D) {
			//3D Triangle-Box Intersection

			//Try the collision in 2D for all three axes, if it passes all three, the objects intersect
			for (unsigned int i = 0; i < 3; i++) {
				Axis axis = (Axis) i;
				BoundingObject2D* this_2D = projectTo2D(axis);
				BoundingObject2D* bound_2D = bound_obj.projectTo2D(axis);
				bool intersects = this_2D->intersects2D(*bound_2D);
				delete(this_2D);
				delete(bound_2D);
				if (!intersects)
					return false;
			}
			return true;
		}

		return false;

	}

	void Triangle3D::translate(const Point& translation) {
		for (int i = 0; i < 3; i++) {
			vertex[i] += translation;
		}
	}

	Point Triangle3D::getVertex(short vert_index) const {
		return vertex[vert_index];
	}

}  // namespace Math
}  // namespace Project