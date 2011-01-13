#ifndef PROJECT_RENDER__COMPOSITE_RENDERABLE_H
#define PROJECT_RENDER__COMPOSITE_RENDERABLE_H

#include "RenderListAccessor.h"

namespace Project {
namespace Render {

class CompositeRenderable {
public:

	virtual void addToRenderList(RenderListAccessor* accessor) = 0;

};

}  // namespace Render
}  // namespace Project

#endif
