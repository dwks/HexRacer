#ifndef PROJECT_BONUS__BONUS_MESSAGE_QUEUE_H
#define PROJECT_BONUS__BONUS_MESSAGE_QUEUE_H

#include "render/StringTexture.h"
#include <list>

#include "event/Enabler.h"
#include "event/BonusEvent.h"
#include "object/WorldManager.h"

#include "boost/smart_ptr/shared_ptr.hpp"

namespace Project {
namespace Bonus {

class BonusMessageQueue
	: public Event::Enabler {
public:
	struct BonusMessage {
		boost::shared_ptr<Render::StringTexture> texture;
		unsigned long appearTime;
	};
private:
	Object::WorldManager* worldManager;
	std::list<BonusMessage> messageQueue;
protected:
	void bonusEventHandler(Event::BonusEvent* event);
public:
	BonusMessageQueue();
	void setWorldManager(Object::WorldManager* world_manager) { worldManager = world_manager; }
	const std::list<BonusMessage>& getQueue();
};

}  // namespace Bonus
}  // namespace Project

#endif
