#include "PaintSubsystem.h"

#include "event/PaintEvent.h"
#include "event/EventSystem.h"
#include "bonus/GlobalBonusManager.h"

#include "math/Values.h"

#include "config.h"

namespace Project {
namespace Paint {

/*void PaintSubsystem::TogglePaintingHandler::observe(
    Event::TogglePainting *toggle) {
    
    subsystem->setPainting(toggle->getID(), toggle->getPaintType());
}*/

void PaintSubsystem::togglePaintingObserver(Event::TogglePainting *toggle) {
    setPainting(toggle->getID(), toggle->getPaintType());
}

PaintSubsystem::PaintSubsystem(Object::WorldManager *worldManager,
    Paint::PaintManager *paintManager, unsigned long tickTime)
    : TimedSubsystem(tickTime), worldManager(worldManager),
    paintManager(paintManager) {
    
    METHOD_OBSERVER(&PaintSubsystem::togglePaintingObserver);
}

void PaintSubsystem::setPainting(int id,
    Event::TogglePainting::PaintType type) {
    
    painting[id] = type;
}

Event::TogglePainting::PaintType PaintSubsystem::getPainting(int id) {
    PaintingType::iterator found = painting.find(id);
    
    if(found == painting.end()) {
        return Event::TogglePainting::NOTHING;
    }
    else {
        return (*found).second;
    }
}

void PaintSubsystem::doAction(unsigned long currentTime) {
    for(PaintingType::iterator i = painting.begin(); i != painting.end();
        ++ i) {
        
        int id = (*i).first;
        Event::TogglePainting::PaintType type = (*i).second;
        
        // can't paint or erase when in the air
        if(!worldManager->getPlayer(id)->getOnGround()) continue;
        
        switch(type) {
        case Event::TogglePainting::ERASING:
			playerErase(worldManager->getPlayer(id));
			/*
            EMIT_EVENT(new Event::PaintEvent(
                worldManager->getPlayer(id)->getPosition(),
                PAINTING_RADIUS,
                -1));
			*/
            break;
        case Event::TogglePainting::PAINTING:
			playerPaint(worldManager->getPlayer(id));
			/*
            EMIT_EVENT(new Event::PaintEvent(
                worldManager->getPlayer(id)->getPosition(),
                PAINTING_RADIUS,
				worldManager->getPlayer(id)->getTeamID()));
            break;
			*/
        default:
        case Event::TogglePainting::NOTHING:
            break;
        }
    }
    
    calculateBoostSpeeds();
}

void PaintSubsystem::calculateBoostSpeeds() {

    Object::WorldManager::PlayerIteratorType iterator
        = worldManager->getPlayerIterator();

    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();

		player->setPaintType(getPainting(player->getID()));
        
        if(getPainting(player->getID()) == Event::TogglePainting::NOTHING) {

            double factor = paintManager->weightedCellsInRadius(
                player->getPosition(),
                PAINTING_RADIUS,
                player->getTeamID());

			factor *= GET_SETTING("game.paint.boostweightfactor", 2.0);
			factor += 1.0;
			factor = Math::bound(factor,
				GET_SETTING("game.paint.boostmin", 0.5),
				GET_SETTING("game.paint.boostmax", 1.5)
				);

			double max_boost_increase = GET_SETTING("game.paint.maxboostincrease", 0.01);
			double current_boost = player->getSpeedBoost();
			if (factor < current_boost+max_boost_increase)
	            player->setSpeedBoost(factor);
			else
				player->setSpeedBoost(current_boost+max_boost_increase);
        }
        else {
            // if painting or erasing, slow down the player no matter what
			if (getPainting(player->getID()) == Event::TogglePainting::PAINTING)
				player->setSpeedBoost(GET_SETTING("game.paint.paintingboost", 0.8));
			else
				player->setSpeedBoost(GET_SETTING("game.paint.erasingboost", 0.8));
        }
    }
}

void PaintSubsystem::playerPaint(Object::Player* player) {

	std::vector<Math::HexHeightMap::LayeredHexIndex> changed_indices;
	paintManager->colorCellsInRadius(player->getPosition(), PAINTING_RADIUS, player->getTeamID(), GET_SETTING("game.paint.allowoverwrite", false), &changed_indices);
	if (Bonus::GlobalBonusManager::getInstance())
		Bonus::GlobalBonusManager::getInstance()->getPlayerBonuses(player->getID()).playerPaint(static_cast<int>(changed_indices.size()));

}

void PaintSubsystem::playerErase(Object::Player* player) {

	if (GET_SETTING("game.paint.allowerase", true)) {

		std::vector<Math::HexHeightMap::LayeredHexIndex> changed_indices;
		paintManager->colorCellsInRadius(player->getPosition(), PAINTING_RADIUS, -1, false, &changed_indices);
		if (Bonus::GlobalBonusManager::getInstance())
			Bonus::GlobalBonusManager::getInstance()->getPlayerBonuses(player->getID()).playerErase(static_cast<int>(changed_indices.size()));

	}

}

}  // namespace Paint
}  // namespace Project
