#include "CompositeWidget.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void CompositeWidget::addChild(WidgetBase *widget) {
    childList.push_back(widget);
    
    /*WidgetBase *child;
    LOG(GLOBAL, "direct:");
    for(ChildListType::iterator i = childList.begin(); i != childList.end();
        ++ i) {
        
        child = *i;
        
        LOG(GLOBAL, *i << "/" << child);
    }
    
    LOG(GLOBAL, "iterator:");
    for(CompositeWidget::IteratorType i = getIterator(); i.hasNext();
        child = i.next()) {
        
        LOG(GLOBAL, child);
    }*/
}

void CompositeWidget::recursiveAccept(WidgetVisitor &visitor) {
    for(ChildListType::iterator i = childList.begin(); i != childList.end();
        ++ i) {
        
        (*i)->accept(visitor);
    }
}

}  // namespace Widget
}  // namespace Project
