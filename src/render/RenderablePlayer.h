#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "BaseRenderable.h"

namespace Project {
namespace Render {

class RenderablePlayer : public BaseRenderable {
private:
    Math::Point origin;
public:
    virtual void updatePhysicalData(const Math::Point &origin);
    
    virtual void renderGeometry(ShaderParamSetter setter) = 0;
};

}  // namespace Render
}  // namespace Project

#endif
