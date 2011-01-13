#ifndef PROJECT_RENDER__COMPOSITE_RENDERABLE_H
#define PROJECT_RENDER__COMPOSITE_RENDERABLE_H

#include "RenderableObject.h"
#include <vector>

namespace Project {
namespace Render {

class CompositeRenderable
	: public RenderableObject {

	void renderGeometry();

private:

	virtual std::vector<RenderableObject*> getChildren() = 0;

};

}  // namespace Render
}  // namespace Project

#endif
