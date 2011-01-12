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
	bool contains(ObjectSpatial* object) const;
	void add(vector<ObjectSpatial*> objects);
	vector<ObjectSpatial*> query(BoundingObject& bounding_object, QueryType query_type) const;
	vector<ObjectSpatial*> all() const;
	int size() const;
	void clear();

	void appendQuery(vector<ObjectSpatial*>* result_list, BoundingObject& bounding_object, QueryType query_type) const;

protected:

	bool leaf;
	int splitCount;
	BSPTree* child[2];
	int subtreeSize;

	SpatialList list;

	virtual bool allowSplit() const = 0;
	virtual void split(vector<ObjectSpatial*>* objects = NULL) = 0;
	virtual const BoundingObject& getBoundingObject() const = 0;

};

}  // namespace Math
}  // namespace Project