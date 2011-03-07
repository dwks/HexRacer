#ifndef PROJECT_WIDGET__REPAINT_EVENT_H
#define PROJECT_WIDGET__REPAINT_EVENT_H

#include "WidgetEvent.h"
#include "WidgetBase.h"

namespace Project {
namespace Widget {

/** Warning: this event is currently only fired for top-level CompositeWidgets!
*/
class RepaintEvent : public WidgetEvent {
private:
    Widget::WidgetBase *widget;
public:
    RepaintEvent(Widget::WidgetBase *widget) : widget(widget) {}
    
    Widget::WidgetBase *getWidget() const { return widget; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_REPAINT; }
};

}  // namespace Widget
}  // namespace Project

#endif
