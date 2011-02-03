#ifndef PROJECT_WIDGET__WIDGET_VISITOR_H
#define PROJECT_WIDGET__WIDGET_VISITOR_H

namespace Project {
namespace Widget {

class BoxWidget;

class WidgetVisitor {
public:
    virtual ~WidgetVisitor() {}
    
    virtual void visit(BoxWidget *widget) = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
