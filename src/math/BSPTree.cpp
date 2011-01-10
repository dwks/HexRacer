#include "BSPTree.h"
#include "BoundingObject3D.h"
#include "BoundingObject2D.h"
using namespace Project;
using namespace Math;

BSPTree::BSPTree(int split_count)
{
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
			for (int i = 0; i < old_objects.size(); i++)
				add(old_objects[i]);
		}
	}

	for (short i = 0; i < 2; i++) {
		BoundingObject& b = child[i]->getBoundingObject();
		if (object->isInside(child[i]->getBoundingObject())) {
			child[i]->add(object);
			return true;
		}
	}

	list.add(object);
	return true;
}

bool BSPTree::remove(ObjectSpatial* object) {
	return false;
}

bool BSPTree::contains(ObjectSpatial* object) {
	return false;
}

vector<ObjectSpatial*> BSPTree::query(BoundingObject& bounding_object, QueryType query_type) {
	if (leaf)
		return list.query(bounding_object, query_type);
	else {
		vector<ObjectSpatial*> query_list;
		appendQuery(&query_list, bounding_object, query_type);
		return query_list;
	}
}

vector<ObjectSpatial*> BSPTree::all() {
	vector<ObjectSpatial*> return_list;

	vector<ObjectSpatial*> list_all = list.all();
	for (int i = 0; i < list_all.size(); i++)
		return_list.push_back(list_all[i]);

	if (!leaf) {
		for (short i = 0; i < 2; i++) {
			vector<ObjectSpatial*> child_list = child[i]->all();
			for (int j = 0; j < child_list.size(); j++) {
				return_list.push_back(child_list[j]);
			}
		}
	}

	return return_list;
}

int BSPTree::size() {
	if (leaf)
		return list.size();
	else
		return list.size()+subtreeSize;
}

void BSPTree::clear() {

}

void BSPTree::appendQuery(vector<ObjectSpatial*>* result_list, BoundingObject& bounding_object, QueryType query_type) {

	list.appendQuery(result_list, bounding_object, query_type);
	if (!leaf) {
		for (short i = 0; i < 2; i++) {

			bool append_child = false;

			BoundingObject& child_bound = child[i]->getBoundingObject();
			BoundingObject3D* bound_3D = dynamic_cast<BoundingObject3D*>(&child_bound);
			if (bound_3D) {
				append_child = bound_3D->intersects(bounding_object);
			}
			else {
				BoundingObject2D* bound_2D = dynamic_cast<BoundingObject2D*>(&child_bound);
				if (bound_2D) {
					append_child = bound_2D->intersects(bounding_object);
				}
			}

			if (append_child) {
				child[i]->appendQuery(result_list, bounding_object, query_type);
			}
			else {
				int b = 5;
			}
			
		}
	}

}