#include "BonusMessages.h"

namespace Project {
namespace HUD {

	BonusMessages::BonusMessages()
		: messageQueue(),
		entry(Render::TextureObjectChain::HORIZONTAL,
		Render::TextureObjectChain::MID,
		Render::TextureObjectChain::MAX) {

	}

	void BonusMessages::render() {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, totalWidth, 0.0, totalHeight, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		double y = totalHeight-entryHeight;

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
