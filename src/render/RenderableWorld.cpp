#include "RenderableWorld.h"

namespace Project {
namespace Render {

RenderableWorld::RenderableWorld(std::vector<Object::ObjectBase *> &objects)
    : objects(objects) {
    
}

const std::vector<RenderableObject*>& RenderableWorld::getChildren() {

    renderableObjects.clear();
    for(std::vector<Object::ObjectBase *>::size_type x = 0; x < objects.size();
        x ++) {
        
        renderableObjects.push_back(objects[x]->getRenderableObject());
    }
    
    return renderableObjects;
}

}  // namespace Render
}  // namespace Project
