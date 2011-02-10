#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "RenderableObject.h"
#include "mesh/MeshGroup.h"
#include "RenderList.h"

#include "math/Values.h"
#include "math/BoundingSphere.h"

namespace Project {
namespace Render {

class RenderablePlayer : public RenderableObject {

private:
    Math::Point origin;
    
	Mesh::MeshGroup *player_cube_mesh;
    RenderList *player_tire;
    
    double wheelRotationDegrees; 
    Math::Point suspension[4];
	Math::BoundingSphere boundingSphere;

public:
    virtual void initialize(int id);
    
    virtual void preRenderUpdate(const Math::Matrix &transformation);
    void setWheelRotation(double velocity);
    void setSuspension(int wheel, Math::Point suspension)
        { this->suspension[wheel] = suspension; }
    
    virtual void updatePhysicalData(const Math::Point &origin);
    
    virtual void subRender(RenderManager* manager);

	virtual bool shouldDraw(const Math::BoundingObject& bounding_obj);
};

}  // namespace Render
}  // namespace Project

#endif
