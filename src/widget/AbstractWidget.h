#ifndef PROJECT_WIDGET__ABSTRACT_WIDGET_H
#define PROJECT_WIDGET__ABSTRACT_WIDGET_H

#include "WidgetBase.h"

namespace Project {
namespace Widget {

class AbstractWidget : public WidgetBase {
private:
    std::string name;
    Layout *layout;
    EventProxy *proxy;
public:
    AbstractWidget(const std::string &name)
        : name(name), layout(0), proxy(0) {}
    virtual ~AbstractWidget();
    
    virtual void updateLayout();
    virtual void updateLayout(const WidgetRect &newBounds);
    virtual WidgetRect getBoundingRect() const;
    
    virtual Layout *getLayout() const { return layout; }
    virtual void setLayout(Layout *layout) { this->layout = layout; }
    
    virtual void handleEvent(WidgetEvent *event);
    
    virtual EventProxy *getEventProxy() const { return proxy; }
    virtual void setEventProxy(EventProxy *proxy) { this->proxy = proxy; }
    
    virtual std::string getName() const { return name; }
protected:
    void setName(const std::string &newName) { name = newName; }
};

}  // namespace Widget
}  // namespace Project

#endif
