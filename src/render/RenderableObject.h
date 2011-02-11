#ifndef PROJECT_RENDER__RENDERABLE_OBJECT_H
#define PROJECT_RENDER__RENDERABLE_OBJECT_H

#include "RenderProperties.h"
#include "shader/ShaderParamSetter.h"
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
    
    /** Sets the transformation of this RenderableObject based on its physical
        state.
    */
    virtual void preRenderUpdate(const Math::Matrix &transformation);
    
	void render(RenderManager* manager);
	virtual void subRender(RenderManager* manager) = 0;

	virtual bool shouldDraw(const Math::BoundingObject& bounding_obj) { return true; }

	RenderProperties* getRenderProperties() const;

	void setRenderingEnabled(bool enabled) { renderingEnabled = enabled; }
	bool getRenderingEnabled() const { return renderingEnabled; }
	
};

}  // namespace Render
}  // namespace Project

#endif
