#include <iostream>  // for debugging
#include "Logger.h"
#include "TypeRegistry.h"

namespace Project {
namespace Log {

//template <>
//Logger *Misc::Singleton<Logger>::instance = 0;
Logger Logger::instance;

void Logger::closeAllStreams() {
    for(stream_list_t::iterator it = streamList.begin();
        it != streamList.end(); ++ it) {
        
        delete *it;
    }
}

void Logger::addStream(Stream *stream) {
    streamList.push_back(stream);
}

void Logger::log(const Message &message) {
    for(stream_list_t::iterator it = streamList.begin();
        it != streamList.end(); ++ it) {
        
        Stream *stream = *it;
        
        if(stream->getLevel() >= message.getType().getLevel()) {
            stream->write(message.asText(stream->isWideStream()));
        }
    }
}

}  // namespace Log
}  // namespace Project
