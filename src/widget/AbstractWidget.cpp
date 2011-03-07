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

boost::shared_ptr<Layout> AbstractWidget::getLayout() const {
    return layout;
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

WidgetBase *AbstractWidget::getChildPath(const std::string &path) {
    std::string::size_type end = path.find('/');
    Widget::WidgetBase *child = getChild(path.substr(0, end));
    
    if(!child) return NULL;
    
    if(end == std::string::npos) {
        return child;
    }
    else {
        return child->getChildPath(path.substr(end + 1));
    }
}

}  // namespace Widget
}  // namespace Project
