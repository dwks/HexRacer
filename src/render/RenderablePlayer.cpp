#include "RenderablePlayer.h"

#include "ShaderUniformVector4.h"
#include "ColorConstants.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"

#include "log/Logger.h"

namespace Project {
namespace Render {

void RenderablePlayer::initialize(int id) {
    player_cube_mesh = MeshLoader::getInstance()->getModelByName("playerCube");
    addRenderable(player_cube_mesh);
    
    player_tire = MeshLoader::getInstance()->getModelByName("playerTire");
    addRenderable(player_tire);
    
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
    
    for(int wheel = 0; wheel < 4; wheel ++) {
        glPushMatrix();
        Math::Point s = suspension[wheel];
        glTranslated(s.getX(), s.getY(), s.getZ());
        
        if(wheel == 1 || wheel == 2) {
            glRotated(180.0,0.0,1.0,0.0);
        }
        
        if(wheel == 1 || wheel == 2){
            glRotated(360.0*velocity,1.0,0.0,0.0);
        } else {
            glRotated(-360.0*velocity,1.0,0.0,0.0);
        }
        
        player_tire->render(manager);
        
        glPopMatrix();
    }
}

/*void RenderablePlayer::renderGeometry(ShaderParamSetter setter) {
    LOG(OPENGL, "WARNING: this code is not used and thus not debugged");
    
    Math::BoundingBox3D box(2.0, 2.0, 2.0, origin);
    OpenGL::GeometryDrawing::drawObject(box, true);
}*/

}  // namespace Render
}  // namespace Project
