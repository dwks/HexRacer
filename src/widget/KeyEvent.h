#ifndef PROJECT_WIDGET__KEY_EVENT_H
#define PROJECT_WIDGET__KEY_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class KeyEvent : public WidgetEvent {
private:
    long key;
    long unicode;
    bool down;
public:
    KeyEvent(long key, long unicode, bool down)
        : key(key), unicode(unicode), down(down) {}
    
    long getKey() const { return key; }
    long getUnicode() const { return unicode; }
    bool getDown() const { return down; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_KEY; }
};

}  // namespace Widget
}  // namespace Project

#endif
