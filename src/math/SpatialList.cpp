#include "SpatialList.h"
#include "misc/StdVectorFunctions.h"
using namespace Project;
using namespace Math;
using namespace Misc;

SpatialList::SpatialList(bool use_bounding_box) {
	if (use_bounding_box)
		boundingBox = new BoundingBox3D();
	else
		boundingBox = NULL;
}

SpatialList::~SpatialList(void) {
	//if (boundingBox)
	//	delete(boundingBox);
}

bool SpatialList::add(ObjectSpatial* object) {

	if (boundingBox) {
		if (objectsVector.empty())
			boundingBox->setToObject(*object);
		else
			boundingBox->expandToInclude(*object);
	}

	objectsVector.push_back(object);
	return true;
}

void SpatialList::add(const vector<ObjectSpatial*>& objects) {
	if (boundingBox) {
		for (unsigned int i = 0; i < objects.size(); i++) {
			add(objects[i]);
		}
	}
	else
		objectsVector.insert(objectsVector.end(), objects.begin(), objects.end());
}

bool SpatialList::remove(ObjectSpatial* object) {

	if (vectorRemoveOneElement(objectsVector, object)) {
		refreshBoundingBox();
		return true;
	}
	return false;
}

bool SpatialList::contains(ObjectSpatial* object) const {
	return vectorContains(objectsVector, object);
}

void SpatialList::appendQuery(vector<ObjectSpatial*>& result_vector, const BoundingObject& bounding_object, QueryType query_type) const {

	if (objectsVector.empty() || (boundingBox && !boundingBox->intersects(bounding_object)))
		return;

	if (query_type == SpatialContainer::NEARBY)
		result_vector.insert(result_vector.end(), objectsVector.begin(), objectsVector.end());
	else {
		for (unsigned int i = 0; i < objectsVector.size(); i++) {
			if (queryTypeFilter(objectsVector[i], bounding_object, query_type)) {
				result_vector.push_back(objectsVector[i]);
			}
		}
	}
}

void SpatialList::appendAll(vector<ObjectSpatial*>& result_vector) const {
	result_vector.insert(result_vector.end(), objectsVector.begin(), objectsVector.end());
}

ObjectSpatial* SpatialList::nearestSquared(const Point& point, double max_distance_squared, bool bounded) const {

	ObjectSpatial* nearest = NULL;

	for (unsigned int i = 0; i < objectsVector.size(); i++) {

		double dist_squared = point.distanceSquared(objectsVector[i]->centroid());
		if ((!bounded && i == 0) || dist_squared < max_distance_squared) {
			nearest = objectsVector[i];
			max_distance_squared = dist_squared;
		}
	}

	return nearest;

}
RayIntersection SpatialList::rayIntersection(Ray ray) const {
	if (objectsVector.empty() || (boundingBox && !boundingBox->rayIntersection(ray).intersects))
		return RayIntersection();

	RayIntersection first_intersection = RayIntersection();

	for (unsigned int i = 0; i < objectsVector.size(); i++) {
		RayIntersection intersection = objectsVector[i]->rayIntersection(ray);
		if (intersection.intersects && (!first_intersection.intersects || intersection < first_intersection)) {
			first_intersection = intersection;
		}
	}

	return first_intersection;
}

int SpatialList::size() const {
	return objectsVector.size();
}

void SpatialList::clear() {
	objectsVector.clear();
}

void SpatialList::refreshBoundingBox() {

	if (boundingBox && !objectsVector.empty()) {

		boundingBox->setToObject(*objectsVector[0]);
		for (unsigned int i = 1; i < objectsVector.size(); i++)
			boundingBox->expandToInclude(*objectsVector[i]);

	}

}