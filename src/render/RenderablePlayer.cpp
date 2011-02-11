#include "RenderablePlayer.h"

#include "shader/ShaderParamVector4.h"
#include "ColorConstants.h"

#include "math/BoundingBox3D.h"
#include "math/Values.h"
#include "opengl/GeometryDrawing.h"
#include "mesh/MeshLoader.h"
#include "config.h"

#include "log/Logger.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace Render {

void RenderablePlayer::initialize(int id) {
    this->wheelRotationDegrees = 0;

	//Note: All these things need a place to be deleted!

	drawScale = GET_SETTING("render.vehicle.scale", 2.0);
	Math::Matrix scale_matrix = Math::Matrix::getScalingMatrix(drawScale);
	tireScaleMatrix =  Math::Matrix::getScalingMatrix(GET_SETTING("render.tire.scale", 2.5));
   
	Mesh::MeshGroup* chassis_mesh_group = Mesh::MeshLoader::getInstance()->getModelByName(VEHICLE_CHASSIS_MODEL_NAME);
	chassisMesh = new RenderParent(chassis_mesh_group);
	chassisMesh->getRenderProperties()->setTransformation(scale_matrix);

	tireMesh = new RenderParent(Mesh::MeshLoader::getInstance()->getModelByName(VEHICLE_WHEEL_MODEL_NAME));

	materialTint = new OpenGL::Material("matTint");
	materialTint->setDiffuse(OpenGL::Color::WHITE);
	materialTint->setSpecular(OpenGL::Color::WHITE);
	materialTint->setAmbient(OpenGL::Color::WHITE);
	getRenderProperties()->setMaterialTint(materialTint);

	//Set the radius of the bounding sphere for camera culling
	boundingSphere.setRadius(chassis_mesh_group->getRadiusFromOrigin()*drawScale);

}



void RenderablePlayer::preRenderUpdate(const Math::Matrix &transformation) {
    getRenderProperties()->setTransformation(transformation);
}

void RenderablePlayer::updatePhysicalData(const Math::Point &origin) {
    this->origin = origin;
}

void RenderablePlayer::subRender(RenderManager* manager) {

    chassisMesh->render(manager);
    
    for (int wheel = 0; wheel < 4; wheel ++) {
	Math::Matrix matrix = Math::Matrix::getTranslationMatrix(suspension[wheel]);
        
        if (wheel == 1 || wheel == 2) {
		matrix *= Math::Matrix::getRotationMatrix(Math::Y_AXIS, PI);
        }
        
        if (wheel == 1 || wheel == 2){
                matrix *= Math::Matrix::getRotationMatrix(Math::X_AXIS, -wheelRotationDegrees);
        } else {
                matrix *= Math::Matrix::getRotationMatrix(Math::X_AXIS, wheelRotationDegrees);
        }

		matrix *= tireScaleMatrix;

		tireMesh->getRenderProperties()->setTransformation(matrix);
        tireMesh->render(manager);
    }
}

void RenderablePlayer::setWheelRotation(double velocity)
{
    //This is still a bit of a hack. But I need to know what the velocity actually represents
    //In relation to the distance.
    
    this->wheelRotationDegrees += velocity*0.05;
}

bool RenderablePlayer::shouldDraw(const Math::BoundingObject& bounding_obj) {
	boundingSphere.moveCentroid(origin);
	return boundingSphere.intersects(bounding_obj);
}

void RenderablePlayer::setGlowColor(OpenGL::Color color) {
	materialTint->setAmbient(color);
}

}  // namespace Render
}  // namespace Project
