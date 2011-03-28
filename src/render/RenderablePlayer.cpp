#include "RenderablePlayer.h"

#include "shader/ShaderParamVector4.h"

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

	drawScale = GET_SETTING("render.vehicle.scale", 2.0);
	Math::Matrix scale_matrix = Math::Matrix::getScalingMatrix(drawScale);
	//tireScaleMatrix =  Math::Matrix::getScalingMatrix(GET_SETTING("render.tire.scale", 2.5));
   
	Mesh::MeshGroup* chassis_mesh_group = Mesh::MeshLoader::getInstance()->getModelByName(VEHICLE_CHASSIS_MODEL_NAME);
	chassisMesh = boost::shared_ptr<RenderParent>(
		new RenderParent(chassis_mesh_group)
		);
	chassisMesh->getRenderProperties()->setTransformation(scale_matrix);

	tireMesh = boost::shared_ptr<RenderParent>(
		new RenderParent(Mesh::MeshLoader::getInstance()->getModelByName(VEHICLE_WHEEL_MODEL_NAME))
		);

	materialTint = boost::shared_ptr<OpenGL::Material>(
		new OpenGL::Material("matTint")
		);
	materialTint->setDiffuse(OpenGL::Color::WHITE);
	materialTint->setSpecular(OpenGL::Color::WHITE);
	materialTint->setAmbient(OpenGL::Color::WHITE);
	getRenderProperties()->setMaterialTint(materialTint.get());

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

		double tire_scale = GET_SETTING("render.tire.scale", 2.5);
		matrix.set(0, 0, tire_scale);
		matrix.set(1, 1, tire_scale);
		matrix.set(2, 2, tire_scale);
        
        if (wheel == 1 || wheel == 2) {
			//matrix *= Math::Matrix::getRotationMatrix(Math::Y_AXIS, PI);
			matrix.set(0, 0, -matrix.get(0, 0));
			matrix.set(2, 2, -matrix.get(2, 2));
        }
        
        if (wheel == 1 || wheel == 2){
			matrix *= Math::Matrix::getRotationMatrix(Math::X_AXIS, -wheelRotationDegrees);
        } else {
			matrix *= Math::Matrix::getRotationMatrix(Math::X_AXIS, wheelRotationDegrees);
        }

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
