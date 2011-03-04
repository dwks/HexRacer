#include "WidgetRenderer.h"

#include "BoxWidget.h"
#include "TextWidget.h"
#include "ButtonWidget.h"
#include "CompositeWidget.h"
#include "EditWidget.h"
#include "ScrollbarWidget.h"
#include "ListWidget.h"
#include "ImageWidget.h"
#include "ProgressBarWidget.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void WidgetRenderer::begin() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();  // save original projection matrix
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 1.0, 0.0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();  // save original modelview matrix
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST); 
}

void WidgetRenderer::end() {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();  // restore original modelview matrix
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // restore original projection matrix
    
    // switch back to the modelview matrix, rest of the code expects this
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
}

void WidgetRenderer::visit(BoxWidget *widget) {
    widget->render();
}

void WidgetRenderer::visit(TextWidget *widget) {
    //LOG(WIDGET, "text at " << widget->getBoundingRect());
    
    widget->render();
}

void WidgetRenderer::visit(ButtonWidget *widget) {
    widget->getBox()->accept(*this);
    widget->getText()->accept(*this);
}

void WidgetRenderer::visit(CompositeWidget *widget) {
    CompositeWidget::IteratorType i = widget->getIterator();
    while(i.hasNext()) {
        WidgetBase *child = i.next();
        
        child->accept(*this);
    }
}

void WidgetRenderer::visit(EditWidget *widget) {
    widget->getBox()->accept(*this);
    widget->getText()->accept(*this);
}

void WidgetRenderer::visit(ScrollbarWidget *widget) {
    widget->render(*this);
}

void WidgetRenderer::visit(ListWidget *widget) {
    widget->render(*this);
}

void WidgetRenderer::visit(ImageWidget *widget) {
    widget->render();
}

void WidgetRenderer::visit(ProgressBarWidget *widget) {
    widget->render();
}

void WidgetRenderer::glVertex(const WidgetPoint &point) {
    glVertex2d(point.getX(), point.getY());
}

}  // namespace Widget
}  // namespace Project
