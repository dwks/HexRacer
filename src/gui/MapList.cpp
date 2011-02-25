#include "MapList.h"
#include "widget/TextWidget.h"

#include "settings/SettingsManager.h"

#include "misc/StreamAsString.h"

namespace Project {
namespace GUI {

MapList::MapList(const std::string &name, bool vertical, bool horizontal,
    const Widget::WidgetRect &bounds)
    : Widget::ListWidget(name, vertical, horizontal, bounds) {
    
    maps = 0;
    for(;;) {
        std::string root = mapRoot(maps);
        std::string hrm = GET_SETTING(root, "");
        if(hrm.length() == 0) break;
        
        std::string title = GET_SETTING(root + ".title", hrm);
        
        addChild(new Widget::TextWidget(root, title,
            Widget::NormalTextLayout::ALIGN_LEFT,
            Widget::WidgetRect(0.0, 0.0, 0.8, 0.05)));
        
        maps ++;
    }
}

std::string MapList::mapRoot(int map) {
    return Misc::StreamAsString() << "maps." << (map+1);
}

}  // namespace GUI
}  // namespace Project
