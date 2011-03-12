#ifndef PROJECT_GUI__GUICONSTRUCTION_H
#define PROJECT_GUI__GUICONSTRUCTION_H

#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class GUIConstruction {
private:
    Widget::CompositeWidget *widgets;
public:
    Widget::CompositeWidget *construct();
private:
    void constructMain();
    void constructSelectMap();
    void constructHost();
    void constructConnect();
    void constructLobby();
    void constructSinglePlayer();
    void constructLoading();
    void constructRunning();
    void constructPaused();
    void constructSettings();
private:
    Widget::WidgetBase *getWidget(const std::string &path);
    void setShortcut(Widget::WidgetBase *widget, long key);
};

}  // namespace GUI
}  // namespace Project

#endif
