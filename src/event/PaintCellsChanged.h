#ifndef PROJECT_EVENT__PAINT_CELLS_CHANGED_H
#define PROJECT_EVENT__PAINT_CELLS_CHANGED_H

#include <vector>

#include "boost/serialization/vector.hpp"
#include "EventBase.h"

namespace Project {
namespace Event {

class PaintCellsChanged : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & colour;
        ar & cells;
    }
private:
    int colour;
    std::vector<int> cells;
public:
    PaintCellsChanged() : colour(-1) {}
    PaintCellsChanged(int colour, const std::vector<int> &cells)
        : colour(colour), cells(cells) {}
    
    int getColour() const { return colour; }
    const std::vector<int> &getCells() const { return cells; }
    
    virtual EventType::type_t getType() const
        { return EventType::PAINT_CELLS_CHANGED; }
};

}  // namespace Event
}  // namespace Project

#endif
