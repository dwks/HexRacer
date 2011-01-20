#ifndef PROJECT_SETTINGS__SETTINGS_MANAGER_H
#define PROJECT_SETTINGS__SETTINGS_MANAGER_H

#include <string>
#include <map>

namespace Project {
namespace Settings {

/** Singleton class which contains global settings read from a configuration
    file.
*/
class SettingsManager {
private:
    static SettingsManager *instance;
public:
    static SettingsManager *getInstance() { return instance; }
private:
    static int INT_UNSET;
    static double DOUBLE_UNSET;
    
    /** A single setting value. This is stored as a string but can be converted
        to an int or a double upon request. The converted int/double values are
        cached for the next time they are requested.
    */
    class Setting {
    private:
        std::string stringData;
        mutable int intData;
        mutable double doubleData;
    public:
        Setting(std::string string = "") : stringData(string),
            intData(INT_UNSET), doubleData(DOUBLE_UNSET) {}
        
        const std::string &getString() const { return stringData; }
        int getInt(int defaultValue) const;
        double getDouble(double defaultValue) const;
        
        int getIntData() const { return intData; }
        double getDoubleData() const { return doubleData; }
    };
private:
    typedef std::map<std::string, Setting> settingMapType;
    settingMapType settingMap;
public:
    SettingsManager(const char *filename = NULL);
    
    /** Loads settings contained in @a filename.
    */
    void load(const char *filename);
    
    /** Sets the setting @a key to @a value.
    */
    void set(const std::string &key, const std::string &value);
    
    /** Returns the setting @a key as a string.
        
        If the setting does not exist then @a defaultValue will be returned.
    */
    std::string get(const std::string &key,
        const std::string &defaultValue);
    
    /** Returns the setting @a key as an int.
        
        If the setting does not exist then @a defaultValue will be returned.
    */
    int get(const std::string &key, int defaultValue);
    
    /** Returns the setting @a key as a double.
        
        If the setting does not exist then @a defaultValue will be returned.
    */
    double get(const std::string &key, double defaultValue);
    
    /** Removes (unsets) the setting @a key.
    */
    void remove(const std::string &key);
    
    /** Removes all settings. This may be useful before loading a new
        configuration file.
    */
    void removeAll();
    
    /** Dumps all the settings for examination to a log file.
    */
    void dump();
};

#define GET_SETTING(key, defaultValue) \
    ::Project::Settings::SettingsManager::getInstance() \
        ->get(key, defaultValue)

}  // namespace Settings
}  // namespace Project

#endif
