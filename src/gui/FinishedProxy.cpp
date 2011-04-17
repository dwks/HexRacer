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
    double scoreHeight = scoreArea.getHeight() / 11.0;
	double teamHeight = teamArea.getHeight() / 6.0;
    
    Map::RaceResults results = event->getResults();
    for(int r = 0; r <= results.getRanks() && r <= 10; r ++) {

        Widget::WidgetRect textRect(
            scoreArea.getCorner().getX(),
            scoreArea.getCorner().getY() + scoreHeight * r,
            scoreArea.getWidth() / 3.0 - 0.03,
            scoreHeight);
        Widget::WidgetRect textRect1 = textRect;
        Widget::WidgetRect textRect2 = textRect;
        Widget::WidgetRect textRect3 = textRect;
        
        textRect2.getCorner().addX(scoreArea.getWidth() / 3.0);
        textRect3.getCorner().addX(scoreArea.getWidth() / 3.0 * 2);

		if (r == 0) {

			ranks->addChild(
				new Widget::TextWidget("playerColTitle",
					"Player",
					Widget::NormalTextLayout::ALIGN_HCENTRE,
					textRect1));
			ranks->addChild(
				new Widget::TextWidget("timeColTitle",
					"Time - Bonus",
					Widget::NormalTextLayout::ALIGN_HCENTRE,
					textRect2));
			ranks->addChild(
				new Widget::TextWidget("bonusColTitle",
					"Final",
					Widget::NormalTextLayout::ALIGN_HCENTRE,
					textRect3));

		}
		else {

			int p = results.getPlayerByRank(r-1);
			int score = results.getPlayerPoints(p);
			int bonus = results.getPlayerBonus(p);
	        
			Object::Player *player = playerManager->getPlayer(p);
	        
			ranks->addChild(
				new Widget::TextWidget(Misc::StreamAsString() << "rank" << r-1,
					player->getPlayerName(),
					Widget::NormalTextLayout::ALIGN_LEFT,
					textRect1,
					Map::Teams::teamColor(player->getTeamID())));
			ranks->addChild(
				new Widget::TextWidget(Misc::StreamAsString() << "time" << r-1,
					Misc::StreamAsString() << formatTime((score+bonus) / 1000.0)
                        << " - " << formatTime(bonus / 1000.0) << " secs",
					Widget::NormalTextLayout::ALIGN_HCENTRE,
					textRect2));
			ranks->addChild(
				new Widget::TextWidget(Misc::StreamAsString() << "final" << r-1,
					Misc::StreamAsString() << "= " << formatTime(score / 1000.0) << " secs",
					Widget::NormalTextLayout::ALIGN_RIGHT,
					textRect3));

		}

    }
    
    for(int r = 0; r <= results.getTeams() && r <= 5; r ++) {
        Widget::WidgetRect textRect(
            teamArea.getCorner().getX(),
            teamArea.getCorner().getY() + teamHeight * r,
            teamArea.getWidth() / 2.0 - 0.03,
            teamHeight);
        Widget::WidgetRect textRect1 = textRect;
        Widget::WidgetRect textRect2 = textRect;
        
        textRect2.getCorner().addX(scoreArea.getWidth() / 2.0);
        
        if(r == 0) {
            ranks->addChild(
                new Widget::TextWidget("teamNamesTitle",
                    "Best Teams",
                    Widget::NormalTextLayout::ALIGN_LEFT,
                    textRect1));
            ranks->addChild(
                new Widget::TextWidget("teamScoresTitle",
                    "Average Score",
                    Widget::NormalTextLayout::ALIGN_RIGHT,
                    textRect2));
        }
        else {
            int team = results.getTeamByRank(r-1);
            int score = results.getTeamPoints(team);
            
            ranks->addChild(
                new Widget::TextWidget(Misc::StreamAsString() << "rank" << r-1,
                    Misc::StreamAsString() << "Team " << Map::Teams::teamName(team),
                    Widget::NormalTextLayout::ALIGN_LEFT,
                    textRect1,
                    Map::Teams::teamColor(team)));
            ranks->addChild(
                new Widget::TextWidget(Misc::StreamAsString() << "team" << r-1,
                    Misc::StreamAsString() << formatTime(score / 1000.0) << " secs",
                    Widget::NormalTextLayout::ALIGN_RIGHT,
                    textRect2));
        }
    }
    
    EMIT_EVENT(new Event::SwitchToScreen("finished"));
}

std::string FinishedProxy::formatTime(double time) {
    char buffer[128];
    std::sprintf(buffer, "%7.3f", time);
    return std::string(buffer);
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
    else if(name == "dismiss") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
}

}  // namespace GUI
}  // namespace Project
