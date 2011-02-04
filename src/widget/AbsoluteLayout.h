#ifndef PROJECT_WIDGET__ABSOLUTE_LAYOUT_H
#define PROJECT_WIDGET__ABSOLUTE_LAYOUT_H

#include "Layout.h"

namespace Project {
namespace Widget {

class AbsoluteLayout : public Layout {
private:
    WidgetRect rect;
public:
    AbsoluteLayout(const WidgetRect &rect) : rect(rect) {}
    
    virtual void update() {}
    virtual void update(const WidgetRect &newBounds)
        { this->rect = newBounds; }
    
    virtual WidgetRect getBoundingRect() const { return rect; }
};

}  // namespace Widget
}  // namespace Project

#endif
