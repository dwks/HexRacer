#pragma once

#include "math/ObjectSpatial.h"
#include "math/BoundingBox3D.h"
#include "math/BoundingBox2D.h"
#include "math/BoundingCircle.h"
#include "math/BoundingTriangle2D.h"
#include "math/Triangle3D.h"
#include "math/BoundingPlane3D.h"
#include "math/BoundingConvexHull3D.h"
#include "math/BoundingSphere.h"
#include "math/Vertex2D.h"

namespace Project {
namespace OpenGL {

/** Functions for drawing spatial objects
*/
class GeometryDrawing
{
public:
	GeometryDrawing(void);
	~GeometryDrawing(void);

	static void drawObject(const Math::ObjectSpatial& object, bool wireframe = false);
	static void drawBoundingObject(const Math::BoundingObject& object, bool wireframe = false);
	static void drawBoundingBox3D(const Math::BoundingBox3D& object, bool wireframe = false);
	static void drawBoundingBox2D(const Math::BoundingBox2D& object, bool wireframe = false);
	static void drawBoundingCircle(const Math::BoundingCircle& object, bool wireframe = false);
	static void drawBoundingTriangle2D(const Math::BoundingTriangle2D& object, bool wireframe = false);
	static void drawTriangle3D(const Math::Triangle3D& object, bool wireframe = false);
	static void drawBoundingPlane3D(const Math::BoundingPlane3D& object, bool wireframe = false);
	static void drawBoundingConvexHull3D(const Math::BoundingConvexHull3D& object, bool wireframe = false);
	static void drawBoundingSphere(const Math::BoundingSphere& object, bool wireframe = false);

	static void drawHexagon(const Math::Vertex2D& center, double radius, bool edge_only = false);
};

}  // namespace OpenGL
}  // namespace Project
