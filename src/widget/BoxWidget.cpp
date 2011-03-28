#include "BoxWidget.h"
#include "WidgetRenderer.h"

#include "opengl/OpenGL.h"
#include "render/WidgetTextureList.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void BoxWidget::setArtwork(const std::string &artwork) {
    this->artwork = artwork;
}

void BoxWidget::render() {
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    Render::WidgetTextureList *textureList
        = Render::WidgetTextureList::getInstance();
    unsigned id = textureList->getTexture(artwork);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnable(GL_BLEND);  // comment this out to disable transparency
    
    glBegin(GL_QUADS);
    
#if 0
    glTexCoord2i(0, 0); WidgetRenderer::glVertex(corner);
    glTexCoord2i(1, 0); WidgetRenderer::glVertex(corner.plusXOf(dimensions));
    glTexCoord2i(1, 1); WidgetRenderer::glVertex(corner + dimensions);
    glTexCoord2i(0, 1); WidgetRenderer::glVertex(corner.plusYOf(dimensions));
#else
    // first draw the corners
    
    double ratio = dimensions.getY() / dimensions.getX();
    
    WidgetPoint cornerSize;
    if(ratio > 1.0) {
        cornerSize = WidgetPoint(
            dimensions.getX() * 0.3,
            dimensions.getY() * 0.3 / ratio);
    }
    else {
        cornerSize = WidgetPoint(
            dimensions.getX() * 0.3 * ratio,
            dimensions.getY() * 0.3);
    }
    
    if(cornerSize.getX() > 0.05) {
        cornerSize *= 0.05 / cornerSize.getX();
    }
    if(cornerSize.getY() > 0.05) {
        cornerSize *= 0.05 / cornerSize.getY();
    }
    
    WidgetPoint ur = corner.plusX(dimensions.getX() - cornerSize.getX());
    WidgetPoint ll = corner.plusY(dimensions.getY() - cornerSize.getY());
    WidgetPoint lr = corner + dimensions - cornerSize;
    
    drawQuad(0.0, 0.5, 0.0, 0.5, corner, cornerSize);
    drawQuad(0.5, 1.0, 0.0, 0.5, ur, cornerSize);
    drawQuad(0.5, 1.0, 0.5, 1.0, lr, cornerSize);
    drawQuad(0.0, 0.5, 0.5, 1.0, ll, cornerSize);
    
    // next draw the edges
    
    double width = dimensions.getX() - 2 * cornerSize.getX();
    double height = dimensions.getY() - 2 * cornerSize.getY();
    
    // horizontal
    drawQuad(0.5, 0.5, 0.0, 0.5, corner.plusXOf(cornerSize),
        WidgetPoint(width, cornerSize.getY()));
    drawQuad(0.5, 0.5, 0.5, 1.0, (corner + cornerSize).plusY(height),
        WidgetPoint(width, cornerSize.getY()));
    
    // vertical
    drawQuad(0.0, 0.5, 0.5, 0.5, corner.plusYOf(cornerSize),
        WidgetPoint(cornerSize.getX(), height));
    drawQuad(0.5, 1.0, 0.5, 0.5, (corner + cornerSize).plusX(width),
        WidgetPoint(cornerSize.getX(), height));
    
    // finally, draw the centre
    
    drawQuad(0.5, 0.5, 0.5, 0.5, corner + cornerSize,
        WidgetPoint(width, height));
#endif
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void BoxWidget::drawQuad(double xmin, double xmax, double ymin, double ymax,
    WidgetPoint corner, WidgetPoint dimensions) {
    
    glTexCoord2d(xmin, ymin);
    WidgetRenderer::glVertex(corner);
    glTexCoord2d(xmax, ymin);
    WidgetRenderer::glVertex(corner.plusXOf(dimensions));
    glTexCoord2d(xmax, ymax);
    WidgetRenderer::glVertex(corner + dimensions);
    glTexCoord2d(xmin, ymax);
    WidgetRenderer::glVertex(corner.plusYOf(dimensions));
}

}  // namespace Widget
}  // namespace Project
