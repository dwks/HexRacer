#ifndef PROJECT_WIDGET__COMPOSITE_WIDGET_H
#define PROJECT_WIDGET__COMPOSITE_WIDGET_H

#include <vector>
#include "boost/shared_ptr.hpp"

#include "AbstractWidget.h"

namespace Project {
namespace Widget {

class CompositeWidget : public AbstractWidget {
private:
    typedef std::vector<boost::shared_ptr<WidgetBase> > ChildListType;
    ChildListType childList;
public:
    class IteratorType {
    private:
        ChildListType::iterator it, end;
    public:
        IteratorType(ChildListType &list)
            : it(list.begin()), end(list.end()) {}
        
        WidgetBase *next() { return (*it ++).get(); }
        bool hasNext() const { return it != end; }
    };
public:
    CompositeWidget(const std::string &name);
    ~CompositeWidget();
    
    /** Adds a child widget to this composite widget.
        
        This is the primary version of the function, which should be overloaded
        by subclasses if necessary.
    */
    virtual void addChild(boost::shared_ptr<WidgetBase> widget);
    virtual void addChild(WidgetBase *widget);
    virtual WidgetBase *getChild(const std::string &name);
    
    virtual void removeChild(const std::string &name);
    
    IteratorType getIterator() { return IteratorType(childList); }
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
