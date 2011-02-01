#include "RenderList.h"

namespace Project {
namespace Render {

	void RenderList::addRenderable(RenderableObject* renderable) {
		if (renderable)
			renderableList.push_back(renderable);
	}

	bool RenderList::removeRenderable(RenderableObject* renderable) {
		for (unsigned int i = 0; i < renderableList.size(); i++) {
			if (renderableList[i] == renderable) {
				renderableList[i] = renderableList[renderableList.size()-1];
				renderableList.resize(renderableList.size()-1);
				return true;
			}
		}
		return false;
	}

}  // namespace Render
}  // namespace Project
