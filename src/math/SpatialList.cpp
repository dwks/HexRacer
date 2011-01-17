#include "SpatialList.h"
using namespace Project;
using namespace Math;

SpatialList::SpatialList(bool use_bounding_box)
{
	if (use_bounding_box)
		boundingBox = new BoundingBox3D();
	else
		boundingBox = NULL;
}

SpatialList::~SpatialList(void)
{
}

bool SpatialList::add(ObjectSpatial* object) {

	if (boundingBox) {
		if (size() == 0)
			boundingBox->setToObject(*object);
		else
			boundingBox->expandToInclude(*object);
	}

	objectList.push_back(object);
	return true;
}

void SpatialList::add(vector<ObjectSpatial*> objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		add(objects[i]);
	}
}

bool SpatialList::remove(ObjectSpatial* object) {

	for (unsigned int i = 0; i < objectList.size(); i++) {
		if (objectList[i] == object) {
			//Swap the object with the last object in the list and decrement the size
			objectList[i] = objectList[objectList.size()-1];
			objectList.resize(objectList.size()-1);
			refreshBoundingBox();
			return true;
		}
	}

	return false;
}

bool SpatialList::contains(ObjectSpatial* object) const {
	for (unsigned int i = 0; i < objectList.size(); i++) {
		if (objectList[i] == object)
			return true;
	}
	return false;
}

vector<ObjectSpatial*> SpatialList::query(const BoundingObject& bounding_object, QueryType query_type) const {

	vector<ObjectSpatial*> return_list;

	if (boundingBox && !boundingBox->intersects(bounding_object))
		return return_list;

	appendQuery(&return_list, bounding_object, query_type);
	return return_list;
}

void SpatialList::appendQuery(vector<ObjectSpatial*>* result_list, const BoundingObject& bounding_object, QueryType query_type) const {

	if (boundingBox && !boundingBox->intersects(bounding_object))
		return;

	for (unsigned int i = 0; i < objectList.size(); i++) {

		if (queryTypeFilter(objectList[i], bounding_object, query_type)) {
			result_list->push_back(objectList[i]);
		}

	}
}

vector<ObjectSpatial*> SpatialList::all() const {
	return objectList;
}

int SpatialList::size() const {
	return objectList.size();
}

void SpatialList::clear() {
	objectList.clear();
}

void SpatialList::refreshBoundingBox() {

	if (size() > 0) {

		boundingBox->setToObject(*objectList[0]);

		for (unsigned int i = 1; i < objectList.size(); i++) {
			boundingBox->expandToInclude(*objectList[i]);
		}

	}

}