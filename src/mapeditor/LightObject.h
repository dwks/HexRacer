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
	void setPosition(const Point& new_position) { light->moveCentroid(new_position); }
	void translate(const Point& translation) { light->translate(translation); }

	bool hasColors() const { return true; }
	void setDiffuse(const Color& color) { light->setDiffuse(color); }
	void setSpecular(const Color& color) { light->setSpecular(color); }
	void setAmbient(const Color& color) { light->setAmbient(color); }
	Color getDiffuse() const { return light->getDiffuse(); }
	Color getSpecular() const { return light->getSpecular(); }
	Color getAmbient() const { return light->getAmbient(); }

	Light* getLight() { return light; }
	BoundingBox3D getBoundingBox() const;

	MapObject::ObjectType getType() const { return MapObject::LIGHT; }
};
