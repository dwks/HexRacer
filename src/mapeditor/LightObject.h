#pragma once
#include "MapObject.h"
#include "render/Light.h"
using namespace Project;
using namespace Render;
using namespace Math;

class LightObject :
	public MapObject
{
private:
	Light* light;
public:
	LightObject(Light* _light)
		: light(_light) {}

	Point getPosition() const { return light->getPosition(); }
	void setPosition(Point pos) { light->moveCentroid(pos); }
	void translate(const Point& translation) { light->translate(translation); }

	Light* getLight() { return light; }
	BoundingBox3D getBoundingBox() const;

	MapObject::ObjectType getType() const { return MapObject::LIGHT; }
};
