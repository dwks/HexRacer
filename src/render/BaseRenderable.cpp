#include "BaseRenderable.h"
#include "RenderManager.h"

namespace Project {
namespace Render {

    void BaseRenderable::preRenderUpdate(const Math::Matrix &transformation) {
        getRenderProperties()->setTransformation(transformation);
    }
    
	void BaseRenderable::subRender(RenderManager* manager) {
        ShaderParamSetter param = manager->getShaderParamSetter();
		if (manager->hasTransformation())
			renderGeometry(param, NULL);
		else
			renderGeometry(param, manager->getBoundingObject());
	}

}  // namespace Render
}  // namespace Project
