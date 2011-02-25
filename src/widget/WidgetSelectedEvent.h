#ifndef PROJECT_WIDGET__WIDGET_SELECTED_EVENT_H
#define PROJECT_WIDGET__WIDGET_SELECTED_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class ListWidget;
class TextWidget;

class WidgetSelectedEvent : public WidgetEvent {
private:
    ListWidget *widget;
    TextWidget *selected;
public:
    WidgetSelectedEvent(ListWidget *widget, TextWidget *selected)
        : widget(widget), selected(selected) {}
    
    ListWidget *getWidget() const { return widget; }
    TextWidget *getSelected() const { return selected; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_SELECTED; }
};

}  // namespace Widget
}  // namespace Project

#endif
