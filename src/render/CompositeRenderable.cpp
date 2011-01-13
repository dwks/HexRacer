#include "CompositeRenderable.h"
using namespace std;

namespace Project {
namespace Render {

	void CompositeRenderable::renderGeometry() {

		vector<RenderableObject*> children = getChildren();

		for (unsigned int i = 0; i < children.size(); i++) {
			children[i]->render();
		}

	}

}  // namespace Render
}  // namespace Project
