#include "Triangle3D.h"
#include "BoundingTriangle2D.h"
#include "BoundingObject2D.h"
#include "BoundingBox3D.h"
#include "BoundingPlane3D.h"
#include "BoundingConvexHull3D.h"
#include "Values.h"
#include "Geometry.h"

namespace Project {
namespace Math {

	Triangle3D::Triangle3D(void)
	{
	}

	Triangle3D::Triangle3D(Point vertex1, Point vertex2, Point vertex3) {
		vertex[0] = vertex1;
		vertex[1] = vertex2;
		vertex[2] = vertex3;
		normal = Geometry::triangleNormal(vertex1, vertex2, vertex3);
	}

	/*
	Triangle3D::Triangle3D(Triangle3D& triangle) {
		vertex[0] = triangle.getVertex(0);
		vertex[1] = triangle.getVertex(1);
		vertex[2] = triangle.getVertex(2);
	}
	*/

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

	bool Triangle3D::intersects3D(const BoundingObject3D& bounding_obj) const {

		//Not implemented for sphere

		switch (bounding_obj.getObjectType()) {

			case BOX: {
				//Try the collision in 2D for all three axes, if it passes all three, the objects intersect
				for (unsigned int i = 0; i < 3; i++) {
					Axis axis = (Axis) i;
					BoundingObject2D* this_2D = projectTo2D(axis);
					BoundingObject2D* bound_2D = bounding_obj.projectTo2D(axis);
					bool intersects = this_2D->intersects2D(*bound_2D);
					delete(this_2D);
					delete(bound_2D);
					if (!intersects)
						return false;
				}
				return true;
			}

			case PLANE: {
				const BoundingPlane3D& plane_3D = (const BoundingPlane3D&) bounding_obj;
				//3D Triangle-Plane Intersection
				for (short i = 0; i < 3; i++) {
					if (plane_3D.pointInside(getVertex(i))) {
						return true;
					}
				}
				return false;
			}

			case CONVEX_HULL: {
				//3D Triangle-Convex Hull Intersection
				const std::vector<BoundingPlane3D>& planes = ((const BoundingConvexHull3D&) bounding_obj).getPlanes();
				for (unsigned int i = 0; i < planes.size(); i++) {
					if (!intersects3D(planes[i])) {
						return false;
					}
				}
				return (planes.size() > 0);
            }
            
            default:
                break;

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

	RayIntersection Triangle3D::rayIntersection(Ray ray) const {

		double d = normal.dotProduct(ray.direction);

		if (d != 0.0) {

			Point v = ray.origin-getVertex(0);
			double t = -(v.dotProduct(normal))/d;

			//Fail if the intersection is not within the ray's range
			if (!ray.insideRange(t))
				return RayIntersection();

			Point ray_point = ray.atT(t);

			//Calculate barycentric coordinates, return if point is outside of triangle
			double s0 = ((getVertex(2)-getVertex(1)).crossProduct(ray_point-getVertex(1))).dotProduct(normal);
			if (s0 < 0.0)
				return RayIntersection();

			double s1 = ((getVertex(0)-getVertex(2)).crossProduct(ray_point-getVertex(2))).dotProduct(normal);
			if (s1 < 0.0)
				return RayIntersection();

			double s2 = ((getVertex(1)-getVertex(0)).crossProduct(ray_point-getVertex(0))).dotProduct(normal);
			if (s2 < 0.0)
				return RayIntersection();

			return RayIntersection(t);
		}

		return RayIntersection();

	}
}  // namespace Math
}  // namespace Project