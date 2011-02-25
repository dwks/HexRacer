#ifndef PROJECT_GUI__MAP_LIST_H
#define PROJECT_GUI__MAP_LIST_H

#include <string>
#include <vector>

#include "widget/ListWidget.h"

namespace Project {
namespace GUI {

class MapList : public Widget::ListWidget {
private:
    int maps;
public:
    MapList(const std::string &name, bool vertical, bool horizontal,
        const Widget::WidgetRect &bounds);
    
    int getMaps() const { return maps; }
private:
    std::string mapRoot(int map);
};

}  // namespace GUI
}  // namespace Project

#endif
