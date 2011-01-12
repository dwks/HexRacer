#pragma once
#include "SpatialContainer.h"

namespace Project {
namespace Math {

class SpatialList :
	public SpatialContainer
{
public:
	SpatialList(void);
	~SpatialList(void);

	bool add(ObjectSpatial* object);
	bool remove(ObjectSpatial* object);
	bool contains(ObjectSpatial* object);
	void add(vector<ObjectSpatial*> objects);
	vector<ObjectSpatial*> query(BoundingObject& bounding_object, QueryType query_type);
	vector<ObjectSpatial*> all();
	int size();
	void clear();

	void appendQuery(vector<ObjectSpatial*>* result_list, BoundingObject& bounding_object, QueryType query_type);

private:

	vector<ObjectSpatial*> objectList;

};

}  // namespace Math
}  // namespace Project