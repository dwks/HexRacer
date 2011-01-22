#include "BaseRenderable.h"
#include "RenderManager.h"

namespace Project {
namespace Render {

	void BaseRenderable::subRender(RenderManager* manager) {
		if (manager->hasTransformation())
			renderGeometry(manager->getShaderParamSetter(), NULL);
		else
			renderGeometry(manager->getShaderParamSetter(), manager->getBoundingObject());
	}

}  // namespace Render
}  // namespace Project
