#ifndef PROJECT_WIDGET__WIDGET_BASE_H
#define PROJECT_WIDGET__WIDGET_BASE_H

#include <string>

#include "WidgetVisitor.h"
#include "WidgetRect.h"

namespace Project {
namespace Widget {

class Layout;
class EventProxy;
class WidgetEvent;

/** Abstract base class for all widgets.
*/
class WidgetBase {
public:
    virtual ~WidgetBase() {}
    
    virtual void accept(WidgetVisitor &visitor) = 0;
    
    virtual void updateLayout() = 0;
    virtual void updateLayout(const WidgetRect &newBounds) = 0;
    virtual WidgetRect getBoundingRect() const = 0;
    
    virtual Layout *getLayout() const = 0;
    virtual void setLayout(Layout *layout) = 0;
    
    virtual void handleEvent(WidgetEvent *event) = 0;
    
    virtual EventProxy *getEventProxy() const = 0;
    virtual void setEventProxy(EventProxy *proxy) = 0;
    
    virtual std::string getName() const = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
