#ifndef PROJECT_WIDGET__COMPOSITE_WIDGET_H
#define PROJECT_WIDGET__COMPOSITE_WIDGET_H

#include <vector>

#include "AbstractWidget.h"

namespace Project {
namespace Widget {

class CompositeWidget : public AbstractWidget {
private:
    typedef std::vector<WidgetBase *> ChildListType;
    ChildListType childList;
public:
    class IteratorType {
    private:
        ChildListType::iterator it, end;
    public:
        IteratorType(ChildListType &list)
            : it(list.begin()), end(list.end()) {}
        
        WidgetBase *next() { return *it ++; }
        bool hasNext() const { return it != end; }
    };
public:
    CompositeWidget(const std::string &name);
    
    void addChild(WidgetBase *widget);
    virtual WidgetBase *getChild(const std::string &name);
    
    IteratorType getIterator() { return IteratorType(childList); }
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
    
    void recursiveAccept(WidgetVisitor &visitor);
};

}  // namespace Widget
}  // namespace Project

#endif
