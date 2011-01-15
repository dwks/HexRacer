#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "RenderableObject.h"

namespace Project {
namespace Render {

class RenderablePlayer : public RenderableObject {
private:
    Math::Point origin;
public:
    void updatePhysicalData(const Math::Point &origin);
    
    virtual void renderGeometry(ShaderParamSetter setter) = 0;
};

}  // namespace Render
}  // namespace Project

#endif
