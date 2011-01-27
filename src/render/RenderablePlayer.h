#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "RenderList.h"

namespace Project {
namespace Render {

class RenderablePlayer : public RenderList {
private:
    Math::Point origin;
public:
    virtual void initialize(int id);
    
    virtual void preRenderUpdate(const Math::Matrix &transformation);
    
    virtual void updatePhysicalData(const Math::Point &origin);
};

}  // namespace Render
}  // namespace Project

#endif
