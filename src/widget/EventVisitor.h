#ifndef PROJECT_WIDGET__EVENT_VISITOR_H
#define PROJECT_WIDGET__EVENT_VISITOR_H

namespace Project {
namespace Widget {

class MouseMoveEvent;
class MouseButtonEvent;
class KeyEvent;
class WidgetActivateEvent;
class FocusEvent;
class WidgetModifiedEvent;
class WidgetSelectedEvent;

class EventVisitor {
public:
    virtual ~EventVisitor() {}
    
    virtual void visit(MouseMoveEvent *event) = 0;
    virtual void visit(MouseButtonEvent *event) = 0;
    virtual void visit(KeyEvent *event) = 0;
    virtual void visit(WidgetActivateEvent *event) = 0;
    virtual void visit(FocusEvent *event) = 0;
    virtual void visit(WidgetModifiedEvent *event) = 0;
    virtual void visit(WidgetSelectedEvent *event) = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
