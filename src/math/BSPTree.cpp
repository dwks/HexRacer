#include "BSPTree.h"
#include "BoundingObject3D.h"
#include "BoundingObject2D.h"
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

	for (short i = 0; i < 2; i++) {
		if (object->isInside(child[i]->getBoundingObject())) {
			child[i]->add(object);
			return true;
		}
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
		list.add(add_to_this);
		
	}
	
}

bool BSPTree::remove(ObjectSpatial* object) {
	return false;
}

bool BSPTree::contains(ObjectSpatial* object) const {
	return false;
}

vector<ObjectSpatial*> BSPTree::query(const BoundingObject& bounding_object, QueryType query_type) const {
	if (leaf)
		return list.query(bounding_object, query_type);
	else {
		vector<ObjectSpatial*> query_list;
		appendQuery(&query_list, bounding_object, query_type);
		return query_list;
	}
}

vector<ObjectSpatial*> BSPTree::all() const {
	vector<ObjectSpatial*> return_list;

	vector<ObjectSpatial*> list_all = list.all();
	for (unsigned int i = 0; i < list_all.size(); i++)
		return_list.push_back(list_all[i]);

	if (!leaf) {
		for (short i = 0; i < 2; i++) {
			vector<ObjectSpatial*> child_list = child[i]->all();
			for (unsigned int j = 0; j < child_list.size(); j++) {
				return_list.push_back(child_list[j]);
			}
		}
	}

	return return_list;
}

int BSPTree::size() const {
	if (leaf)
		return list.size();
	else
		return list.size()+subtreeSize;
}

void BSPTree::clear() {

}

void BSPTree::appendQuery(vector<ObjectSpatial*>* result_list, const BoundingObject& bounding_object, QueryType query_type) const {

	list.appendQuery(result_list, bounding_object, query_type);
	if (!leaf) {
		for (short i = 0; i < 2; i++) {

			bool append_child = false;

			const BoundingObject& child_bound = child[i]->getBoundingObject();
			const BoundingObject3D* bound_3D = dynamic_cast<const BoundingObject3D*>(&child_bound);
			if (bound_3D) {
				append_child = bound_3D->intersects(bounding_object);
			}
			else {
				const BoundingObject2D* bound_2D = dynamic_cast<const BoundingObject2D*>(&child_bound);
				if (bound_2D) {
					append_child = bound_2D->intersects(bounding_object);
				}
			}

			if (append_child) {
				child[i]->appendQuery(result_list, bounding_object, query_type);
			}
		}
	}

}