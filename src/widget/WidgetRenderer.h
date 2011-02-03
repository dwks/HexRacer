#ifndef PROJECT_WIDGET__WIDGET_RENDERER_H
#define PROJECT_WIDGET__WIDGET_RENDERER_H

#include "WidgetBase.h"
#include "WidgetVisitor.h"

namespace Project {
namespace Widget {

class WidgetRenderer : public WidgetVisitor {
private:
    WidgetPoint screenSize;
public:
    WidgetRenderer(const WidgetPoint &screenSize) : screenSize(screenSize) {}
    
    void begin();
    void end();
    
    virtual void visit(BoxWidget *widget);
    virtual void visit(TextWidget *widget);
    virtual void visit(ButtonWidget *widget);
private:
    void glVertex(const WidgetPoint &point);
};

}  // namespace Widget
}  // namespace Project

#endif
