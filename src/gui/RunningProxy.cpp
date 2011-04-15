#include "RunningProxy.h"

#include "widget/WidgetBase.h"
#include "widget/KeyEvent.h"
#include "widget/WidgetActivateEvent.h"
#include "widget/TextWidget.h"

#include "event/PauseGame.h"

#include "event/EventSystem.h"

#include "log/Logger.h"
#include "misc/StreamAsString.h"

#include "settings/SettingsManager.h"

#include "SDL_keysym.h"

namespace Project {
namespace GUI {

void RunningProxy::playerProgressHandler(Event::PlayerProgressEvent *event) {
    Widget::TextWidget *lapCount
        = dynamic_cast<Widget::TextWidget *>(running->getChild("lapcount"));

	if (GET_SETTING("internal.practicemode", false)) {
		lapCount->setText(Misc::StreamAsString() << "Lap " << (event->getLapCount()+1));
	}
	else {
    
		if (event->getLapCount()+1 < event->getTotalLaps()) {
			lapCount->setText(Misc::StreamAsString()
				<< "Lap " << (event->getLapCount()+1)
				<< '/' << (event->getTotalLaps())
				);
		}
		else
			lapCount->setText("Final Lap");

	}
}

RunningProxy::RunningProxy(Widget::WidgetBase *running) : running(running) {
    METHOD_OBSERVER(&RunningProxy::playerProgressHandler);
}

void RunningProxy::visit(Widget::KeyEvent *event) {
    if(event->getKey() == SDLK_ESCAPE && event->getDown()) {
        EMIT_EVENT(new Event::PauseGame(true));
    }
}

}  // namespace GUI
}  // namespace Project
