#ifndef PROJECT_LOG__LOGGER_H
#define PROJECT_LOG__LOGGER_H

#include <vector>
#include <iosfwd>

#include "Message.h"
#include "Stream.h"
#include "Subsystem.h"
#include "misc/Singleton.h"
#include "misc/StreamAsString.h"

namespace Project {
namespace Log {

class Logger /*: public Misc::Singleton<Logger>*/ {
private:
	static Logger instance;
public:
	static Logger &getInstance() { return instance; }
private:
    typedef std::vector<Stream *> stream_list_t;
    stream_list_t streamList;
protected:
    friend class Misc::Singleton<Logger>;
public:
    /** The Logger takes ownership of @a stream and will delete it when
        necessary.
    */
    void addStream(Stream *stream);
    void closeAllStreams();
    
    void log(const Message &message);
};

}  // namespace Log
}  // namespace Project

#define LOG(subsystem, text) \
    ::Project::Log::Logger::getInstance().log( \
        ::Project::Log::Message( \
            ::Project::Log::MessageType( \
                ::Project::Log::Subsystem::subsystem), \
            ::Project::Misc::StreamAsString() << text))

#define LOG2(subsystem, type, text) \
    ::Project::Log::Logger::getInstance().log( \
        ::Project::Log::Message( \
            ::Project::Log::MessageType( \
                ::Project::Log::Subsystem::subsystem, \
                # type), \
            ::Project::Misc::StreamAsString() << text))

#endif
