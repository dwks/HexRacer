#include "FinishedProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/CompositeWidget.h"
#include "widget/TextWidget.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"
#include "event/QuitEvent.h"

#include "map/Teams.h"

#include "misc/StreamAsString.h"

namespace Project {
namespace GUI {

void FinishedProxy::handleRaceFinished(Event::RaceFinished *event) {
    LOG(GLOBAL, "Race finished!");
    
    Widget::CompositeWidget *ranks = dynamic_cast<Widget::CompositeWidget *>(
        finished->getChild("ranks"));
    ranks->removeAllChildren();
    
    Widget::CompositeWidget *teams = dynamic_cast<Widget::CompositeWidget *>(
        finished->getChild("teams"));
    teams->removeAllChildren();
    
    Widget::WidgetRect scoreArea(0.1, 0.45, 0.8, 0.4);
    Widget::WidgetRect teamArea(0.1, 0.05, 0.8, 0.35);
    
    Map::RaceResults results = event->getResults();
    for(int r = 0; r < results.getRanks() && r < 10; r ++) {
        int p = results.getPlayerByRank(r);
        int score = results.getPlayerPoints(p);
		int bonus = results.getPlayerBonus(p);
        
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
                Map::Teams::teamColor(player->getTeamID())));
        ranks->addChild(
            new Widget::TextWidget(Misc::StreamAsString() << "time" << r,
                Misc::StreamAsString() << ((score+bonus) / 1000.0) << " secs",
				Widget::NormalTextLayout::ALIGN_HCENTRE,
                textRect));
		ranks->addChild(
            new Widget::TextWidget(Misc::StreamAsString() << "bonus" << r,
                Misc::StreamAsString() << (bonus / 1000.0) << " secs",
                Widget::NormalTextLayout::ALIGN_RIGHT,
                textRect));
    }
    
    for(int r = 0; r < results.getTeams() && r < 5; r ++) {
        int team = results.getTeamByRank(r);
        int score = results.getTeamPoints(team);
        
        double height = teamArea.getHeight() / 6.0;
        
        Widget::WidgetRect textRect(
            teamArea.getCorner().getX(),
            teamArea.getCorner().getY() + height * r,
            teamArea.getWidth(),
            height);
        
        ranks->addChild(
            new Widget::TextWidget(Misc::StreamAsString() << "rank" << r,
                Misc::StreamAsString() << "Team " << Map::Teams::teamName(team),
                Widget::NormalTextLayout::ALIGN_LEFT,
                textRect,
                Map::Teams::teamColor(team)));
        ranks->addChild(
            new Widget::TextWidget(Misc::StreamAsString() << "team" << r,
                Misc::StreamAsString() << (score / 1000.0) << " secs",
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
