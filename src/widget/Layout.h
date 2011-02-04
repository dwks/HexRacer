#ifndef PROJECT_WIDGET__LAYOUT_H
#define PROJECT_WIDGET__LAYOUT_H

#include "WidgetRect.h"

namespace Project {
namespace Widget {

/** Figures out where and how to place a widget on the screen.
*/
class Layout {
public:
    virtual ~Layout() {}
    
    virtual void update() = 0;
    virtual void update(const WidgetRect &newBounds) = 0;
    
    virtual WidgetRect getBoundingRect() const = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
