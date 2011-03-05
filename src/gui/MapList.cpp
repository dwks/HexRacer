#include "MapList.h"
#include "widget/TextWidget.h"

#include "map/MapSettings.h"

#include "settings/SettingsManager.h"
#include "misc/StreamAsString.h"

namespace Project {
namespace GUI {

MapList::MapList(const std::string &name, bool vertical, bool horizontal,
    const Widget::WidgetRect &bounds)
    : Widget::ListWidget(name, vertical, horizontal, bounds) {
    
    Map::MapSettings *mapSettings = Map::MapSettings::getInstance();
    for(int map = 0; map < mapSettings->getMaps(); map ++) {
        std::string file = mapSettings->getMap(map).getFile();
        std::string title = mapSettings->getMap(map).getTitle();
        
        addChild(new Widget::TextWidget(file, title,
            Widget::NormalTextLayout::ALIGN_LEFT,
            Widget::WidgetRect(0.0, 0.0, 0.8, 0.05)));
    }
}

}  // namespace GUI
}  // namespace Project
