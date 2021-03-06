#include "Sleeper.h"

#include "boost/date_time/posix_time/posix_time.hpp"
//#include "SDL.h"

#ifdef WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace Project {
namespace Misc {

unsigned long Sleeper::firstMilliseconds = -1;

void Sleeper::sleep(unsigned milliseconds) {
    //SDL_Delay(milliseconds);
    //return;
    
    // !!! There could also be a Boost version here if we ever start depending
    //     on Boost.Thread.
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

unsigned long Sleeper::getTimeMilliseconds() {
    //return SDL_GetTicks();
#if 1
    boost::posix_time::ptime ptime
        = boost::posix_time::microsec_clock::local_time();
    
    unsigned long ms = static_cast<unsigned long>(
        ptime.time_of_day().total_milliseconds());
    
    if(firstMilliseconds == static_cast<unsigned long>(-1)) {
        firstMilliseconds = ms;
    }
    
    return ms - firstMilliseconds;
#endif
}

}  // namespace Misc
}  // namespace Project
