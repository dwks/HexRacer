#ifndef PROJECT_RENDER__RENDERABLE_OBJECT_H
#define PROJECT_RENDER__RENDERABLE_OBJECT_H

#include "RenderProperties.h"
#include "ShaderParamSetter.h"
#include "math/BoundingObject.h"

namespace Project {
namespace Render {

class RenderManager;

/** An object that can be rendered with given properties
*/
class RenderableObject {

private:
	RenderProperties* renderProperties;
	RenderProperties* createdRenderProperties;
	bool renderingEnabled;

public:

	RenderableObject(bool has_properties = true);
	virtual ~RenderableObject();

	void render(RenderManager* manager);
	virtual void subRender(RenderManager* manager) = 0;

	virtual bool intersects( const Math::BoundingObject& bound_obj ) { return true; }

	//bool hasRenderProperties() const;
	RenderProperties* getRenderProperties() const;
	//void setRenderProperties(RenderProperties* properties);
	void clearRenderProperties();

	void setRenderingEnabled(bool enabled) { renderingEnabled = enabled; }
	bool getRenderingEnabled() const { return renderingEnabled; }
	
};

}  // namespace Render
}  // namespace Project

#endif
