#include "RenderableWorld.h"

namespace Project {
namespace Render {

RenderableWorld::RenderableWorld(std::vector<Object::ObjectBase *> &objects)
    : objects(objects) {
    
}

std::vector<RenderableObject*> RenderableWorld::getChildren() {
    std::vector<RenderableObject*> renderableObjects;
    
    for(std::vector<Object::ObjectBase *>::size_type x = 0; x < objects.size();
        x ++) {
        
        renderableObjects.push_back(objects[x]->getRenderableObject());
    }
    
    return renderableObjects;
}

}  // namespace Render
}  // namespace Project
