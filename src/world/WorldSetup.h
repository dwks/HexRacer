#ifndef PROJECT_WORLD__WORLD_SETUP_H
#define PROJECT_WORLD__WORLD_SETUP_H

#include <map>
#include <string>

#include "boost/serialization/access.hpp"

#include "opengl/Color.h"

namespace Project {
namespace World {

class WorldSetup {
public:
    class PlayerSettings {
    private:
        friend class boost::serialization::access;
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned version) {
            ar & name;
            ar & color;
        }
    private:
        std::string name;
        OpenGL::Color color;
    public:
        PlayerSettings() : color(OpenGL::Color::RED) {}
        
        const std::string &getName() const { return name; }
        OpenGL::Color getColor() const { return color; }
    };
    
    class ClientSettings {
    private:
        friend class boost::serialization::access;
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned version) {
            ar & readyToStart;
        }
    private:
        bool readyToStart;
    public:
        ClientSettings() : readyToStart(false) {}
        
        void setReadyToStart(bool yes) { readyToStart = yes; }
        bool isReadyToStart() const { return readyToStart; }
    };
    
    enum GameStage {
        AT_LOBBY,
        DOING_COUNTDOWN,
        RUNNING_GAME,
        FINISHED
    };
private:
    typedef std::map<int, PlayerSettings> PlayerSettingsList;
    typedef std::map<int, ClientSettings> ClientSettingsList;
    PlayerSettingsList playerSettings;
    ClientSettingsList clientSettings;
    GameStage stage;
    int clientID;
public:
    WorldSetup(int clientID = -1) : stage(AT_LOBBY), clientID(clientID) {}
    
    PlayerSettings *getPlayerSettings(int id);
    ClientSettings *getClientSettings(int id);
    
    bool everyoneReadyToStart() const;
    
    /** This only works in the client code, of course.
    */
    int getClientID() const { return clientID; }
};

}  // namespace World
}  // namespace Project

#endif
