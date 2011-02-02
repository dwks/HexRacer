#include "RenderableWorld.h"

namespace Project {
namespace Render {

RenderableWorld::RenderableWorld(std::vector<Object::ObjectBase *> &objects)
    : objects(objects) {
    
}

void RenderableWorld::subRender(RenderManager* manager) {
	for(std::vector<Object::ObjectBase *>::size_type x = 0; x < objects.size();
        x ++) {
        objects[x]->getRenderableObject()->render(manager);
    }
}

}  // namespace Render
}  // namespace Project
