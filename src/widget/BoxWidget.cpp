#include "BoxWidget.h"
#include "WidgetRenderer.h"

#include "opengl/OpenGL.h"
#include "render/WidgetTextureList.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void BoxWidget::render() {
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    Render::WidgetTextureList *textureList
        = Render::WidgetTextureList::getInstance();
    unsigned id = textureList->getTexture("corners/out/normal");
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnable(GL_BLEND);  // comment this out to disable transparency
    glBlendFunc(GL_ONE, GL_ONE);
    
    glBegin(GL_QUADS);
    
    glTexCoord2i(0, 0); WidgetRenderer::glVertex(corner);
    glTexCoord2i(1, 0); WidgetRenderer::glVertex(corner.plusXOf(dimensions));
    glTexCoord2i(1, 1); WidgetRenderer::glVertex(corner + dimensions);
    glTexCoord2i(0, 1); WidgetRenderer::glVertex(corner.plusYOf(dimensions));
    
    glEnd();
    
    // back to the blending the rest of the code expects
    // note: must match SDLMain
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisable(GL_TEXTURE_2D);
}

}  // namespace Widget
}  // namespace Project
