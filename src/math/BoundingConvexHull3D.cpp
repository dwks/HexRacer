#include "BoundingConvexHull3D.h"
#include "Vertex2D.h"

namespace Project {
namespace Math {

	BoundingConvexHull3D::BoundingConvexHull3D(int num_planes) {
		planes.resize(num_planes);
	}

	BoundingConvexHull3D::BoundingConvexHull3D(std::vector<BoundingPlane3D> _planes) {
		planes = _planes;
		if (planes.size() > 0)
			centroidPoint = planes[0].centroid();
	}

	//Abstract method implementations
	bool BoundingConvexHull3D::isInside(const BoundingObject& bounding_obj) const {
		return false; //Not implemented
	}

	BoundingObject2D* BoundingConvexHull3D::projectTo2D(Axis project_axis) const {
		return new Vertex2D();
	}

	bool BoundingConvexHull3D::pointInside(const Point& p) const {
		for (unsigned int i = 0; i < planes.size(); i++) {
			if (!planes[i].pointInside(p)) {
				return false;
			}
		}
		return (planes.size() > 0);
	}

	bool BoundingConvexHull3D::intersects3D(const BoundingObject3D& bound_obj) const {
		return bound_obj.intersects3D(*this);
	}

	void BoundingConvexHull3D::translate(const Point& translation) {
		for (unsigned int i = 0; i < planes.size(); i++) {
			planes[i].translate(translation);
		}
		centroidPoint += translation;
	}

	//Class Specific
	void BoundingConvexHull3D::setPlaneNormal(int plane_index, Point normal) {
		if (plane_index < 0 || plane_index >= static_cast<int>(planes.size()))
			return;

		planes[plane_index].setNormal(normal);
	}

	void BoundingConvexHull3D::setPlaneOrigin(int plane_index, Point origin) {
		if (plane_index < 0 || plane_index >= static_cast<int>(planes.size()))
			return;

		planes[plane_index].moveCentroid(origin);
	}

}  // namespace Math
}  // namespace Project
