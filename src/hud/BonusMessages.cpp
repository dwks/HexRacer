#include "BonusMessages.h"
#include "event/EventSystem.h"
#include "render/FontManager.h"

namespace Project {
namespace HUD {

	BonusMessages::BonusMessages()
		: messageQueue(),
		entry(Render::TextureObjectChain::HORIZONTAL,
		Render::TextureObjectChain::MID,
		Render::TextureObjectChain::MIN) {

			playerID =-1;
			totalBonuses = 0;
			totalBonusesTexture.setString("Bonus: 0", Render::FontManager::getInstance()->getCurrentFont());
			METHOD_OBSERVER(&BonusMessages::bonusEventHandler);

	}

	void BonusMessages::bonusEventHandler(Event::BonusEvent* event) {
		if (event->getPlayerID() == playerID) {
			totalBonuses += event->getPoints();
			totalBonusesTexture.setString(Misc::StreamAsString() << "Bonus: " << totalBonuses, Render::FontManager::getInstance()->getCurrentFont());
		}
	}

	void BonusMessages::render() {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, totalWidth, 0.0, totalHeight, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		entry.clear();
		entry.append(&totalBonusesTexture);
		entry.render(totalWidth*0.5, totalHeight-entryHeight*2.0, entryHeight*2.0);

		double y = totalHeight-entryHeight*3.0;

		const std::list<Bonus::BonusMessageQueue::BonusMessage>& queue = messageQueue.getQueue();
		std::list<Bonus::BonusMessageQueue::BonusMessage>::const_iterator it = queue.begin();
		while (it != queue.end() && y >= 0) {
			entry.clear();
			entry.append((*it).texture.get());
			entry.render(totalWidth*0.5, y, entryHeight);
			y -= entryHeight;
			++it;
		}
	}

}  // namespace HUD
}  // namespace Project
