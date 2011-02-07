#ifndef PROJECT_WIDGET__KEYBOARD_SHORTCUT_PROXY_H
#define PROJECT_WIDGET__KEYBOARD_SHORTCUT_PROXY_H

#include "WidgetBase.h"
#include "EventProxy.h"

namespace Project {
namespace Widget {

class KeyboardShortcutProxy : public EventProxy {
private:
    WidgetBase *widget;
    long key;
public:
    KeyboardShortcutProxy(WidgetBase *widget, long key)
        : widget(widget), key(key) {}
    
    virtual void visit(Widget::KeyEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
