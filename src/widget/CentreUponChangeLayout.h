#ifndef PROJECT_WIDGET__CENTRE_UPON_CHANGE_LAYOUT_H
#define PROJECT_WIDGET__CENTRE_UPON_CHANGE_LAYOUT_H

#include "Layout.h"

#include "boost/shared_ptr.hpp"

namespace Project {
namespace Widget {

class CentreUponChangeLayout : public Layout {
private:
    boost::shared_ptr<Layout> normalLayout;
    unsigned long lastChanged;
    int centreTime, transitionTime;
    WidgetRect centreRect;
    static CentreUponChangeLayout *lastChangedLayout;
public:
    CentreUponChangeLayout(boost::shared_ptr<Layout> normalLayout,
        int centreTime, int transitionTime, WidgetRect centreRect);
    
    virtual void update();
    virtual void update(const WidgetRect &newBounds);
    
    virtual WidgetRect getBoundingRect() const;
};

}  // namespace Widget
}  // namespace Project

#endif
