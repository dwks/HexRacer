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
    
    /** Accepts a visitor for widgets.
    */
    virtual void accept(WidgetVisitor &visitor) = 0;
    
    /** Forces the layout to re-calculate its position, in case the parent has
        changed dimensions in some way.
    */
    virtual void updateLayout() = 0;
    
    /** Forces the layout to re-calculate the position based on the new
        bounding area @a newBounds.
    */
    virtual void updateLayout(const WidgetRect &newBounds) = 0;
    
    /** Returns the current bounding rectangle for this Widget.
    */
    virtual WidgetRect getBoundingRect() const = 0;
    
    /** Returns the Layout registered in this Widget.
    */
    virtual Layout *getLayout() const = 0;
    
    /** Registers a different Layout for this Widget. This layout will
        automatically be freed when the Widget is destroyed.
    */
    virtual void setLayout(Layout *layout) = 0;
    
    /** Handles the given widget event.
    */
    virtual void handleEvent(WidgetEvent *event) = 0;
    
    /** Adds another event proxy for this widget. When an event arrives, it
        will be sent to all registered proxies in order.
    */
    virtual void addEventProxy(EventProxy *proxy) = 0;
    
    /** Removes all event proxies from this widget.
    */
    virtual void removeAllEventProxies() = 0;
    
    /** Returns the name of this widget; each widget should have a unique name
        amongst its siblings, so that name-based lookups are possible.
    */
    virtual std::string getName() const = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
