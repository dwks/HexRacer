#ifndef PROJECT_TIMING__TIMED_SUBSYSTEM_H
#define PROJECT_TIMING__TIMED_SUBSYSTEM_H

namespace Project {
namespace Timing {

/** All times for this class are in milliseconds.
*/
class TimedSubsystem {
private:
    unsigned long tickTime, lastTime;
public:
    /** Creates a timed subsystem which activates every @a tickTime
        milliseconds.
    */
    TimedSubsystem(unsigned long tickTime);
    
    /** Used to set the time of the last frame explicitly. This is used when
        the program is first initialized and when resuming from a pause state.
    */
    virtual void setLastTime(unsigned long timeNow);
    
    /** Called whenever the main loop can give this subsystem a timeslice.
    */
    virtual void doStep(unsigned long currentTime);
    
    /** This method should be implemented by concrete subclasses.
    */
    virtual void doAction(unsigned long currentTime) = 0;
};

}  // namespace Timing
}  // namespace Project

#endif
