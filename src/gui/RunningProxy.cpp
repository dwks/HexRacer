#include "RunningProxy.h"

#include "widget/WidgetBase.h"
#include "widget/KeyEvent.h"
#include "widget/WidgetActivateEvent.h"
#include "widget/TextWidget.h"
#include "widget/CountdownLayout.h"

#include "event/PauseGame.h"

#include "event/EventSystem.h"

#include "log/Logger.h"
#include "misc/StreamAsString.h"

#include "settings/SettingsManager.h"

#include "SDL_keysym.h"

namespace Project {
namespace GUI {

void RunningProxy::handleSwitchToScreen(Event::SwitchToScreen *event) {
    if(event->getScreen() != "running") return;
    
    Widget::TextWidget *lapcount
        = dynamic_cast<Widget::TextWidget *>(running->getChild("lapcount"));
    
    Widget::CountdownLayout *transition
        = dynamic_cast<Widget::CountdownLayout *>(lapcount->getLayout().get());
    
    transition->setIgnoreChange();
}

void RunningProxy::handleGameStageChanged(Event::GameStageChanged *event) {
    Widget::TextWidget *lapcount
        = dynamic_cast<Widget::TextWidget *>(running->getChild("lapcount"));
    
    Widget::CountdownLayout *transition
        = dynamic_cast<Widget::CountdownLayout *>(lapcount->getLayout().get());
    
    switch(event->getStage()) {
    case World::WorldSetup::COUNT_3:
        transition->setCountdown("   3   ");
        break;
    case World::WorldSetup::COUNT_2:
        transition->setCountdown("   2   ");
        break;
    case World::WorldSetup::COUNT_1:
        transition->setCountdown("   1   ");
        break;
    case World::WorldSetup::RUNNING_GAME:
        transition->setCountdown("  GO!  ");
        break;
    default:
        break;
    }
}

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
		else if(event->getLapCount()+1 == event->getTotalLaps()) {
			lapCount->setText("Final Lap");
        }
        else {
            lapCount->setText("Finished!");
        }

	}
}

RunningProxy::RunningProxy(Widget::WidgetBase *running) : running(running) {
    METHOD_OBSERVER(&RunningProxy::handleSwitchToScreen);
    METHOD_OBSERVER(&RunningProxy::handleGameStageChanged);
    METHOD_OBSERVER(&RunningProxy::playerProgressHandler);
}

void RunningProxy::visit(Widget::KeyEvent *event) {
    if(event->getKey() == SDLK_ESCAPE && event->getDown()) {
        EMIT_EVENT(new Event::PauseGame(true));
    }
}

}  // namespace GUI
}  // namespace Project
