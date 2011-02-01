#ifndef PROJECT_RENDER__COMPOSITE_RENDERABLE_H
#define PROJECT_RENDER__COMPOSITE_RENDERABLE_H

#include "RenderableObject.h"
#include <vector>

namespace Project {
namespace Render {

/** A collection of several renderable objects
*/
class CompositeRenderable
	: public RenderableObject {

    virtual void preRenderUpdate(const Math::Matrix &transformation);
    
	void subRender(RenderManager* manager);
	virtual const std::vector<RenderableObject*>& getChildren() = 0;

};

}  // namespace Render
}  // namespace Project

#endif
