#include "InputSnapshot.h"

#include "math/Values.h"

namespace Project {
namespace Input {

InputSnapshot::InputSnapshot() {
    for(int x = 0; x < NUM_DIGITAL_TYPES; x ++) {
        digitalStatus[x] = false;
        digitalTriggered[x] = false;
    }
    
    for(int x = 0; x < NUM_ANALOG_TYPES; x ++) {
        analogStatus[x] = 0.0;
    }
}

void InputSnapshot::setDigitalStatus(DigitalInputEvent type, bool status) {
    digitalStatus[type] = status;
}

void InputSnapshot::setDigitalTriggered(DigitalInputEvent type,
    bool triggered) {
    
    digitalTriggered[static_cast<int>(type)] = triggered;
}

void InputSnapshot::setAnalogStatus(AnalogInputEvent type, double value) {
    analogStatus[static_cast<int>(type)] = value;
}
    
bool InputSnapshot::getDigitalStatus(DigitalInputEvent type) const {
    return digitalStatus[static_cast<int>(type)];
}

bool InputSnapshot::getDigitalTriggered(DigitalInputEvent type) const {
    return digitalTriggered[static_cast<int>(type)];
}

double InputSnapshot::getAnalogStatus(AnalogInputEvent type) const {
    return analogStatus[static_cast<int>(type)];
}

void InputSnapshot::asPlayerIntention(World::PlayerIntention &intention) const {
    intention.setAccel(Math::bound(getAnalogStatus(INPUT_A_ACCELERATE), -1.0, 1.0));
    intention.setTurn(Math::bound(getAnalogStatus(INPUT_A_TURN), -1.0, 1.0));
    intention.setJump(getDigitalStatus(INPUT_D_JUMP));
    
    intention.setReset(getDigitalTriggered(INPUT_D_RESET));
    intention.setPaint(getDigitalStatus(INPUT_D_PAINT));
    intention.setErase(getDigitalStatus(INPUT_D_ERASE));
}

}  // namespace Input
}  // namespace Project
