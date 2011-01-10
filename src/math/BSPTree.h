#pragma once
#include "SpatialList.h"

namespace Project {
namespace Math {

class BSPTree :
	public SpatialContainer
{
public:
	BSPTree(int split_count);
	virtual ~BSPTree(void);

	bool add(ObjectSpatial* object);
	bool remove(ObjectSpatial* object);
	bool contains(ObjectSpatial* object);
	vector<ObjectSpatial*> query(BoundingObject& bounding_object, QueryType query_type);
	vector<ObjectSpatial*> all();
	int size();
	void clear();

	void appendQuery(vector<ObjectSpatial*>* result_list, BoundingObject& bounding_object, QueryType query_type);

protected:

	bool leaf;
	int splitCount;
	BSPTree* child[2];
	int subtreeSize;

	SpatialList list;

	virtual bool allowSplit() = 0;
	virtual void split() = 0;
	virtual BoundingObject& getBoundingObject() = 0;

};

}  // namespace Math
}  // namespace Project