#include "LoadingProxy.h"
#include "event/EventSystem.h"
#include "event/JoinGame.h"

#include "widget/ImageWidget.h"
#include "widget/RepaintEvent.h"

#include "map/MapSettings.h"

#include "log/Logger.h"

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
    
    lastRepaint = NULL;
}

LoadingProxy::LoadingProxy(Widget::WidgetBase *loading) : loading(loading) {
    lastRepaint = NULL;
    
    METHOD_OBSERVER(&LoadingProxy::initialize);
}

void LoadingProxy::visit(Widget::RepaintEvent *event) {
    if(lastRepaint != event->getWidget()) {
        lastRepaint = event->getWidget();
        
        if(event->getWidget() == loading) {
            // !!! don't try to connect to the network just yet
            EMIT_EVENT(new Event::JoinGame());
        }
    }
}

}  // namespace GUI
}  // namespace Project
