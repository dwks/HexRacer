#pragma once

#include "BoundingObject.h"
#include "ObjectSpatial.h"
#include <vector>
#include <list>
using namespace std;

namespace Project {
namespace Math {

/** A container of spatial objects
*/
class SpatialContainer
{
public:

	/** Query Types:
		INSIDE: objects completely inside the bounding object
		INTERSECT: objects that intersect the bounding object
		BOX_INTERSECT: objects that roughly intersect the bounding object
		NEARBY: data-structure specific, objects that could possibly intersect bounding object
	*/
	enum QueryType {INSIDE, INTERSECT, BOX_INTERSECT, NEARBY};

	SpatialContainer(void) {}
	virtual ~SpatialContainer(void) {}

	virtual bool add(ObjectSpatial* object) = 0;
	virtual void add(const vector<ObjectSpatial*>& objects) = 0;

	virtual bool remove(ObjectSpatial* object) = 0;
	virtual bool contains(ObjectSpatial* object) const = 0;

	virtual RayIntersection rayIntersection(Ray ray) const = 0;
	virtual int size() const = 0;
	virtual void clear() = 0;

	ObjectSpatial* nearest(const Point& point) const;
	ObjectSpatial* nearest(const Point& point, double max_distance) const;

	vector<ObjectSpatial*> query(const BoundingObject& bounding_object, QueryType query_type) const;
	vector<ObjectSpatial*> all() const;

	virtual ObjectSpatial* nearestSquared(const Point& point, double max_distance_squared, bool bounded) const = 0;

	virtual void appendQuery(vector<ObjectSpatial*>& result_vector, const BoundingObject& bounding_object, QueryType query_type) const = 0;
	virtual void appendAll(vector<ObjectSpatial*>& result_vector) const = 0;

protected:

	/** Returns true if @a object interacts with @a bounding object as specified by @a query_type
	*/
	bool queryTypeFilter(ObjectSpatial* object, const BoundingObject& bounding_object, QueryType query_type) const;


};

}  // namespace Math
}  // namespace Project