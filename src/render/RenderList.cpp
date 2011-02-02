#include "RenderList.h"
#include "misc/StdVectorFunctions.h"

namespace Project {
namespace Render {

	void RenderList::addRenderable(RenderableObject* renderable) {
		if (renderable)
			renderableList.push_back(renderable);
	}

	bool RenderList::removeRenderable(RenderableObject* renderable) {
		return Misc::vectorRemoveOneElement(renderableList, renderable);
	}

	void RenderList::subRender(RenderManager* manager) {
		for (unsigned int i = 0; i < renderableList.size(); i++) {
			renderableList[i]->render(manager);
		}
	}

}  // namespace Render
}  // namespace Project
