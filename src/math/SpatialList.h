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
	bool remove(ObjectSpatial* object);
	bool contains(ObjectSpatial* object) const;
	void add(vector<ObjectSpatial*> objects);
	vector<ObjectSpatial*> query(const BoundingObject& bounding_object, QueryType query_type) const;
	vector<ObjectSpatial*> all() const;
	int size() const;
	void clear();

	void appendQuery(vector<ObjectSpatial*>* result_list, const BoundingObject& bounding_object, QueryType query_type) const;
	void appendAll(vector<ObjectSpatial*>* result_list) const;

private:

	BoundingBox3D* boundingBox;
	vector<ObjectSpatial*> objectList;

	void refreshBoundingBox();

};

}  // namespace Math
}  // namespace Project