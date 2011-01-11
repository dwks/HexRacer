#pragma once

#include "../math/ObjectSpatial.h"
#include "../math/BoundingBox3D.h"
#include "../math/BoundingBox2D.h"
#include "../math/BoundingTriangle2D.h"

namespace Project {
namespace OpenGL {

class GeometryDrawing
{
public:
	GeometryDrawing(void);
	~GeometryDrawing(void);

	static void drawObject(Math::ObjectSpatial& object, bool wireframe = false);
	static void drawBoundingObject(Math::BoundingObject& object, bool wireframe = false);
	static void drawBoundingBox3D(Math::BoundingBox3D& object, bool wireframe = false);
	static void drawBoundingBox2D(Math::BoundingBox2D& object, bool wireframe = false);
	static void drawBoundingTriangle2D(Math::BoundingTriangle2D& object, bool wireframe = false);
};

}  // namespace OpenGL
}  // namespace Project