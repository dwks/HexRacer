#ifndef PROJECT_LOG__STREAM_H
#define PROJECT_LOG__STREAM_H

#include <iosfwd>
#include <string>

#include "VerbosityLevel.h"

namespace Project {
namespace Log {

class Stream {
public:
    virtual ~Stream() {}
    
    virtual VerbosityLevel getLevel() const = 0;
    virtual bool isWideStream() const = 0;
    
    virtual void write(const std::string &data) = 0;
};

}  // namespace Log
}  // namespace Project

#endif
