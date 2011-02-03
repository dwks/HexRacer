#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "RenderList.h"
#include "MeshGroup.h"
#include "MeshLoader.h"

namespace Project {
namespace Render {

class RenderablePlayer : public RenderList {
private:
    Math::Point origin;
    
    MeshGroup *player_cube_mesh;
    MeshGroup *player_tire;
    
    double velocity;
    Math::Point suspension[4];
public:
    virtual void initialize(int id);
    
    virtual void preRenderUpdate(const Math::Matrix &transformation);
    void setVelocity(double velocity) { this->velocity = velocity; }
    void setSuspension(int wheel, Math::Point suspension)
        { this->suspension[wheel] = suspension; }
    
    virtual void updatePhysicalData(const Math::Point &origin);
    
    virtual void subRender(RenderManager* manager);
};

}  // namespace Render
}  // namespace Project

#endif
