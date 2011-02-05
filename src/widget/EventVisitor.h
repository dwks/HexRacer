#ifndef PROJECT_WIDGET__EVENT_VISITOR_H
#define PROJECT_WIDGET__EVENT_VISITOR_H

namespace Project {
namespace Widget {

class MouseMoveEvent;
class WidgetActivateEvent;

class EventVisitor {
public:
    virtual ~EventVisitor() {}
    
    virtual void visit(MouseMoveEvent *event) = 0;
    virtual void visit(WidgetActivateEvent *event) = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
