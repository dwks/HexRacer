#ifndef PROJECT_EVENT__BONUS_EVENT_H
#define PROJECT_EVENT__BONUS_EVENT_H

#include "EventBase.h"
#include <string>

namespace Project {
namespace Event {

class BonusEvent
	: public EventBase {
private:
	int playerID;
	int points;
	std::string bonusName;

	friend class boost::serialization::access;
    
    template <typename Archive>
	void serialize(Archive &ar, const unsigned version) {
		ar & boost::serialization::base_object<EventBase>(*this);
		ar & playerID;
		ar & points;
		ar & bonusName;
	}

public:

	BonusEvent() {}
	BonusEvent(int player_id, int _points, const std::string& bonus_name)
		: playerID(player_id), points(_points), bonusName(bonus_name) {}

	int getPlayerID() const { return playerID; }
	int getPoints() const { return points; }
	const std::string& getName() const { return bonusName; }

	virtual EventType::type_t getType() const
	{ return EventType::PLAYER_BONUS; }
};

}  // namespace Event
}  // namespace Project

#endif
