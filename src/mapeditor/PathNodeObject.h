#pragma once

#include "MapObject.h"
#include <vector>
using namespace Project;
using namespace Math;
using namespace std;

class PathNodeObject
	: public MapObject
{
private:
	Point position;
	vector<PathNodeObject*> nextNodes;
public:

	PathNodeObject(const Point& _position = Point())
		: position(_position) {}

	Point getPosition() const { return position; }
	void setPosition(const Point& new_position) { position = new_position; }
	void translate(const Point& translation) { position += translation; }

	bool hasColors() const { return false; }
	BoundingBox3D getBoundingBox() const;

	vector<PathNodeObject*> getNextNodes() const { return nextNodes; }
	MapObject::ObjectType getType() const { return MapObject::PATH_NODE; }
};
