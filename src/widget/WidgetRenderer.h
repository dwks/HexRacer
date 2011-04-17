#ifndef PROJECT_WIDGET__WIDGET_RENDERER_H
#define PROJECT_WIDGET__WIDGET_RENDERER_H

#include "WidgetBase.h"
#include "WidgetVisitor.h"

namespace Project {
namespace Widget {

class WidgetRenderer : public WidgetVisitor {
private:
    WidgetPoint screenSize;
    bool fixAspectRatio;
public:
    WidgetRenderer(const WidgetPoint &screenSize, bool fixAspectRatio)
        : screenSize(screenSize), fixAspectRatio(fixAspectRatio) {}
    
    void begin();
    void end();
    
    virtual void visit(BoxWidget *widget);
    virtual void visit(TextWidget *widget);
    virtual void visit(ButtonWidget *widget);
    virtual void visit(CompositeWidget *widget);
    virtual void visit(EditWidget *widget);
    virtual void visit(ScrollbarWidget *widget);
    virtual void visit(ListWidget *widget);
    virtual void visit(ImageWidget *widget);
	virtual void visit(ProgressBarWidget *widget);
    virtual void visit(CheckWidget *widget);
    virtual void visit(ColourButtonWidget *widget);
public:
    static void glVertex(const WidgetPoint &point);
};

}  // namespace Widget
}  // namespace Project

#endif
