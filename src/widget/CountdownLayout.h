#ifndef PROJECT_WIDGET__COUNTDOWN_LAYOUT_H
#define PROJECT_WIDGET__COUNTDOWN_LAYOUT_H

#include "Layout.h"
#include "TextWidget.h"

#include "boost/shared_ptr.hpp"

namespace Project {
namespace Widget {

/** Cloned from CentreUponChangeLayout
*/
class CountdownLayout : public Layout {
private:
    boost::shared_ptr<Layout> normalLayout;
    unsigned long lastChanged;
    int centreTime, transitionTime;
    WidgetRect centreRect;
    static CountdownLayout *lastChangedLayout;
    
    Widget::TextWidget *widget;
    mutable std::string actualData;
    mutable bool ignoreChange;
public:
    CountdownLayout(Widget::TextWidget *widget,
        boost::shared_ptr<Layout> normalLayout,
        int centreTime, int transitionTime, WidgetRect centreRect);
    
    void setCountdown(std::string data);
    void setIgnoreChange() { ignoreChange = true; }
    
    virtual void update();
    virtual void update(const WidgetRect &newBounds);
    
    virtual WidgetRect getBoundingRect() const;
};

}  // namespace Widget
}  // namespace Project

#endif
