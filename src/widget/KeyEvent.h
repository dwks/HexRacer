#ifndef PROJECT_WIDGET__KEY_EVENT_H
#define PROJECT_WIDGET__KEY_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class KeyEvent : public WidgetEvent {
private:
    long key;
    bool down;
public:
    KeyEvent(long key, bool down) : key(key), down(down) {}
    
    long getKey() const { return key; }
    bool getDown() const { return down; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_KEY; }
};

}  // namespace Widget
}  // namespace Project

#endif
