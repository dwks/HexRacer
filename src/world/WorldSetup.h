#ifndef PROJECT_WORLD__WORLD_SETUP_H
#define PROJECT_WORLD__WORLD_SETUP_H

#include <map>
#include <vector>
#include <string>

#include "boost/serialization/access.hpp"
#include "boost/serialization/map.hpp"

#include "opengl/Color.h"

namespace Project {
namespace World {

class WorldSetup {
private:
    static WorldSetup instance;
public:
    static WorldSetup *getInstance() { return &instance; }
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & playerSettings;
        ar & clientSettings;
        ar & stage;
    }
public:
    class PlayerSettings {
    private:
        friend class boost::serialization::access;
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned version) {
            ar & id;
            ar & name;
            ar & color;
        }
    private:
        int id;
        std::string name;
        int color;
    public:
        PlayerSettings() : id(-1), color(OpenGL::Color::RED) {}
        
        void setID(int id) { this->id = id; }
        int getID() const { return id; }
        
        void setName(const std::string &name) { this->name = name; }
        const std::string &getName() const { return name; }
        
        void setColor(int color) { this->color = color; }
        int getColor() const { return color; }
    };
    
    class ClientSettings {
    private:
        friend class boost::serialization::access;
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned version) {
            ar & id;
            ar & readyToStart;
            ar & fullyLoaded;
        }
    private:
        int id;
        bool readyToStart;
        bool fullyLoaded;
    public:
        ClientSettings() : id(-1), readyToStart(false), fullyLoaded(false) {}
        
        void setID(int id) { this->id = id; }
        int getID() const { return id; }
        
        void setReadyToStart(bool yes) { readyToStart = yes; }
        bool isReadyToStart() const { return readyToStart; }
        
        void setFullyLoaded(bool yes) { fullyLoaded = yes; }
        bool isFullyLoaded() const { return fullyLoaded; }
    };
    
    enum GameStage {
        AT_LOBBY,           // both server and client
        START_LOADING,      // server -> client
        FINISHED_LOADING,   // client -> server
        START_COUNTDOWN,    // server -> client
        RUNNING_GAME,       // both server and client
        FINISHED            // server -> client
    };
private:
    typedef std::map<int, PlayerSettings> PlayerSettingsList;
    typedef std::map<int, ClientSettings> ClientSettingsList;
    PlayerSettingsList playerSettings;
    ClientSettingsList clientSettings;
    GameStage stage;
    int clientID;
public:
    WorldSetup() : stage(AT_LOBBY), clientID(-1) {}
    
    /** Should only be called from the server.
    */
    void addPlayerSettings(int id);
    
    /** Should only be called from the server.
    */
    void addClientSettings(int id);
    
    /** Should only be called from the server.
    */
    void replacePlayerSettings(PlayerSettings settings)
        { playerSettings[settings.getID()] = settings; }
    
    /** Should only be called from the server.
    */
    void replaceClientSettings(ClientSettings settings)
        { clientSettings[settings.getID()] = settings; }
    
    PlayerSettings *getPlayerSettings(int id);
    ClientSettings *getClientSettings(int id);
    
    bool everyoneReadyToStart() const;
    bool everyoneFullyLoaded() const;
    
    /** This only works in the client code, of course.
    */
    int getClientID() const { return clientID; }
    
    void setClientID(int id) { clientID = id; }
    
    void getAllPlayerIDs(std::vector<int> &list);
};

}  // namespace World
}  // namespace Project

#endif
