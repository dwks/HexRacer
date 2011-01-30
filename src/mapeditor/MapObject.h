#pragma once

#include "math/Point.h"
#include "math/BoundingBox3D.h"
#include "math/RotationAxis.h"
#include "opengl/Color.h"
#include <string>
using namespace Project;
using namespace Math;
using namespace OpenGL;
using namespace std;

class MapObject {
public:

	static const int NUM_OBJECT_TYPES = 5;
	enum ObjectType {LIGHT, MESH_INSTANCE, PATH_NODE, START_POINT, FINISH_PLANE};

	virtual void translate(const Point& translation) = 0;
	virtual ObjectType getType() const = 0;
	virtual BoundingBox3D getBoundingBox() const = 0;
	virtual Point getPosition() const = 0;
	virtual void setPosition(const Point& new_position) = 0;

	virtual bool hasRotation() const { return false; }
	virtual double getRotation(RotationAxis axis) {return 0.0;}
	virtual void setRotation(double radians, RotationAxis axis) {}
	virtual void rotate(double radians, RotationAxis axis ) { setRotation(getRotation(axis)+radians, axis); }

	virtual bool hasColors() const { return false; }
	virtual void setDiffuse(const Color& color) {}
	virtual void setSpecular(const Color& color) {}
	virtual void setAmbient(const Color& color) {}
	virtual Color getDiffuse() const { return Color(); }
	virtual Color getSpecular() const { return Color(); }
	virtual Color getAmbient() const { return Color(); }

	virtual void setColor(int color_index, const Color& color) {
		switch (color_index) {
			case 0:
				setDiffuse(color);
				break;
			case 1:
				setSpecular(color);
				break;
			case 2:
				setAmbient(color);
				break;
			case 3:
				break;
		}
	}

	virtual Color getColor(int color_index) const {
		switch (color_index) {
			case 0:
				return getDiffuse();
			case 1:
				return getSpecular();
			case 2:
				return getAmbient();
			default:
				return Color();
		}
	}

	void setPositionCoord(double value, Axis axis) {
		Point pos = getPosition();
		pos.setCoord(value, axis);
		setPosition(pos);
	}
	

	static string typeTitle(ObjectType type);
};
