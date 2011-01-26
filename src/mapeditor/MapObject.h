#pragma once

#include "math/Point.h"
#include "math/BoundingBox3D.h"
#include "opengl/Color.h"
#include <string>
using namespace Project;
using namespace Math;
using namespace OpenGL;
using namespace std;

class MapObject {
public:

	static const int NUM_OBJECT_TYPES = 4;

	enum ObjectType {LIGHT, MESH_INSTANCE, PATH_NODE, START_POINT};

	virtual void translate(const Point& translation) = 0;
	virtual ObjectType getType() const = 0;
	virtual BoundingBox3D getBoundingBox() const = 0;

	static string typeTitle(ObjectType type);
};
