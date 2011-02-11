#include "RenderablePlayer.h"

#include "shader/ShaderParamVector4.h"
#include "ColorConstants.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"
#include "mesh/MeshLoader.h"
#include "config.h"

#include "log/Logger.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace Render {

void RenderablePlayer::initialize(int id) {
    this->wheelRotationDegrees = 0;
    
	player_cube_mesh = Mesh::MeshLoader::getInstance()->getModelByName(VEHICLE_CHASSIS_MODEL_NAME);
    player_tire = new RenderList();
	player_tire->addRenderable(Mesh::MeshLoader::getInstance()->getModelByName(VEHICLE_WHEEL_MODEL_NAME));

	//Set the radius of the bounding sphere for camera culling
	boundingSphere.setRadius(
		player_cube_mesh->getRadiusFromOrigin()*GET_SETTING("render.vehicle.scale", 2.0)
		);
    
    getRenderProperties()->addShaderParameter(
		new Shader::ShaderParamVector4("playerColor", Shader::ShaderParameter::UNIFORM,
            ColorConstants::playerColor(id)));

}

void RenderablePlayer::preRenderUpdate(const Math::Matrix &transformation) {
    getRenderProperties()->setTransformation(transformation);
}

void RenderablePlayer::updatePhysicalData(const Math::Point &origin) {
    this->origin = origin;
}

void RenderablePlayer::subRender(RenderManager* manager) {

    player_cube_mesh->render(manager);
    
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

		player_tire->getRenderProperties()->setTransformation(matrix);
        player_tire->render(manager);
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

}  // namespace Render
}  // namespace Project
