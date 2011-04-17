#include "ColourButtonWidget.h"
#include "WidgetRenderer.h"

#include "sdl/Projector.h"

namespace Project {
namespace Widget {

ColourButtonWidget::ColourButtonWidget(const std::string &name,
    OpenGL::Color colour, const std::string &data, const WidgetRect &bounds)
    : ButtonWidget(name, data, bounds), colour(colour) {
    
}

void ColourButtonWidget::render(WidgetRenderer &renderer) {
    getBox()->accept(renderer);
    
    WidgetPoint corner = getBoundingRect().getCorner();
    WidgetPoint dimensions = getBoundingRect().getDimensions();
    
    colour.glApply();
    
    // code cloned from BoxWidget!
    
    double ratio = dimensions.getY() / dimensions.getX();
    ratio /= SDL::Projector::getInstance()->getAspectRatio();
    
    WidgetPoint cornerSize;
    if(ratio > 1.0) {
        cornerSize = WidgetPoint(
            dimensions.getX() * 0.35,
            dimensions.getY() * 0.35 / ratio);
    }
    else {
        cornerSize = WidgetPoint(
            dimensions.getX() * 0.35 * ratio,
            dimensions.getY() * 0.35);
    }
    
    if(cornerSize.getX() > 0.05) {
        cornerSize *= 0.05 / cornerSize.getX();
    }
    if(cornerSize.getY() > 0.05) {
        cornerSize *= 0.05 / cornerSize.getY();
    }
    
    WidgetPoint ul = corner + cornerSize;
    WidgetPoint ur = ul.plusX(dimensions.getX() - 2*cornerSize.getX());
    WidgetPoint ll = ul.plusY(dimensions.getY() - 2*cornerSize.getY());
    WidgetPoint lr = corner + dimensions - cornerSize;
    
    glBegin(GL_QUADS);
    
    WidgetRenderer::glVertex(ul);
    WidgetRenderer::glVertex(ur);
    WidgetRenderer::glVertex(lr);
    WidgetRenderer::glVertex(ll);
    
    glEnd();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    getText()->accept(renderer);
}

}  // namespace Widget
}  // namespace Project
