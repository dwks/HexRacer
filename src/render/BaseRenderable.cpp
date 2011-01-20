#include "BaseRenderable.h"
#include "RenderManager.h"

namespace Project {
namespace Render {

	void BaseRenderable::subRender(RenderManager* manager) {
		renderGeometry(manager->getShaderParamSetter(), manager->getBoundingObject());
	}

}  // namespace Render
}  // namespace Project
