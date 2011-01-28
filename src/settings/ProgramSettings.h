#ifndef PROJECT_SETTINGS__PROGRAM_SETTINGS_H
#define PROJECT_SETTINGS__PROGRAM_SETTINGS_H

namespace Project {
namespace Settings {

class ProgramSettings {
private:
    static ProgramSettings *instance;
public:
    static ProgramSettings *getInstance() { return instance; }
private:
    bool client;
    bool connected;
public:
    ProgramSettings(bool client);
    
    bool isClient() const { return client; }
    bool isServer() const { return !client; }
    
    void setConnected(bool connected) { this->connected = connected; }
    
    bool isConnectedClient() const { return connected; }
};

}  // namespace Settings
}  // namespace Project

#endif
