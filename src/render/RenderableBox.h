#ifndef PROJECT_RENDER__RENDERABLE_BOX_H
#define PROJECT_RENDER__RENDERABLE_BOX_H

#include "BaseRenderable.h"
#include "math/BoundingBox3D.h"

namespace Project {
namespace Render {

class RenderableBox
	: public BaseRenderable, public Math::BoundingBox3D {
public:
	RenderableBox(double width, double height, double depth, Math::Point centroid = Math::Point())
		: Math::BoundingBox3D(width, height, depth, centroid) {}

	void renderGeometry(ShaderParamSetter& setter, const Math::BoundingObject* bounding_object = NULL);
	bool shouldDraw( const Math::BoundingObject& bound_obj );
	
};

}  // namespace Render
}  // namespace Project

#endif
