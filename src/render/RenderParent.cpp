#include "RenderParent.h"

namespace Project {
namespace Render {

	RenderParent::RenderParent(RenderableObject* _child) {
		child = _child;
	}

	void RenderParent::subRender(RenderManager* manager) {
		if (child) {
			child->render(manager);
		}
	}

}  // namespace Render
}  // namespace Project
