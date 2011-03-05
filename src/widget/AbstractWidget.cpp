#include "AbstractWidget.h"
#include "Layout.h"
#include "EventProxy.h"

namespace Project {
namespace Widget {

AbstractWidget::~AbstractWidget() {
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

Layout *AbstractWidget::getLayout() const {
    return layout.get();
}

void AbstractWidget::setLayout(Layout *layout) {
    this->layout = boost::shared_ptr<Layout>(layout);
}

void AbstractWidget::handleEvent(WidgetEvent *event) {
    for(proxy_list_t::iterator i = proxyList.begin(); i != proxyList.end();
        ++ i) {
        
        boost::shared_ptr<EventProxy> proxy = *i;
        event->accept(*proxy);
    }
}

void AbstractWidget::addEventProxy(boost::shared_ptr<EventProxy> proxy) {
    proxyList.push_back(proxy);
}

void AbstractWidget::addEventProxy(EventProxy *proxy) {
    addEventProxy(boost::shared_ptr<EventProxy>(proxy));
}

void AbstractWidget::removeAllEventProxies() {
    // smart pointers automatically free proxies if necessary
    proxyList.clear();
}

}  // namespace Widget
}  // namespace Project
