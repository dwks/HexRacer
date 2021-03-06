#ifndef PROJECT_RENDER__RENDERABLE_PLAYER_H
#define PROJECT_RENDER__RENDERABLE_PLAYER_H

#include "RenderableObject.h"
#include "mesh/MeshGroup.h"
#include "RenderParent.h"
#include "RenderList.h"
#include "opengl/Color.h"

#include "boost/smart_ptr/shared_ptr.hpp"

#include "math/Values.h"
#include "math/BoundingSphere.h"

namespace Project {
namespace Render {

class RenderablePlayer : public RenderableObject {

private:
    Math::Point origin;
    
	boost::shared_ptr<RenderParent> chassisMesh;
        boost::shared_ptr<RenderParent> nonChassisMesh;
    boost::shared_ptr<RenderParent> tireMesh;
	boost::shared_ptr<OpenGL::Material> materialTint;
        boost::shared_ptr<OpenGL::Material> chassisTint;
    
    double wheelRotationDegrees; 
    Math::Point suspension[4];
	Math::BoundingSphere boundingSphere;

	double drawScale;
	//Math::Matrix tireScaleMatrix;

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
