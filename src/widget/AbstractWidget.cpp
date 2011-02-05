#include "AbstractWidget.h"
#include "Layout.h"
#include "EventProxy.h"

namespace Project {
namespace Widget {

AbstractWidget::~AbstractWidget() {
    delete layout;
    removeAllEventProxies();
}

void AbstractWidget::updateLayout() {
    layout->update();
}

void AbstractWidget::updateLayout(const WidgetRect &newBounds) {
    layout->update(newBounds);
}

WidgetRect AbstractWidget::getBoundingRect() const {
    return layout->getBoundingRect();
}

void AbstractWidget::handleEvent(WidgetEvent *event) {
    for(proxy_list_t::iterator i = proxyList.begin(); i != proxyList.end();
        ++ i) {
        
        EventProxy *proxy = *i;
        event->accept(*proxy);
    }
}

void AbstractWidget::addEventProxy(EventProxy *proxy) {
    proxyList.push_back(proxy);
}

void AbstractWidget::removeAllEventProxies() {
    for(proxy_list_t::iterator i = proxyList.begin(); i != proxyList.end();
        ++ i) {
        
        EventProxy *proxy = *i;
        delete proxy;
    }
    
    proxyList.clear();
}

}  // namespace Widget
}  // namespace Project
