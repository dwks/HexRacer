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
public:
    ProgramSettings(bool client);
    
    bool isClient() const { return client; }
    bool isServer() const { return !client; }
};

}  // namespace Settings
}  // namespace Project

#endif
