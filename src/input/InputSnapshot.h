#ifndef PROJECT_INPUT__INPUT_SNAPSHOT_H
#define PROJECT_INPUT__INPUT_SNAPSHOT_H

#include "InputEventTypes.h"

namespace Project {
namespace Input {

/** Supports a copy constructor.
*/
class InputSnapshot {
private:
    bool digitalStatus[NUM_DIGITAL_TYPES];
    bool digitalTriggered[NUM_DIGITAL_TYPES];
    double analogStatus[NUM_ANALOG_TYPES];
public:
    InputSnapshot();
    
    void setDigitalStatus(DigitalInputEvent type, bool status);
    void setDigitalTriggered(DigitalInputEvent type, bool triggered);
    void setAnalogStatus(AnalogInputEvent type, double value);
    
    bool getDigitalStatus(DigitalInputEvent type) const;
    bool getDigitalTriggered(DigitalInputEvent type) const;
    double getAnalogStatus(AnalogInputEvent type) const;
};

}  // namespace Input
}  // namespace Project

#endif
