#include "RenderablePlayer.h"

#include "MeshGroup.h"
#include "MeshLoader.h"
#include "ShaderUniformVector4.h"
#include "ColorConstants.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"

#include "log/Logger.h"

namespace Project {
namespace Render {

void RenderablePlayer::initialize(int id) {
    MeshGroup *player_cube_mesh
        = MeshLoader::getInstance()->getModelByName("playerCube");
    addRenderable(player_cube_mesh);
    
    MeshGroup *player_tire_mesh
        = MeshLoader::getInstance()->getModelByName("playerTire");
    addRenderable(player_tire_mesh);
    
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

/*void RenderablePlayer::renderGeometry(ShaderParamSetter setter) {
    LOG(OPENGL, "WARNING: this code is not used and thus not debugged");
    
    Math::BoundingBox3D box(2.0, 2.0, 2.0, origin);
    OpenGL::GeometryDrawing::drawObject(box, true);
}*/

}  // namespace Render
}  // namespace Project
