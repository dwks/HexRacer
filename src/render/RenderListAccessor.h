#ifndef PROJECT_RENDER__RENDER_LIST_ACCESSOR_H
#define PROJECT_RENDER__RENDER_LIST_ACCESSOR_H

#include "BaseRenderable.h"

namespace Project {
namespace Render {

class RenderListAccessor {
public:

	void addToRenderList(BaseRenderable* renderable);
};

}  // namespace Render
}  // namespace Project

#endif
