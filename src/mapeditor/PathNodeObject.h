#pragma once

#include "MapObject.h"
#include "map/PathNode.h"
#include <vector>
using namespace Project;
using namespace Math;
using namespace Map;
using namespace std;

class PathNodeObject
	: public MapObject
{
private:
	PathNode* node;
public:

	PathNodeObject(PathNode* _node)
		: node(_node) {}

	Point getPosition() const { return node->getPosition(); }
	void setPosition(const Point& new_position) { node->moveCentroid(new_position); }
	void translate(const Point& translation) { node->translate(translation); }

	BoundingBox3D getBoundingBox() const;

	const vector<PathNode*>& getNextNodes() const { return node->getNextNodes(); }
	PathNode* getNode() { return node; }
	void linkNode(PathNodeObject* node_object);

	MapObject::ObjectType getType() const { return MapObject::PATH_NODE; }
};
