#ifndef PROJECT_HISTORY__PING_TIME_MEASURER_H
#define PROJECT_HISTORY__PING_TIME_MEASURER_H

#include <climits>

namespace Project {
namespace History {

class PingTimeMeasurer {
private:
    static PingTimeMeasurer *instance;
public:
    static PingTimeMeasurer *getInstance() { return instance; }
public:
    static const int NO_CLOCK_OFFSET = INT_MIN;
private:
    int clockOffset;
public:
    PingTimeMeasurer() : clockOffset(NO_CLOCK_OFFSET) { instance = this; }
    ~PingTimeMeasurer() { instance = 0; }
    
    void setClockOffset(int offset) { clockOffset = offset; }
    int getClockOffset() const { return clockOffset; }
};

}  // namespace History
}  // namespace Project

#endif
