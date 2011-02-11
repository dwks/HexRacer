#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "RenderableObject.h"
#include "mesh/MeshGroup.h"
#include "RenderParent.h"
#include "opengl/Color.h"

#include "math/Values.h"
#include "math/BoundingSphere.h"

namespace Project {
namespace Render {

class RenderablePlayer : public RenderableObject {

private:
    Math::Point origin;
    
	RenderParent *chassisMesh;
	RenderParent *glowMesh;
    RenderParent *tireMesh;
    
    double wheelRotationDegrees; 
    Math::Point suspension[4];
	Math::BoundingSphere boundingSphere;

	double drawScale;
	Math::Matrix tireScaleMatrix;

public:
    virtual void initialize(int id);
    
    virtual void preRenderUpdate(const Math::Matrix &transformation);
    void setWheelRotation(double velocity);
    void setSuspension(int wheel, Math::Point suspension)
        { this->suspension[wheel] = suspension; }
    
    virtual void updatePhysicalData(const Math::Point &origin);
    virtual void subRender(RenderManager* manager);
	virtual bool shouldDraw(const Math::BoundingObject& bounding_obj);
	void setGlowColor(OpenGL::Color color);
};

}  // namespace Render
}  // namespace Project

#endif
