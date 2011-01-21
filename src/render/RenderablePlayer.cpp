#include "RenderablePlayer.h"

#include "math/BoundingBox3D.h"
#include "opengl/GeometryDrawing.h"

#include "log/Logger.h"

namespace Project {
namespace Render {

void RenderablePlayer::updatePhysicalData(const Math::Point &origin) {
    this->origin = origin;
}

void RenderablePlayer::renderGeometry(ShaderParamSetter setter) {
    LOG(OPENGL, "WARNING: this code is not used and thus not debugged");
    
    Math::BoundingBox3D box(2.0, 2.0, 2.0, origin);
    OpenGL::GeometryDrawing::drawObject(box, true);
}

}  // namespace Render
}  // namespace Project
