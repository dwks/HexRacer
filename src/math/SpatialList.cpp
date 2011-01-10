#include "SpatialList.h"
using namespace Project;
using namespace Math;

SpatialList::SpatialList(void)
{
}

SpatialList::~SpatialList(void)
{
}

bool SpatialList::add(ObjectSpatial* object) {
	objectList.push_back(object);
	return true;
}

bool SpatialList::remove(ObjectSpatial* object) {

	for (int i = 0; i < objectList.size(); i++) {
		if (objectList[i] == object) {
			//Swap the object with the last object in the list and decrement the size
			objectList[i] = objectList[objectList.size()-1];
			objectList.resize(objectList.size()-1);
			return true;
		}
	}

	return false;
}

bool SpatialList::contains(ObjectSpatial* object) {
	for (int i = 0; i < objectList.size(); i++) {
		if (objectList[i] == object)
			return true;
	}
	return false;
}

vector<ObjectSpatial*> SpatialList::query(BoundingObject& bounding_object, QueryType query_type) {

	vector<ObjectSpatial*> return_list;
	appendQuery(&return_list, bounding_object, query_type);
	return return_list;
}

void SpatialList::appendQuery(vector<ObjectSpatial*>* result_list, BoundingObject& bounding_object, QueryType query_type) {

	for (int i = 0; i < objectList.size(); i++) {

		if (queryTypeFilter(objectList[i], bounding_object, query_type)) {
			result_list->push_back(objectList[i]);
		}

	}
}

vector<ObjectSpatial*> SpatialList::all() {
	return objectList;
}

int SpatialList::size() {
	return objectList.size();
}

void SpatialList::clear() {
	objectList.clear();
}