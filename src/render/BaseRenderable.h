#ifndef PROJECT_RENDER__BASE_RENDERABLE_H
#define PROJECT_RENDER__BASE_RENDERABLE_H

#include "RenderableObject.h"
#include "shader/ShaderParamSetter.h"

namespace Project {
namespace Render {

/** A renderable object that draws geometry
*/
class BaseRenderable
	: public RenderableObject {
public:
    virtual void preRenderUpdate(const Math::Matrix &transformation);
    
	void subRender(RenderManager* manager);
	virtual void renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object = NULL) = 0;

};

}  // namespace Render
}  // namespace Project

#endif
