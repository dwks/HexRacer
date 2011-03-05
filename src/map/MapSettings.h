#ifndef PROJECT_MAP__MAP_SETTINGS_H
#define PROJECT_MAP__MAP_SETTINGS_H

#include <string>

namespace Project {
namespace Map {

class MapSettings {
private:
    static MapSettings *instance;
public:
    static MapSettings *getInstance() { return instance; }
private:
    std::string mapFile;
    std::string mapThumbnail;
    std::string mapTitle;
public:
    MapSettings() { instance = this; }
    
    void setMapFile(const std::string &filename);
    const std::string &getMapFile() const;
    
    void setMapThumbnail(const std::string &filename);
    const std::string &getMapThumbnail() const;
    
    void setMapTitle(const std::string &title);
    const std::string &getMapTitle() const;
};

}  // namespace Map
}  // namespace Project

#endif
