#include "MapSettings.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace Map {

MapSettings *MapSettings::instance = 0;

void MapSettings::setMapFile(const std::string &filename) {
    Settings::SettingsManager::getInstance()->set("map", filename);
    this->mapFile = filename;
}

const std::string &MapSettings::getMapFile() const {
    return mapFile;
}

void MapSettings::setMapThumbnail(const std::string &filename) {
    this->mapThumbnail = filename;
}

const std::string &MapSettings::getMapThumbnail() const {
    return mapThumbnail;
}

void MapSettings::setMapTitle(const std::string &name) {
    this->mapTitle = name;
}

const std::string &MapSettings::getMapTitle() const {
    return mapTitle;
}

}  // namespace Map
}  // namespace Project
