#ifndef PROJECT_WIDGET__MENU_MOVE_EVENT_H
#define PROJECT_WIDGET__MENU_MOVE_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class MenuMoveEvent : public WidgetEvent {
public:
	enum XDirection {XNONE, LEFT, RIGHT};
	enum YDirection {YNONE, DOWN, UP};

	XDirection xDir;
	YDirection yDir;

	MenuMoveEvent()
		: xDir(XNONE), yDir(YNONE) {}

	virtual void accept(EventVisitor &visitor) {}
    virtual event_type_t getType() const { return EVENT_MENU_MOVE; }
};

}  // namespace Widget
}  // namespace Project

#endif
