#ifndef PROJECT_HUD__BONUS_MESSAGES_H
#define PROJECT_HUD__BONUS_MESSAGES_H

#include "bonus/BonusMessageQueue.h"
#include "render/TextureObjectChain.h"
#include "render/StringTexture.h"
#include "event/Enabler.h"
#include "event/BonusEvent.h"

namespace Project {
namespace HUD {

class BonusMessages
	: public Event::Enabler {
private:
	Bonus::BonusMessageQueue messageQueue;

	double entryHeight;
	double totalHeight;
	double totalWidth;
	int playerID;
	int totalBonuses;
	Render::TextureObjectChain entry;
	Render::StringTexture totalBonusesTexture;

protected:
	void bonusEventHandler(Event::BonusEvent* event);

public:

	void setEntryHeight(double entry_height) { entryHeight = entry_height; }
	void setTotalHeight(double total_height) { totalHeight = total_height; }
	void setTotalWidth(double total_width) { totalWidth = total_width; }
	void setPlayerID(int player_id) { playerID = player_id; }

	void setWorldManager(Object::WorldManager* world_manager) { messageQueue.setWorldManager(world_manager); }

	BonusMessages();

	void render();
};

}  // namespace HUD
}  // namespace Project

#endif
