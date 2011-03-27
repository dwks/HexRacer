#ifndef PROJECT_EVENT__SET_CHECKING_NETWORK_H
#define PROJECT_EVENT__SET_CHECKING_NETWORK_H

#include "EventBase.h"

namespace Project {
namespace Event {

class SetCheckingNetwork : public EventBase {
private:
    bool checking;
public:
    SetCheckingNetwork(bool checking = false) : checking(checking) {}
    
    bool getChecking() const { return checking; }
    
    virtual EventType::type_t getType() const
        { return EventType::SET_CHECKING_NETWORK; }
};

}  // namespace Event
}  // namespace Project

#endif
