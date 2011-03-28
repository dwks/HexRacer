#ifndef PROJECT_WIDGET__MENU_SELECTED_EVENT_H
#define PROJECT_WIDGET__MENU_SELECTED_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class MenuSelectedEvent : public WidgetEvent {
public:
	enum SelectedEventType {CONFIRM, BACK};

	SelectedEventType type;

	MenuSelectedEvent(SelectedEventType _type = CONFIRM)
		: type(_type) {}

	virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_MENU_SELECTED; }
};

}  // namespace Widget
}  // namespace Project

#endif
