#ifndef PROJECT_EVENT__SWITCH_TO_SCREEN_H
#define PROJECT_EVENT__SWITCH_TO_SCREEN_H

#include <string>
#include "EventBase.h"

namespace Project {
namespace Event {

/** Switches to displaying a different GUI screen.
*/
class SwitchToScreen : public EventBase {
private:
    std::string screen;
public:
    SwitchToScreen() {}
    SwitchToScreen(const std::string &screen) : screen(screen) {}
    
    const std::string &getScreen() const { return screen; }
    
    virtual EventType::type_t getType() const
        { return EventType::SWITCH_TO_SCREEN; }
};

}  // namespace Event
}  // namespace Project

#endif
