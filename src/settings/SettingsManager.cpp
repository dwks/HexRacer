#include <istream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>

#include "SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace Settings {

SettingsManager *SettingsManager::instance = NULL;

int SettingsManager::INT_UNSET = std::numeric_limits<int>::min();
double SettingsManager::DOUBLE_UNSET = std::numeric_limits<double>::min();

int SettingsManager::Setting::getInt(int defaultValue) const {
    if(intData != INT_UNSET) return intData;
    
    std::istringstream stream(stringData);
    if(stream >> intData) {
        return intData;
    }
    else {
        LOG2(GLOBAL, SETTING, "Can't parse \"" << stringData << "\" as int");
        
        return defaultValue;
    }
}

double SettingsManager::Setting::getDouble(double defaultValue) const {
    if(doubleData != DOUBLE_UNSET) return doubleData;
    
    std::istringstream stream(stringData);
    if(stream >> doubleData) {
        return doubleData;
    }
    else {
        LOG2(GLOBAL, SETTING,
            "Can't parse \"" << stringData << "\" as double");
        
        return defaultValue;
    }
}

SettingsManager::SettingsManager(const char *filename) {
    instance = this;
    
    if(filename) load(filename);
}

void SettingsManager::load(const char *filename) {
    if(!filename) return;
    std::ifstream stream(filename);
    
    if(!stream.is_open()) {
        LOG2(GLOBAL, SETTING, "Error opening settings file "
            "\"" << filename << "\"");
        return;
    }
    
    std::string line;
    while(std::getline(stream, line)) {
        // remove comments
        std::string::size_type hash = line.find('#');
        if(hash != std::string::npos) line.resize(hash);
        
        // skip empty lines
        if(line.length() == 0) continue;
        
        std::istringstream linestream(line);
        
        // skip lines with no key (probably just whitespace)
        std::string key;
        if(!(linestream >> key)) continue;
        
        // if there's no '=', the line is misformatted
        char equal;
        if(!(linestream >> equal) || equal != '=') {
            LOG2(GLOBAL, SETTING, "Misformatted line: \"" << line << "\"");
            continue;
        }
        
        // skip space after '=' if there is one
        if(linestream.peek() == ' ') linestream.get();
        
        // whatever remains is the data
        std::string data;
        std::getline(linestream, data);
        
        // record this setting
        set(key, data);
    }
}

void SettingsManager::set(const std::string &key, const std::string &value) {
    settingMap[key] = Setting(value);
}

std::string SettingsManager::get(const std::string &key,
    const std::string &defaultValue) {
    
    settingMapType::iterator found = settingMap.find(key);
    if(found == settingMap.end()) {
        return defaultValue;
    }
    
    return (*found).second.getString();
}

int SettingsManager::get(const std::string &key, int defaultValue) {
    settingMapType::iterator found = settingMap.find(key);
    if(found == settingMap.end()) {
        return defaultValue;
    }
    
    return (*found).second.getInt(defaultValue);
}

double SettingsManager::get(const std::string &key, double defaultValue) {
    settingMapType::iterator found = settingMap.find(key);
    if(found == settingMap.end()) {
        return defaultValue;
    }
    
    return (*found).second.getDouble(defaultValue);
}

void SettingsManager::remove(const std::string &key) {
    settingMap.erase(key);
}

void SettingsManager::removeAll() {
    settingMap.clear();
}

void SettingsManager::dump() {
    LOG2(GLOBAL, SETTING, "Dump of all settings:");
    
    for(settingMapType::iterator i = settingMap.begin(); i != settingMap.end();
        i ++) {
        
        Setting &setting = (*i).second;
        Misc::StreamAsString stream;
        stream << "    "
            << std::left << std::setw(20) << (*i).first
            << " = " << setting.getString();
        
        if(setting.getIntData() != INT_UNSET) {
            stream << " (" << setting.getIntData() << ")";
        }
        if(setting.getDoubleData() != DOUBLE_UNSET) {
            stream << " (" << setting.getDoubleData() << ")";
        }
        
        LOG2(GLOBAL, SETTING, stream.operator std::string());
    }
}

}  // namespace Settings
}  // namespace Project
