#ifndef PROJECT_WIDGET__SCROLLBAR_SLIDER_EVENT_PROXY_H
#define PROJECT_WIDGET__SCROLLBAR_SLIDER_EVENT_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetPoint.h"

namespace Project {
namespace Widget {

class ScrollbarWidget;

class ScrollbarSliderEventProxy : public Widget::EventProxy {
private:
    ScrollbarWidget *scrollbar;
    bool sliding;
    Widget::WidgetPoint lastSlidePosition;
public:
    ScrollbarSliderEventProxy(ScrollbarWidget *scrollbar)
        : scrollbar(scrollbar), sliding(false), lastSlidePosition() {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(FocusEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
