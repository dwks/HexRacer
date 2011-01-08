#ifndef PROJECT_LOG__ALIAS_STREAM_H
#define PROJECT_LOG__ALIAS_STREAM_H

#include "AbstractStream.h"

namespace Project {
namespace Log {

class AliasStream : public AbstractStream {
private:
    std::ostream &stream;
public:
    AliasStream(VerbosityLevel level, std::ostream &stream,
        bool wideStream = true);
    
    virtual void write(const std::string &data);
};

}  // namespace Log
}  // namespace Project

#endif
