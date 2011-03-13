#ifndef PROJECT_WIDGET__BUTTON_WIDGET_LAYOUT_H
#define PROJECT_WIDGET__BUTTON_WIDGET_LAYOUT_H

#include "Layout.h"

namespace Project {
namespace Widget {

class ButtonWidget;

class ButtonWidgetLayout : public Layout {
private:
    ButtonWidget *widget;
    WidgetRect bounds;
public:
    ButtonWidgetLayout(ButtonWidget *widget, const WidgetRect &bounds);
    
    virtual void update();
    virtual void update(const WidgetRect &newBounds);
    
    virtual WidgetRect getBoundingRect() const;
};

}  // namespace Widget
}  // namespace Project

#endif
