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

void CompositeWidget::recursiveAccept(WidgetVisitor &visitor) {
    for(ChildListType::iterator i = childList.begin(); i != childList.end();
        ++ i) {
        
        (*i)->accept(visitor);
    }
}

}  // namespace Widget
}  // namespace Project
