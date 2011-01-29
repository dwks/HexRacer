#pragma once

#include "MapObject.h"
#include "math/Vertex3D.h"
using namespace Project;
using namespace Math;

class StartPointObject
	: public MapObject
{
private:
	Vertex3D* point;
public:
	StartPointObject(Vertex3D* _point)
		: point(_point) {}
	~StartPointObject(void);

	Point getPosition() const { return point->centroid(); }
	void setPosition(const Point& new_position) { point->moveCentroid(new_position); }
	void translate(const Point& translation) { point->translate(translation); }

	BoundingBox3D getBoundingBox() const;

	bool hasColors() const { return false; }
	MapObject::ObjectType getType() const { return MapObject::START_POINT; }
};
