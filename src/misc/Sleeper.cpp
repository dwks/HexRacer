#include "Sleeper.h"

#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace Project {
namespace Misc {

void Sleeper::sleep(unsigned milliseconds) {
    // !!! There could also be a Boost version here if we ever start depending
    //     on Boost.Thread.
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds);
#endif
}

}  // namespace Misc
}  // namespace Project
