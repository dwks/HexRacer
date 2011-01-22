#include "RenderableBox.h"
#include "opengl/GeometryDrawing.h"

namespace Project {
namespace Render {

	void RenderableBox::renderGeometry(ShaderParamSetter& setter, const Math::BoundingObject* bounding_object) {
		OpenGL::GeometryDrawing::drawBoundingBox3D(*this);
	}

	bool RenderableBox::shouldDraw( const Math::BoundingObject& bounding_obj ) {
		return intersects(bounding_obj);
	}

}  // namespace Render
}  // namespace Project
