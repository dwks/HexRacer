#ifndef PROJECT_RENDER__RENDERABLE_WORLD_H
#define PROJECT_RENDER__RENDERABLE_WORLD_H

#include <vector>
#include "object/ObjectBase.h"

#include "RenderableObject.h"

namespace Project {
namespace Render {

class RenderableWorld : public RenderableObject {
private:
    std::vector<Object::ObjectBase *> &objects;
public:
    RenderableWorld(std::vector<Object::ObjectBase *> &objects);

	void subRender(RenderManager* manager);
};

}  // namespace Render
}  // namespace Project

#endif
