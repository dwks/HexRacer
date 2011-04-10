#ifndef PROJECT_HUD__BONUS_MESSAGES_H
#define PROJECT_HUD__BONUS_MESSAGES_H

#include "bonus/BonusMessageQueue.h"
#include "render/TextureObjectChain.h"

namespace Project {
namespace HUD {

class BonusMessages {
private:
	Bonus::BonusMessageQueue messageQueue;

	double entryHeight;
	double totalHeight;
	double totalWidth;
	Render::TextureObjectChain entry;

public:

	void setEntryHeight(double entry_height) { entryHeight = entry_height; }
	void setTotalHeight(double total_height) { totalHeight = total_height; }
	void setTotalWidth(double total_width) { totalWidth = total_width; }

	void setWorldManager(Object::WorldManager* world_manager) { messageQueue.setWorldManager(world_manager); }

	BonusMessages();

	void render();
};

}  // namespace HUD
}  // namespace Project

#endif
