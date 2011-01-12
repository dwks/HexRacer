#pragma once

#include "BoundingObject.h"
#include "ObjectSpatial.h"
#include <vector>
using namespace std;

namespace Project {
namespace Math {

class SpatialContainer
{
public:

	enum QueryType {INSIDE, INTERSECT, BOX_INTERSECT, NEARBY};

	SpatialContainer(void) {}
	virtual ~SpatialContainer(void) {}

	virtual bool add(ObjectSpatial* object) = 0;
	virtual bool remove(ObjectSpatial* object) = 0;
	virtual bool contains(ObjectSpatial* object) = 0;
	virtual void add(vector<ObjectSpatial*> objects) = 0;
	virtual vector<ObjectSpatial*> query(BoundingObject& bounding_object, QueryType query_type) = 0;
	virtual vector<ObjectSpatial*> all() = 0;
	virtual int size() = 0;
	virtual void clear() = 0;

protected:

	bool queryTypeFilter(ObjectSpatial* object, BoundingObject& bounding_object, QueryType query_type);
	virtual void appendQuery(vector<ObjectSpatial*>* result_list, BoundingObject& bounding_object, QueryType query_type) = 0;

};

}  // namespace Math
}  // namespace Project