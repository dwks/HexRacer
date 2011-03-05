#ifndef PROJECT_WIDGET__ABSTRACT_WIDGET_H
#define PROJECT_WIDGET__ABSTRACT_WIDGET_H

#include <vector>

#include "WidgetBase.h"

namespace Project {
namespace Widget {

class AbstractWidget : public WidgetBase {
protected:
    typedef std::vector<boost::shared_ptr<EventProxy> > proxy_list_t;
private:
    std::string name;
    boost::shared_ptr<Layout> layout;
    proxy_list_t proxyList;
public:
    AbstractWidget(const std::string &name) : name(name) {}
    virtual ~AbstractWidget();
    
    virtual void updateLayout();
    virtual void updateLayout(const WidgetRect &newBounds);
    virtual WidgetRect getBoundingRect() const;
    
    virtual Layout *getLayout() const;
    virtual void setLayout(Layout *layout);
    
    virtual void handleEvent(WidgetEvent *event);
    
    virtual void addEventProxy(boost::shared_ptr<EventProxy> proxy);
    virtual void addEventProxy(EventProxy *proxy);
    virtual void removeAllEventProxies();
    
    virtual WidgetBase *getChild(const std::string &name) { return NULL; }
    
    virtual std::string getName() const { return name; }
protected:
    void setName(const std::string &newName) { name = newName; }
};

}  // namespace Widget
}  // namespace Project

#endif
