#include "RenderableObject.h"
#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"
#include "math/Matrix.h"
#include "RenderManager.h"
#include "log/Logger.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	RenderableObject::RenderableObject(bool has_properties) {

		renderProperties = new RenderProperties();
		renderingEnabled = true;
	}

	RenderableObject::~RenderableObject() {
		delete(renderProperties);
	}

	void RenderableObject::render(RenderManager* manager) {

		const BoundingObject* draw_bounding_obj = manager->getBoundingObject();

		//Don't draw if rendering is disabled or not in the renderer's bounding object
		if (!getRenderingEnabled() ||
			(!manager->hasTransformation() &&
			draw_bounding_obj != NULL &&
			!shouldDraw(*draw_bounding_obj))
			)
			return;

		manager->setRenderProperties(this);
		subRender(manager);
		manager->revertRenderProperties(this);
	}

	RenderProperties* RenderableObject::getRenderProperties() const {
		return renderProperties;
	}

	void RenderableObject::preRenderUpdate(const Math::Matrix &transformation) {
		LOG(OPENGL, "Warning: preRenderUpdate() called on a renderable without a definition");
	}

}  // namespace Render
}  // namespace Project
