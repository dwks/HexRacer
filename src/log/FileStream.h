#ifndef PROJECT_LOG__FILE_STREAM_H
#define PROJECT_LOG__FILE_STREAM_H

#include <fstream>
#include "AbstractStream.h"

namespace Project {
namespace Log {

class FileStream : public AbstractStream {
private:
    std::ofstream stream;
public:
    FileStream(VerbosityLevel level, const char *filename);
    
    virtual void write(const std::string &data);
};

}  // namespace Log
}  // namespace Project

#endif
