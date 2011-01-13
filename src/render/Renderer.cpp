#include "Renderer.h"
#include "CompositeRenderable.h"
#include <typeinfo>
#include "opengl/OpenGL.h"
using namespace std;

namespace Project {
namespace Render {

	void Renderer::addRenderableObject(RenderableObject* renderable) {

		renderList.push_back(renderable);
		
	}

	void Renderer::clearRenderList() {
		renderList.clear();
	}

	void Renderer::render() {
		for (unsigned int i = 0; i < renderList.size(); i++) {
			renderList[i]->render();
		}
	}

}  // namespace Render
}  // namespace Project
