#include <algorithm>  // for std::find

#include "CompositeWidget.h"

#include "CompositeEventProxy.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

CompositeWidget::CompositeWidget(const std::string &name)
    : AbstractWidget(name) {
    
    addEventProxy(new CompositeEventProxy(this));
}

CompositeWidget::~CompositeWidget() {
    // smart pointers automatically free the child widgets if necessary
    childList.clear();
}

void CompositeWidget::addChild(boost::shared_ptr<WidgetBase> widget) {
    childList.push_back(widget);
}

void CompositeWidget::addChild(WidgetBase *widget) {
    addChild(boost::shared_ptr<WidgetBase>(widget));
}

WidgetBase *CompositeWidget::getChild(const std::string &name) {
    for(ChildListType::iterator i = childList.begin(); i != childList.end();
        ++ i) {
        
        WidgetBase *widget = (*i).get();
        if(widget->getName() == name) {
            return widget;
        }
    }
    
    return NULL;
}

void CompositeWidget::removeChild(const std::string &name) {
    for(ChildListType::iterator i = childList.begin(); i != childList.end();
        ++ i) {
        
        WidgetBase *widget = (*i).get();
        if(widget->getName() == name) {
            childList.erase(i);
            break;
        }
    }
}

void CompositeWidget::removeAllChildren() {
    IteratorType it = getIterator();
    while(it.hasNext()) {
        WidgetBase *child = it.next();
        
        removeChild(child->getName());
        it = getIterator();
    }
}

}  // namespace Widget
}  // namespace Project
