#include "MapSettings.h"

#include "settings/SettingsManager.h"
#include "misc/StreamAsString.h"

namespace Project {
namespace Map {

MapSettings *MapSettings::instance = 0;

MapSettings::MapSettings() {
    instance = this;
    
    loadMapList();
    
    setMapFile(GET_SETTING("map", ""));
}

void MapSettings::loadMapList() {
    int maps = 0;
    for(;;) {
        std::string root = Misc::StreamAsString() << "maps." << (maps+1);
        std::string hrm = GET_SETTING(root, "");
        if(hrm.length() == 0) break;
        
        std::string title = GET_SETTING(root + ".title", hrm);
        std::string image = GET_SETTING(root + ".image", hrm);
        
        SingleMap singleMap(hrm, image, title);
        mapList[hrm] = singleMap;
        mapVector.push_back(singleMap);
        
        maps ++;
    }
}

void MapSettings::setMapFile(const std::string &filename) {
    Settings::SettingsManager::getInstance()->set("map", filename);
    currentMap = mapList[filename];
    
    if(currentMap.getFile() == "") {
        currentMap = SingleMap(filename, "", "");
    }
}

const std::string &MapSettings::getMapFile() const {
    return currentMap.getFile();
}

const std::string &MapSettings::getMapThumbnail() const {
    return currentMap.getThumbnail();
}

const std::string &MapSettings::getMapTitle() const {
    return currentMap.getTitle();
}

}  // namespace Map
}  // namespace Project
