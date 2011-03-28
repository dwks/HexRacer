#include "FinishedProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/CompositeWidget.h"
#include "widget/TextWidget.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"
#include "event/QuitEvent.h"

#include "misc/StreamAsString.h"
//#include "settings/SettingsManager.h"

namespace Project {
namespace GUI {

void FinishedProxy::handleRaceFinished(Event::RaceFinished *event) {
    LOG(GLOBAL, "Race finished!");
    
    Widget::CompositeWidget *ranks = dynamic_cast<Widget::CompositeWidget *>(
        finished->getChild("ranks"));
    ranks->removeAllChildren();
    
    Widget::WidgetRect scoreArea(0.25, 0.1, 0.5, 0.7);
    
    /*Settings::SettingsManager::getInstance()->set(
        "hud.placinglist.enable", "0");*/
    
    Map::RaceResults results = event->getResults();
    for(int r = 0; r < results.getRanks() && r < 10; r ++) {
        int p = results.getPlayerByRank(r);
        int score = results.getPlayerPoints(p);
        
        double height = scoreArea.getHeight() / 11.0;
        
        Object::Player *player = playerManager->getPlayer(p);
        
        Widget::WidgetRect textRect(
            scoreArea.getCorner().getX(),
            scoreArea.getCorner().getY() + height * r,
            scoreArea.getWidth(),
            height);
        
        ranks->addChild(
            new Widget::TextWidget(Misc::StreamAsString() << "rank" << r,
                player->getPlayerName(),
                Widget::NormalTextLayout::ALIGN_LEFT,
                textRect,
                static_cast<OpenGL::Color::ColorPreset>(player->getTeamID())));
        ranks->addChild(
            new Widget::TextWidget(Misc::StreamAsString() << "score" << r,
                Misc::StreamAsString() << score,
                Widget::NormalTextLayout::ALIGN_RIGHT,
                textRect));
    }
    
    EMIT_EVENT(new Event::SwitchToScreen("finished"));
}

void FinishedProxy::handleBasicWorldConstructed(
    Event::BasicWorldConstructed *event) {
    
    this->playerManager = event->getBasicWorld()->getPlayerManager();
}

FinishedProxy::FinishedProxy(Widget::WidgetBase *finished)
    : finished(finished), playerManager(NULL) {
    
    METHOD_OBSERVER(&FinishedProxy::handleRaceFinished);
    METHOD_OBSERVER(&FinishedProxy::handleBasicWorldConstructed);
}

void FinishedProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "quit") {
        EMIT_EVENT(new Event::QuitEvent());
    }
}

}  // namespace GUI
}  // namespace Project
