#ifndef PROJECT_EVENT__TOGGLE_PAINTING_H
#define PROJECT_EVENT__TOGGLE_PAINTING_H

#include "EventBase.h"

namespace Project {
namespace Event {

class TogglePainting : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & id;
        ar & type;
        ar & propagate;
    }
public:
    enum PaintType {
        ERASING,
        NOTHING,
        PAINTING
    };
private:
    int id;
    PaintType type;
    bool propagate;
public:
    TogglePainting() : id(-1), propagate(true) {}
    TogglePainting(int id, PaintType type)
        : id(id), type(type), propagate(true) {}
    
    int getID() const { return id; }
    PaintType getPaintType() const { return type; }
    
    void setPropagate(bool p) { propagate = p; }
    bool getPropagate() const { return propagate; }
    
    virtual EventType::type_t getType() const
        { return EventType::TOGGLE_PAINT; }
};

}  // namespace Event
}  // namespace Project

#endif
