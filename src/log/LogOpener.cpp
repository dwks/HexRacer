#include <iostream>
#include "config.h"

//#ifdef WIN32
//    #include <windows.h>
#if defined(HAVE_BOOST)
    #include "boost/filesystem.hpp"
#else
    #include <unistd.h>
    #include <sys/stat.h>
#endif

#include "LogOpener.h"
#include "Logger.h"

#include "FileStream.h"
#include "AliasStream.h"

#define LOG_DIRECTORY "logs/"

namespace Project {
namespace Log {

void LogOpener::openLogs() {
//#ifdef WIN32
//    CreateDirectory((LPCWSTR)LOG_DIRECTORY, NULL);
#if defined(HAVE_BOOST_FILESYSTEM)
    boost::filesystem::create_directory(LOG_DIRECTORY);
#else
    mkdir(LOG_DIRECTORY, 0755);
#endif
    
    std::string logfile;
    for(int x = 0; x < 10000; x ++) {
        logfile = Misc::StreamAsString() << LOG_DIRECTORY << x;

//#ifdef WIN32
//        if(GetFileAttributes((LPCWSTR)logfile.c_str()) == 0xFFFFFFFF)
#if defined(HAVE_BOOST_FILESYSTEM)
        if(!boost::filesystem::exists(logfile))
#else
        if(access(logfile.c_str(), F_OK) != 0)
#endif
        {
            break;
        }
    }
    
#if defined(WIN32) && defined(WIN32_NO_CONSOLE)
    Logger::getInstance().addStream(
        new AliasStream(VerbosityLevel(), std::cout, false));
#endif
    Logger::getInstance().addStream(
        new FileStream(VerbosityLevel(), logfile.c_str()));
    
    LOG2(GLOBAL, PROGRESS, "Logging to \"" << logfile << '"');
}

void LogOpener::closeLogs() {
    LOG2(GLOBAL, PROGRESS, "Closing log files");
    
    Logger::getInstance().closeAllStreams();
}

}  // namespace Log
}  // namespace Project
