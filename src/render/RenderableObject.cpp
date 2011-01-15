#include "RenderableObject.h"
#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"
#include "math/Matrix.h"
#include "RenderManager.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	RenderableObject::RenderableObject() {
		renderProperties = new RenderProperties();
		originalRenderProperties = renderProperties;
	}

	RenderableObject::~RenderableObject() {
		delete(originalRenderProperties);
	}

	void RenderableObject::render(RenderManager* manager) {
		manager->setRenderProperties(this);
		setShaderParams(manager->getShaderParamSetter());

		subRender(manager);
	
		manager->revertRenderProperties(this);
	}

	bool RenderableObject::hasRenderProperties() const {
		return (renderProperties != NULL);
	}

	RenderProperties* RenderableObject::getRenderProperties() const {
		return renderProperties;
	}
	
	void RenderableObject::setRenderProperties(RenderProperties* properties) {
		renderProperties = properties;
	}

	void RenderableObject::clearRenderProperties() {
		renderProperties = NULL;
	}

}  // namespace Render
}  // namespace Project
