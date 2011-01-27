#ifndef PROJECT_RENDER__RENDERABLE_WORLD_H
#define PROJECT_RENDER__RENDERABLE_WORLD_H

#include <vector>
#include "object/ObjectBase.h"

#include "CompositeRenderable.h"

namespace Project {
namespace Render {

class RenderableWorld : public CompositeRenderable {
private:
    std::vector<Object::ObjectBase *> &objects;
public:
    RenderableWorld(std::vector<Object::ObjectBase *> &objects);
    
    virtual std::vector<RenderableObject*> getChildren();
};

}  // namespace Render
}  // namespace Project

#endif
