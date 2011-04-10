#include "BonusMessageQueue.h"
#include "misc/StreamAsString.h"
#include "world/TimeElapsed.h"
#include "event/EventSystem.h"
#include "render/FontManager.h"
#include "map/Teams.h"

namespace Project {
namespace Bonus {

	BonusMessageQueue::BonusMessageQueue()
		: worldManager(NULL) {
		METHOD_OBSERVER(&BonusMessageQueue::bonusEventHandler);
	}

	void BonusMessageQueue::bonusEventHandler(Event::BonusEvent* event) {

		if (!worldManager)
			return;

		boost::shared_ptr<Render::StringTexture> new_texture = boost::shared_ptr<Render::StringTexture>(new Render::StringTexture());
		new_texture->setString(Misc::StreamAsString() << worldManager->getPlayer(event->getPlayerID())->getPlayerName() << ' '
			<< event->getName() << " +" << event->getPoints(),
			Render::FontManager::getInstance()->getCurrentFont(),
			(Map::Teams::teamColor(worldManager->getPlayer(event->getPlayerID())->getTeamID())*0.75) + (OpenGL::Color(OpenGL::Color::WHITE)*0.25) );

		BonusMessage message;
		message.texture = new_texture;
		message.appearTime = World::TimeElapsed::getInstance().getGameTime();
		
		messageQueue.push_back(message);
	}

	const std::list<BonusMessageQueue::BonusMessage>& BonusMessageQueue::getQueue() {

		unsigned long current_time = World::TimeElapsed::getInstance().getGameTime();
		int duration = GET_SETTING("hud.bonusmessages.appearduration", 2000);

		while (!messageQueue.empty() && (*messageQueue.begin()).appearTime+duration < current_time)
			messageQueue.pop_front();

		return messageQueue;
	}

}  // namespace Bonus
}  // namespace Project
