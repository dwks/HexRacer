#ifndef PROJECT_WIDGET__FOCUS_MANAGER_H
#define PROJECT_WIDGET__FOCUS_MANAGER_H

#include "WidgetBase.h"

namespace Project {
namespace Widget {

class FocusManager {
private:
    static FocusManager *instance;
public:
    static FocusManager *getInstance() { return instance; }
private:
    WidgetBase *motionFocus;
    WidgetBase *clickFocus;
    WidgetBase *keyFocus;
public:
    FocusManager();
    
    WidgetBase *getMotionFocus() { return motionFocus; }
    WidgetBase *getClickFocus() { return clickFocus; }
    WidgetBase *getKeyFocus() { return keyFocus; }
    
    void setMotionFocus(WidgetBase *focus);
    void setClickFocus(WidgetBase *focus);
    void setKeyFocus(WidgetBase *focus);
};

}  // namespace Widget
}  // namespace Project

#endif
