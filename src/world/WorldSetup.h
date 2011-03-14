#ifndef PROJECT_WORLD__WORLD_SETUP_H
#define PROJECT_WORLD__WORLD_SETUP_H

#include <map>
#include <string>

#include "opengl/Color.h"

namespace Project {
namespace World {

class WorldSetup {
public:
    class PlayerSettings {
    private:
        std::string name;
        OpenGL::Color color;
    public:
        PlayerSettings() : color(OpenGL::Color::RED) {}
    };
    
    class ClientSettings {
    private:
        bool readyToStart;
    public:
        ClientSettings() : readyToStart(false) {}
        
        void setReadyToStart(bool yes) { readyToStart = yes; }
        bool isReadyToStart() const { return readyToStart; }
    };
private:
    typedef std::map<int, PlayerSettings> PlayerSettingsList;
    typedef std::map<int, ClientSettings> ClientSettingsList;
    PlayerSettingsList playerSettings;
    ClientSettingsList clientSettings;
public:
    PlayerSettings *getPlayerSettings(int id);
    ClientSettings *getClientSettings(int id);
    
    bool everyoneReadyToStart() const;
};

}  // namespace World
}  // namespace Project

#endif
