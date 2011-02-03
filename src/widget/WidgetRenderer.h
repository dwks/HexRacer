#ifndef PROJECT_WIDGET__WIDGET_RENDERER_H
#define PROJECT_WIDGET__WIDGET_RENDERER_H

#include "WidgetBase.h"
#include "WidgetVisitor.h"

namespace Project {
namespace Widget {

class WidgetRenderer : public WidgetVisitor {
public:
    void begin();
    void end();
    
    virtual void visit(BoxWidget *widget);
private:
    void glVertex(const WidgetPoint &point);
};

}  // namespace Widget
}  // namespace Project

#endif
