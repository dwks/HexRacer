#include "Renderer.h"
#include "BaseRenderable.h"
#include "CompositeRenderable.h"
#include <typeinfo>
#include "opengl/OpenGL.h"
using namespace std;

namespace Project {
namespace Render {

	void Renderer::addRenderableObject(RenderableObject* renderable) {

		BaseRenderable* base = dynamic_cast<BaseRenderable*> renderable;
		if (base) {
			renderList.push_back(base);
			return;
		}

		CompositeRenderable* composite = dynamic_cast<CompositeRenderable*> renderable;
		if (composite) {
			//Composite object
			return;
		}
		
	}

	void Renderer::clearRenderList() {
		renderList.clear();
	}

	void Renderer::render() {
		for (int i = 0; i < renderList.size(); i++) {
			renderList[i]->render();
		}
	}

}  // namespace Render
}  // namespace Project
