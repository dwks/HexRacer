#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

#include "AboutProxy.h"

#include "widget/WidgetActivateEvent.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void AboutProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "back") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void AboutProxy::loadHelpText(Widget::ListWidget *text) {
    std::ifstream stream("ingameabout.txt");
    
    int count = 0;
    std::string line;
    while(std::getline(stream, line)) {
        // remove DOS line endings on Linux
        while(line.size() > 0 && std::isspace(line[line.size() - 1])) {
            line.resize(line.size() - 1);
        }
        
        if(line.substr(0, 4) == "=== ") {
            text->addChild(new Widget::TextWidget(
                Misc::StreamAsString() << "line" << (++count),
                line.substr(4),
                Widget::NormalTextLayout::ALIGN_LEFT,
                Widget::WidgetRect(0.0, 0.0, 0.8, 0.06),
                OpenGL::Color(0.0f, 0.8f, 1.0f)));
        }
        else if(line.substr(0, 4) == "--- ") {
            text->addChild(new Widget::TextWidget(
                Misc::StreamAsString() << "line" << (++count),
                line.substr(4),
                Widget::NormalTextLayout::ALIGN_LEFT,
                Widget::WidgetRect(0.0, 0.0, 0.9, 0.04),
                OpenGL::Color(0.0f, 0.6f, 0.8f)));
        }
		else if(line.substr(0, 4) == "!!! ") {
            text->addChild(new Widget::TextWidget(
                Misc::StreamAsString() << "line" << (++count),
                line.substr(4),
                Widget::NormalTextLayout::ALIGN_LEFT,
                Widget::WidgetRect(0.0, 0.0, 0.9, 0.03),
				OpenGL::Color::RED));
        }
        else {
            text->addChild(new Widget::TextWidget(
                Misc::StreamAsString() << "line" << (++count),
                line,
                Widget::NormalTextLayout::ALIGN_LEFT,
                Widget::WidgetRect(0.0, 0.0, 0.9, 0.03),
                OpenGL::Color(0.9f, 1.0f, 1.0f)));
        }
    }
}

}  // namespace GUI
}  // namespace Project
