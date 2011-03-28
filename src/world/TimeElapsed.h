#ifndef PROJECT_WORLD__TIME_ELAPSED_H
#define PROJECT_WORLD__TIME_ELAPSED_H

namespace Project {
namespace World {

class TimeElapsed {
private:
    static TimeElapsed instance;
public:
    static TimeElapsed &getInstance() { return instance; }
private:
    unsigned long startTime;
public:
    TimeElapsed() : startTime(0) {}
    
    void setStartTime(unsigned long now) { startTime = now; }
    void addStartTime(unsigned long add) { startTime += add; }
    
    unsigned long getGameTime() const;
};

}  // namespace World
}  // namespace Project

#endif
