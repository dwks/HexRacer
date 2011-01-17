#pragma once

#include "BoundingObject.h"
#include "ObjectSpatial.h"
#include <vector>
using namespace std;

namespace Project {
namespace Math {

/** A container of spatial objects
*/
class SpatialContainer
{
public:

	enum QueryType {INSIDE, INTERSECT, BOX_INTERSECT, NEARBY};

	SpatialContainer(void) {}
	virtual ~SpatialContainer(void) {}

	virtual bool add(ObjectSpatial* object) = 0;
	virtual bool remove(ObjectSpatial* object) = 0;
	virtual bool contains(ObjectSpatial* object) const = 0;
	virtual void add(vector<ObjectSpatial*> objects) = 0;
	/** Return a vector of all objects in this container that interact with the bounding object @a
		bounding_object based on @a query_type.
		INSIDE: objects completely inside the bounding object
		INTERSECT: objects that intersect the bounding object
		BOX_INTERSECT: objects that roughly intersect the bounding object
		NEARBY: data-structure specific
	*/
	virtual vector<ObjectSpatial*> query(const BoundingObject& bounding_object, QueryType query_type) const = 0;
	virtual vector<ObjectSpatial*> all() const = 0;
	virtual int size() const = 0;
	virtual void clear() = 0;

protected:

	/** Returns true if @a object interacts with @a bounding object as specified by @a query_type
	*/
	bool queryTypeFilter(ObjectSpatial* object, const BoundingObject& bounding_object, QueryType query_type) const;
	/** Append all results from a query to @a result_list
	*/
	virtual void appendQuery(vector<ObjectSpatial*>* result_list, const BoundingObject& bounding_object, QueryType query_type) const = 0;

};

}  // namespace Math
}  // namespace Project