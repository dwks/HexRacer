#include "BSPTree.h"
#include "BoundingObject3D.h"
#include "BoundingObject2D.h"
#include "Values.h"
using namespace Project;
using namespace Math;

BSPTree::BSPTree(int split_count)
{
	list = SpatialList(true);
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
			vector<ObjectSpatial*> old_objects = list.all();
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

void BSPTree::add(vector<ObjectSpatial*> objects) {

	if (leaf) {
		if (size()+static_cast<int>(objects.size()) <= splitCount || !allowSplit()) {
			list.add(objects);
			return;
		}
		else {
			vector<ObjectSpatial*> old_objects = list.all();
			list.clear();
			for (unsigned int i = 0; i < old_objects.size(); i++)
				objects.push_back(old_objects[i]);
			split(&objects);
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
		if (object->isInside(child[0]->getBoundingObject())) {
			return child[0]->remove(object);
		}
		else if (object->isInside(child[1]->getBoundingObject())) {
			return child[1]->remove(object);
		}
	}

	return false;
}

bool BSPTree::contains(ObjectSpatial* object) const {

	if (list.contains(object))
		return true;

	if (!leaf) {
		if (object->isInside(child[0]->getBoundingObject())) {
			return child[0]->contains(object);
		}
		else if (object->isInside(child[1]->getBoundingObject())) {
			return child[1]->contains(object);
		}
	}

	return false;
}

vector<ObjectSpatial*> BSPTree::query(const BoundingObject& bounding_object, QueryType query_type) const {

	vector<ObjectSpatial*> query_list;
	appendQuery(&query_list, bounding_object, query_type);
	return query_list;

}

vector<ObjectSpatial*> BSPTree::all() const {
	vector<ObjectSpatial*> return_list;
	appendAll(&return_list);
	return return_list;
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

void BSPTree::appendQuery(vector<ObjectSpatial*>* result_list, const BoundingObject& bounding_object, QueryType query_type) const {

	if (size() <= 0)
		return;

	const ObjectSpatial& this_bound = (const BoundingObject2D&) getBoundingObject();
	if (!this_bound.intersects(bounding_object))
		return;
	else if (this_bound.isInside(bounding_object)) {
		appendAll(result_list);
		return;
	}

	list.appendQuery(result_list, bounding_object, query_type);
	if (!leaf) {
		child[0]->appendQuery(result_list, bounding_object, query_type);
		child[1]->appendQuery(result_list, bounding_object, query_type);
	}

}

void BSPTree::appendAll(vector<ObjectSpatial*>* result_list) const {
	
	list.appendAll(result_list);
	if (!leaf) {
		child[0]->appendAll(result_list);
		child[1]->appendAll(result_list);
	}
}

int BSPTree::getHeight() const {
	if (leaf)
		return 1;
	else
		return (maximum(child[0]->getHeight(), child[1]->getHeight())+1);
}