#ifndef PROJECT_MATH__BOUNDING_CONVEX_HULL3_D_H
#define PROJECT_MATH__BOUNDING_CONVEX_HULL3_D_H

#include "BoundingObject3D.h"
#include "BoundingPlane3D.h"
#include <vector>
#include <float.h>

namespace Project {
namespace Math {

class BoundingConvexHull3D 
	: public BoundingObject3D {
private:
	std::vector<BoundingPlane3D> planes;
	Point centroidPoint;
	
public:
	BoundingConvexHull3D(int num_planes);
	BoundingConvexHull3D(std::vector<BoundingPlane3D> _planes);

	//Abstract method implementations
	double minX() const { return FLT_MIN; }
	double minY() const { return FLT_MIN; }
	double minZ() const { return FLT_MIN; }
	double maxX() const { return FLT_MAX; }
	double maxY() const { return FLT_MAX; }
	double maxZ() const { return FLT_MAX; }
	Point centroid() const { return centroidPoint; }
	bool isInside(const BoundingObject& bounding_obj) const;
	BoundingObject2D* projectTo2D(Axis project_axis) const;
	bool pointInside(const Point& p) const;
	bool intersects3D(const BoundingObject3D& bounding_obj) const;
	void translate(const Point& translation);
	ObjectType getObjectType() const { return CONVEX_HULL; }

	//Class Specific
	void setPlaneNormal(int plane_index, Point normal);
	void setPlaneOrigin(int plane_index, Point origin);
	std::vector<BoundingPlane3D> getPlanes() const { return planes; }
	int getNumPlanes() const { return planes.size(); }
};

}  // namespace Math
}  // namespace Project

#endif
