#ifndef PROJECT_RENDER__RENDERABLE_OBJECT_H
#define PROJECT_RENDER__RENDERABLE_OBJECT_H

#include "RenderProperties.h"
#include "ShaderParamSetter.h"

namespace Project {
namespace Render {

class RenderManager;

class RenderableObject {

public:

	RenderableObject();
	virtual ~RenderableObject();

	void render(RenderManager* manager);
	virtual void subRender(RenderManager* manager) = 0;
	virtual void setShaderParams(ShaderParamSetter setter) {}

	bool hasRenderProperties() const;
	RenderProperties* getRenderProperties() const;
	void setRenderProperties(RenderProperties* properties);
	void clearRenderProperties();

private:
	RenderProperties* renderProperties;
	RenderProperties* originalRenderProperties;
	
};

}  // namespace Render
}  // namespace Project

#endif
