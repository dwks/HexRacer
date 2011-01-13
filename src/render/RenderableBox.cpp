#include "RenderableBox.h"
#include "opengl/GeometryDrawing.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	RenderableBox::RenderableBox(BoundingBox3D box) {
		this->box = box;
	}

	void RenderableBox::renderGeometry() {
		GeometryDrawing::drawBoundingBox3D(box);
	}

}  // namespace Render
}  // namespace Project
