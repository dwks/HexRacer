#include "BaseRenderable.h"
#include "RenderManager.h"

namespace Project {
namespace Render {

    void BaseRenderable::preRenderUpdate(const Math::Matrix &transformation) {
        getRenderProperties()->setTransformation(transformation);
    }
    
	void BaseRenderable::subRender(RenderManager* manager) {
		if (manager->hasTransformation())
			renderGeometry(manager->getShaderManager()->getShaderParamSetter(), NULL, *manager->getRenderSettings());
		else
			renderGeometry(manager->getShaderManager()->getShaderParamSetter(), manager->getBoundingObject(), *manager->getRenderSettings());
	}

}  // namespace Render
}  // namespace Project
