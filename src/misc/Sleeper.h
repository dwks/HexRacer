#ifndef PROJECT_MISC__SLEEPER_H
#define PROJECT_MISC__SLEEPER_H

namespace Project {
namespace Misc {

class Sleeper {
private:
    static unsigned long firstMilliseconds;
public:
    static void sleep(unsigned milliseconds);
    
    /** Returns the number of milliseconds that have elapsed since the program
        started (or since some internal system was initialized).
    */
    static unsigned long getTimeMilliseconds();
};

}  // namespace Misc
}  // namespace Project

#endif
