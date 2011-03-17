#include <sstream>

#include "SettingsProxy.h"

#include "widget/WidgetBase.h"
#include "widget/EditWidget.h"
#include "widget/ListWidget.h"

#include "widget/RepaintEvent.h"
#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/WidgetSelectedEvent.h"

#include "event/SwitchToScreen.h"
#include "event/ChangeScreenMode.h"
#include "event/EventSystem.h"

#include "input/GlobalInputManager.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void SettingsProxy::handleSwitchToScreen(Event::SwitchToScreen *event) {
    if(checkingJoystick) {
        setCheckingForJoystick(false);
    }
}

SettingsProxy::SettingsProxy(Widget::WidgetBase *settings)
    : settings(settings), checkingJoystick(false) {
    
    METHOD_OBSERVER(&SettingsProxy::handleSwitchToScreen);
}

void SettingsProxy::visit(Widget::RepaintEvent *event) {
    if(checkingJoystick) {
        if(Input::GlobalInputManager::getInstance()->findJoystick()) {
            LOG(GUI, "Found joystick!");
            setCheckingForJoystick(false);
        }
    }
}

void SettingsProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "accept") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
    else if(name == "fullscreen") {
        int width = GET_SETTING("screen.width", 0);
        int height = GET_SETTING("screen.height", 0);
        int bpp = GET_SETTING("display.bpp", 0);
        bool fullscreen = event->getDown();
        
        EMIT_EVENT(new Event::ChangeScreenMode(
            width, height, bpp, fullscreen));
    }
    else if(name == "shadow") {
        Settings::SettingsManager::getInstance()->set(
            "render.shadow.enable",
			event->getDown() ? "1" : "0");
		LOG(GUI, "shadow button: " << event->getDown());
    }
    else if(name == "bloom") {
        Settings::SettingsManager::getInstance()->set(
            "render.bloom.enable",
            event->getDown() ? "1" : "0");
    }
    else if(name == "joyselect") {
        if(dynamic_cast<Widget::ButtonWidget *>(settings->getChild("joyselect"))
            ->getText()->getData() == "Select joystick") {
        
            LOG(GUI, "Checking for joystick...");
            setCheckingForJoystick(true);
        }
        else {
            LOG(GUI, "Cancel joystick checking");
            setCheckingForJoystick(false);
        }
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void SettingsProxy::visit(Widget::WidgetModifiedEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "screenmode") {
        int width, height;
        char x;
        std::istringstream stream(event->getWidget()->getData());
        if(!(stream >> width >> x >> height) || x != 'x') {
            LOG(GUI, "SettingsProxy: malformed screen mode \""
                << event->getWidget()->getData() << "\"");
            event->getWidget()->setData(Misc::StreamAsString()
                << GET_SETTING("display.width", 0) << 'x'
                << GET_SETTING("display.height", 0));
            return;
        }
        
        int bpp = GET_SETTING("display.bpp", 0);
        bool fullscreen = GET_SETTING("display.fullscreen", 0);
        
        EMIT_EVENT(new Event::ChangeScreenMode(
            width, height, bpp, fullscreen));
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

void SettingsProxy::visit(Widget::WidgetSelectedEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "quality") {
        Settings::SettingsManager::getInstance()->set(
            "render.quality", event->getSelected()->getName());
    }
    else {
        LOG2(GUI, WARNING, "No action for selecting \"" << name << "\"");
    }
}

void SettingsProxy::setCheckingForJoystick(bool yes) {
    if(yes) {
        Input::GlobalInputManager::getInstance()->startFindJoystick();
        
        dynamic_cast<Widget::ButtonWidget *>(settings->getChild("joyselect"))
            ->getText()->setText("Press any button on your joystick...");
    }
    else {
        Input::GlobalInputManager::getInstance()->cancelFindJoystick();
        
        dynamic_cast<Widget::ButtonWidget *>(settings->getChild("joyselect"))
            ->getText()->setText("Select joystick");
    }
    
    checkingJoystick = yes;
}

}  // namespace GUI
}  // namespace Project
