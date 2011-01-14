#ifndef PROJECT_RENDER__BASE_RENDERABLE_H
#define PROJECT_RENDER__BASE_RENDERABLE_H

#include "RenderableObject.h"
#include "ShaderParamSetter.h"

namespace Project {
namespace Render {

class BaseRenderable
	: public RenderableObject {
public:

	void subRender(RenderManager* manager);
	virtual void renderGeometry(ShaderParamSetter setter) = 0;

};

}  // namespace Render
}  // namespace Project

#endif
