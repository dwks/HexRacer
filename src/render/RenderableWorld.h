#ifndef PROJECT_RENDER__RENDERABLE_WORLD_H
#define PROJECT_RENDER__RENDERABLE_WORLD_H

#include "CompositeRenderable.h"

namespace Project {
namespace Render {

class RenderableWorld : public CompositeRenderable {
public:
    virtual std::vector<RenderableObject*> getChildren() = 0;
};

}  // namespace Render
}  // namespace Project

#endif
