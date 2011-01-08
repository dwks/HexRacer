#include <ostream>
#include "AliasStream.h"

namespace Project {
namespace Log {

AliasStream::AliasStream(VerbosityLevel level, std::ostream &stream,
    bool wideStream)
    : AbstractStream(level, wideStream), stream(stream) {
    
}

void AliasStream::write(const std::string &data) {
    stream << data;
    stream.flush();
}

}  // namespace Log
}  // namespace Project
