#include "LoadingProxy.h"
#include "event/EventSystem.h"

#include "widget/ImageWidget.h"

#include "map/MapSettings.h"

namespace Project {
namespace GUI {

void LoadingProxy::initialize(Event::SwitchToScreen *event) {
    if(event->getScreen() != loading->getName()) return;
    
    Widget::ImageWidget *thumbnail
        = dynamic_cast<Widget::ImageWidget *>(loading->getChild("thumbnail"));
    
    if(thumbnail) {
        thumbnail->setFilename(
            Map::MapSettings::getInstance()->getMapThumbnail());
    }
}

LoadingProxy::LoadingProxy(Widget::WidgetBase *loading) : loading(loading) {
    METHOD_OBSERVER(&LoadingProxy::initialize);
}

}  // namespace GUI
}  // namespace Project
