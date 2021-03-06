#include "SpatialContainer.h"
#include "BoundingBox2D.h"
#include "BoundingBox3D.h"
#include "log/Logger.h"
using namespace Project;
using namespace Math;

bool SpatialContainer::queryTypeFilter(ObjectSpatial* object, const BoundingObject& bounding_object, QueryType query_type) const {

	Object2D* obj_2D = NULL;
    
	switch (query_type) {

		case INSIDE:
			return object->isInside(bounding_object);

		case INTERSECT:
			return object->intersects(bounding_object);

		case BOX_INTERSECT:
			//Check if the object is 2D
			//Create a 2D or 3D bounding box based on the object for the intersection test 
			if (obj_2D)
				return BoundingBox2D(*object, obj_2D->getProjectAxis()).intersects(bounding_object);
			else
				return BoundingBox3D(*object).intersects(bounding_object);

		case NEARBY:
			return true;
        
        default:
            break;
	}

	return false;

}

ObjectSpatial* SpatialContainer::nearest(const Point& point) const {
	return nearestSquared(point, 0.0, false);
}
ObjectSpatial* SpatialContainer::nearest(const Point& point, double max_distance) const {
	return nearestSquared(point, max_distance*max_distance, true);
}

vector<ObjectSpatial*> SpatialContainer::query(const BoundingObject& bounding_object, QueryType query_type) const {
	vector<ObjectSpatial*> vector;
	appendQuery(vector, bounding_object, query_type);
	return vector;
}

vector<ObjectSpatial*> SpatialContainer::all() const {
	vector<ObjectSpatial*> vector;
	this->appendAll(vector);
	return vector;
}