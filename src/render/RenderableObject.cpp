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

	RenderableObject::RenderableObject(bool has_properties) {

		renderProperties = new RenderProperties();
		//createdRenderProperties = renderProperties;
		renderingEnabled = true;
	}

	RenderableObject::~RenderableObject() {
		//if (createdRenderProperties != NULL)
		delete(renderProperties);
	}

	void RenderableObject::render(RenderManager* manager) {

		if (!getRenderingEnabled())
			return;

		manager->setRenderProperties(this);
		subRender(manager);
		manager->revertRenderProperties(this);
	}

	/*
	bool RenderableObject::hasRenderProperties() const {
		return (renderProperties != NULL);
	}
	*/

	RenderProperties* RenderableObject::getRenderProperties() const {
		return renderProperties;
	}
	/*
	void RenderableObject::setRenderProperties(RenderProperties* properties) {
		renderProperties = properties;
	}
	*/

	void RenderableObject::clearRenderProperties() {
		/*
		if (renderProperties == createdRenderProperties
			&& createdRenderProperties != NULL) {
			delete(createdRenderProperties);
		}
		renderProperties = NULL;
		*/
	}

}  // namespace Render
}  // namespace Project
