#ifndef PROJECT_RENDER__RENDERER_H
#define PROJECT_RENDER__RENDERER_H

#include "RenderableObject.h"
#include "BaseRenderable.h"
#include "RenderListAccessor.h"
#include <vector>

namespace Project {
namespace Render {

class Renderer {

public:

	void addRenderableObject(RenderableObject* renderable);
	void clearRenderList();
	void render();

private:

	std::vector<BaseRenderable*> renderList;
};

}  // namespace Render
}  // namespace Project

#endif
