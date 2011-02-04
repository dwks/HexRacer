#ifndef PROJECT_WIDGET__EVENT_VISITOR_H
#define PROJECT_WIDGET__EVENT_VISITOR_H

namespace Project {
namespace Widget {

class WidgetFocusEvent;
class WidgetActivateEvent;

class EventVisitor {
public:
    virtual ~EventVisitor() {}
    
    virtual void visit(WidgetFocusEvent *event) = 0;
    virtual void visit(WidgetActivateEvent *event) = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
