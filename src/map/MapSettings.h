#ifndef PROJECT_MAP__MAP_SETTINGS_H
#define PROJECT_MAP__MAP_SETTINGS_H

#include <string>
#include <map>
#include <vector>

namespace Project {
namespace Map {

class MapSettings {
private:
    static MapSettings *instance;
public:
    static MapSettings *getInstance() { return instance; }
public:
    class SingleMap {
    private:
        std::string file;
        std::string thumbnail;
        std::string title;
    public:
        SingleMap() {}
        SingleMap(const std::string &file, const std::string &thumbnail,
            const std::string &title)
            : file(file), thumbnail(thumbnail), title(title) {}
        
        const std::string &getFile() const { return file; }
        const std::string &getThumbnail() const { return thumbnail; }
        const std::string &getTitle() const { return title; }
    };
private:
    typedef std::map<std::string, SingleMap> mapListType;
    typedef std::vector<SingleMap> mapVectorType;
    mapListType mapList;
    mapVectorType mapVector;
    
    SingleMap currentMap;
public:
    MapSettings();
    ~MapSettings() { instance = 0; }
    
    void setMapFile(const std::string &filename);
    const std::string &getMapFile() const;
    const std::string &getMapThumbnail() const;
    const std::string &getMapTitle() const;
    
    int getMaps() const { return mapVector.size(); }
    const SingleMap &getMap(int i) const { return mapVector[i]; }
    const SingleMap &getMap(const std::string &file) const
        { return mapList.at(file); }
private:
    void loadMapList();
};

}  // namespace Map
}  // namespace Project

#endif
