#include "SpawnServer.h"

#include <cstdlib>

#include "settings/SettingsManager.h"
#include "misc/StreamAsString.h"

namespace Project {
namespace SDL {

void SpawnServer::spawn() {
    std::string serverport = GET_SETTING("network.serverport", "1820");
    
    Misc::StreamAsString command;
#ifdef WIN32
    command << "start server.exe --port " << serverport;
#else
    command << "./server --port " << serverport << " &";
#endif
    
    std::system(command.operator std::string().c_str());
}

}  // namespace SDL
}  // namespace Project
