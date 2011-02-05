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

void CompositeWidget::addChild(WidgetBase *widget) {
    childList.push_back(widget);
}

WidgetBase *CompositeWidget::getChild(const std::string &name) {
    for(ChildListType::iterator i = childList.begin(); i != childList.end();
        ++ i) {
        
        WidgetBase *widget = *i;
        if(widget->getName() == name) {
            return widget;
        }
    }
    
    return NULL;
}

}  // namespace Widget
}  // namespace Project
