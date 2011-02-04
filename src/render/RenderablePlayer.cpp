#include "RenderablePlayer.h"

#include "ShaderUniformVector4.h"
#include "ColorConstants.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"
#include "config.h"

#include "log/Logger.h"

namespace Project {
namespace Render {

void RenderablePlayer::initialize(int id) {
    this->wheelRotationDegrees = 0;
    
    player_cube_mesh = MeshLoader::getInstance()->getModelByName(VEHICLE_CHASSIS_MODEL_NAME);
    player_tire = new RenderList();
	player_tire->addRenderable(MeshLoader::getInstance()->getModelByName(VEHICLE_WHEEL_MODEL_NAME));
    
    getRenderProperties()->addShaderParameter(
        new Render::ShaderUniformVector4("playerColor",
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
                matrix *= Math::Matrix::getRotationMatrix(Math::X_AXIS, wheelRotationDegrees);
        } else {
                matrix *= Math::Matrix::getRotationMatrix(Math::X_AXIS, -wheelRotationDegrees);
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

/*void RenderablePlayer::renderGeometry(ShaderParamSetter setter) {
    LOG(OPENGL, "WARNING: this code is not used and thus not debugged");
    
    Math::BoundingBox3D box(2.0, 2.0, 2.0, origin);
    OpenGL::GeometryDrawing::drawObject(box, true);
}*/

}  // namespace Render
}  // namespace Project
