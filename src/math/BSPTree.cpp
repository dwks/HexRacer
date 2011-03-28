#include "BSPTree.h"
#include "BoundingObject3D.h"
#include "BoundingObject2D.h"
#include "BoundingSphere.h"
#include "Values.h"
#include <math.h>
using namespace Project;
using namespace Math;

BSPTree::BSPTree(int split_count)
	: list(true) {
	splitCount = split_count;
	leaf = true;
	subtreeSize = 0;
}

BSPTree::~BSPTree(void)
{
	if (!leaf) {
		delete(child[0]);
		delete(child[1]);
	}
}

bool BSPTree::add(ObjectSpatial* object) {

	if (leaf) {
		if (size() < splitCount || !allowSplit()) {
			list.add(object);
			return true;
		}
		else {
			split();
			vector<ObjectSpatial*> old_objects;
			appendAll(old_objects);
			list.clear();
			for (unsigned int i = 0; i < old_objects.size(); i++)
				add(old_objects[i]);
		}
	}

	if (object->isInside(child[0]->getBoundingObject())) {
		child[0]->add(object);
		subtreeSize++;
		return true;
	} else if (object->isInside(child[1]->getBoundingObject())) {
		child[1]->add(object);
		subtreeSize++;
		return true;
	}

	list.add(object);
	return true;
}

void BSPTree::add(const vector<ObjectSpatial*>& objects) {

	if (leaf) {
		if (size()+static_cast<int>(objects.size()) <= splitCount || !allowSplit()) {
			list.add(objects);
			return;
		}
		else {

			vector<ObjectSpatial*> insert_objects = vector<ObjectSpatial*>(objects);
			list.appendAll(insert_objects);
			list.clear();
			split(&insert_objects);
			add(insert_objects);
			return;
		}
	}
	
	if (!leaf) {

		vector<ObjectSpatial*> add_to_child_0;
		vector<ObjectSpatial*> add_to_child_1;
		vector<ObjectSpatial*> add_to_this;

		for (unsigned int i = 0; i < objects.size(); i++) {

			if (objects[i]->isInside(child[0]->getBoundingObject()))
				add_to_child_0.push_back(objects[i]);
			else if (objects[i]->isInside(child[1]->getBoundingObject()))
				add_to_child_1.push_back(objects[i]);
			else
				add_to_this.push_back(objects[i]);

		}

		child[0]->add(add_to_child_0);
		child[1]->add(add_to_child_1);
		subtreeSize += add_to_child_0.size();
		subtreeSize += add_to_child_1.size();
		list.add(add_to_this);
	}
	
}

bool BSPTree::remove(ObjectSpatial* object) {

	if (list.remove(object))
		return true;

	if (!leaf) {

		bool removed = false;

		if (object->isInside(child[0]->getBoundingObject())) {
			if (child[0]->remove(object)) {
				subtreeSize--;
				removed = true;
			}
		}
		else if (object->isInside(child[1]->getBoundingObject())) {
			if (child[1]->remove(object)) {
				subtreeSize--;
				removed = true;
			}
		}

		if (removed && size() < splitCount/2) {
			vector<ObjectSpatial*> child_objects;

			child[0]->appendAll(child_objects);
			child[1]->appendAll(child_objects);

			delete(child[0]);
			delete(child[1]);

			subtreeSize = 0;
			list.add(child_objects);

			leaf = true;
		}

		return removed;
	}

	return false;

}

bool BSPTree::contains(ObjectSpatial* object) const {

	if (!leaf) {
		if (object->isInside(child[0]->getBoundingObject())) {
			return child[0]->contains(object);
		}
		else if (object->isInside(child[1]->getBoundingObject())) {
			return child[1]->contains(object);
		}
	}

	return list.contains(object);
}

ObjectSpatial* BSPTree::nearestSquared(const Point& point, double max_distance_squared, bool bounded) const {

	ObjectSpatial* nearest = list.nearestSquared(point, max_distance_squared, bounded);

	if (nearest) {
		max_distance_squared = point.distanceSquared(nearest->centroid());
		bounded = true;
	}

	if (!leaf) {
		BoundingSphere sphere = BoundingSphere(point, sqrt(max_distance_squared));

		if (!bounded || sphere.intersects(child[0]->getBoundingObject())) {
			ObjectSpatial* child_nearest = child[0]->nearestSquared(point, max_distance_squared, bounded);
			if (child_nearest) {
				nearest = child_nearest;
				max_distance_squared = point.distanceSquared(nearest->centroid());
				bounded = true;
				sphere.setRadius(sqrt(max_distance_squared));
			}
		}

		if (!bounded || sphere.intersects(child[1]->getBoundingObject())) {
			ObjectSpatial* child_nearest = child[1]->nearestSquared(point, max_distance_squared, bounded);
			if (child_nearest) {
				nearest = child_nearest;
			}
		}

	}

	return nearest;

}

RayIntersection BSPTree::rayIntersection(Ray ray) const {

	bool intersects = getBoundingObject().toConstObjectSpatial().rayIntersection(ray).intersects;

	if (size() == 0 || !intersects)
		return RayIntersection();

	RayIntersection min_intersect = list.rayIntersection(ray);

	if (leaf)
		return min_intersect;

	if (min_intersect.intersects) {
		ray.maxT = min_intersect.t;
		ray.maxBounded = true;
	}

	RayIntersection child_0_intersect = child[0]->getBoundingObject().toConstObjectSpatial().rayIntersection(ray);
	RayIntersection child_1_intersect = child[1]->getBoundingObject().toConstObjectSpatial().rayIntersection(ray);

	if (child_0_intersect.intersects && (!min_intersect.intersects || child_0_intersect < min_intersect)) {
		child_0_intersect = child[0]->rayIntersection(ray);
		if (child_0_intersect.intersects && (!min_intersect.intersects || child_0_intersect < min_intersect)) {
			min_intersect = child_0_intersect;
		}
	}

	if (child_1_intersect.intersects && (!min_intersect.intersects || child_1_intersect < min_intersect)) {
		child_1_intersect = child[1]->rayIntersection(ray);
		if (child_1_intersect.intersects && (!min_intersect.intersects || child_1_intersect < min_intersect)) {
			min_intersect = child_1_intersect;
		}
	}

	return min_intersect;

}

int BSPTree::size() const {
	if (leaf)
		return list.size();
	else
		return list.size()+subtreeSize;
}

void BSPTree::clear() {
	list.clear();
	if (!leaf) {
		delete(child[0]);
		delete(child[1]);
		subtreeSize = 0;
		leaf = true;
	}
}

void BSPTree::appendQuery(vector<ObjectSpatial*>& result_vector, const BoundingObject& bounding_object, QueryType query_type) const {

	if (size() <= 0)
		return;

	ObjectSpatial::IntersectionType intersect;

	intersect = getBoundingObject().toConstObjectSpatial().intersectionType(bounding_object);

	switch (intersect) {

		case ObjectSpatial::INTERSECT_INSIDE:
			appendAll(result_vector);
			return;

		case ObjectSpatial::INTERSECT_INTERSECTS:
			list.appendQuery(result_vector, bounding_object, query_type);
			if (!leaf) {
				child[0]->appendQuery(result_vector, bounding_object, query_type);
				child[1]->appendQuery(result_vector, bounding_object, query_type);
			}
			return;

		default:
			return;
			
	}

}

void BSPTree::appendAll(vector<ObjectSpatial*>& result_vector) const {
	
	list.appendAll(result_vector);
	if (!leaf) {
		child[0]->appendAll(result_vector);
		child[1]->appendAll(result_vector);
	}
}


void BSPTree::operateQuery(SpatialObjectOperator& op, const BoundingObject& bounding_object, QueryType query_type ) const {

	if (size() <= 0)
		return;

	ObjectSpatial::IntersectionType intersect;

	intersect = getBoundingObject().toConstObjectSpatial().intersectionType(bounding_object);

	switch (intersect) {

		case ObjectSpatial::INTERSECT_INSIDE:
			operateAll(op);
			return;

		case ObjectSpatial::INTERSECT_INTERSECTS:
			list.operateQuery(op, bounding_object, query_type);
			if (!leaf) {
				child[0]->operateQuery(op, bounding_object, query_type);
				child[1]->operateQuery(op, bounding_object, query_type);
			}
			return;

		default:
			return;
			
	}

}

void BSPTree::operateAll(SpatialObjectOperator& op) const {
	list.operateAll(op);
	if (!leaf) {
		child[0]->operateAll(op);
		child[1]->operateAll(op);
	}
}

int BSPTree::getHeight() const {
	if (leaf)
		return 1;
	else
		return (maximum(child[0]->getHeight(), child[1]->getHeight())+1);
}