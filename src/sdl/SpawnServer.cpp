#include "SpawnServer.h"

#include <cstdlib>

#include "settings/SettingsManager.h"
#include "misc/StreamAsString.h"

namespace Project {
namespace SDL {

void SpawnServer::spawn() {
    Misc::StreamAsString args;
    args << " \"map=" << GET_SETTING("map", "data/testtrack.hrm") << "\"";
    args << " network.serverport=" << GET_SETTING("network.serverport", "1820");
    args << " server.aicount=" << GET_SETTING("server.aicount", "0");
    args << " game.race.laps=" << GET_SETTING("game.race.laps", "3");
    args << " game.paint.allowerase=" << GET_SETTING("game.paint.allowerase", "1");
    args << " game.paint.allowoverwrite=" << GET_SETTING("game.paint.allowoverwrite", "0");
    args << " bonus.enable=" << GET_SETTING("bonus.enable", "1");
    
    Misc::StreamAsString command;
#ifdef WIN32
    command << "start server.exe" << args.operator std::string();
#else
    command << "./server" << args.operator std::string() << " &";
#endif
    
    std::system(command.operator std::string().c_str());
}

}  // namespace SDL
}  // namespace Project
