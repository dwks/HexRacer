#ifndef PROJECT_EVENT__PAINT_EVENT_H
#define PROJECT_EVENT__PAINT_EVENT_H

#include "EventBase.h"

#include "math/Point.h"

namespace Project {
namespace Event {

class PaintEvent : public EventBase {
private:
    Math::Point point;
    double radius;
    int colour;
public:
    PaintEvent() : colour(-1) {}
    PaintEvent(Math::Point point, double radius, int colour)
        : point(point), radius(radius), colour(colour) {}
    
    Math::Point getPoint() const { return point; }
    double getRadius() const { return radius; }
    int getColour() const { return colour; }
    
    virtual EventType::type_t getType() const
        { return EventType::PAINT_EVENT; }
};

}  // namespace Event
}  // namespace Project

#endif
