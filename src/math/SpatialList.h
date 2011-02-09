#pragma once
#include "SpatialContainer.h"
#include "BoundingBox3D.h"

namespace Project {
namespace Math {

/** A vector of spatial objects
*/
class SpatialList :
	public SpatialContainer
{
public:
	SpatialList(bool use_bounding_box = false);
	~SpatialList(void);

	bool add(ObjectSpatial* object);
	void add(const vector<ObjectSpatial*>& objects);
	bool remove(ObjectSpatial* object);
	bool contains(ObjectSpatial* object) const;

	RayIntersection rayIntersection(Ray ray) const;
	int size() const;
	void clear();

	void appendQuery(vector<ObjectSpatial*>& result_vector, const BoundingObject& bounding_object, QueryType query_type) const;
	void appendAll(vector<ObjectSpatial*>& result_vector) const;
	void operateQuery(SpatialObjectOperator& op, const BoundingObject& bounding_object, QueryType query_type ) const;
	void operateAll(SpatialObjectOperator& op) const;

	ObjectSpatial* nearestSquared(const Point& point, double max_distance_squared, bool bounded) const;

private:

	BoundingBox3D* boundingBox;
	vector<ObjectSpatial*> objectsVector;

	void refreshBoundingBox();

};

}  // namespace Math
}  // namespace Project