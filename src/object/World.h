#ifndef PROJECT_OBJECT__WORLD_H
#define PROJECT_OBJECT__WORLD_H

#include <vector>
#include "AbstractObject.h"

#include "boost/serialization/vector.hpp"

namespace Project {
namespace Object {

class World : public AbstractObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<AbstractObject>(*this);
        ar & objectList;
    }
private:
    typedef std::vector<ObjectBase *> objectListType;
    objectListType objectList;
public:
    // the world always has an ID of 0
    World() : AbstractObject(0) {}
    
    void addObject(ObjectBase *object);
    
    virtual void preRender();
    
    virtual Physics::PhysicalObject *getPhysicalObject()
        { return NULL; }
    virtual Render::RenderableObject *getRenderableObject()
        { return NULL; }
};

}  // namespace Object
}  // namespace Project

#endif
