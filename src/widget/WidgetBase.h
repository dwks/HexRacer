#ifndef PROJECT_WIDGET__WIDGET_BASE_H
#define PROJECT_WIDGET__WIDGET_BASE_H

#include <string>

#include "WidgetVisitor.h"
#include "WidgetRect.h"

namespace Project {
namespace Widget {

/** Abstract base class for all widgets.
*/
class WidgetBase {
public:
    virtual ~WidgetBase() {}
    
    virtual void accept(WidgetVisitor &visitor) = 0;
    
    virtual WidgetRect getBoundingRect() const = 0;
    virtual void setBoundingRect(const WidgetRect &rect) = 0;
    
    virtual std::string getName() const = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
