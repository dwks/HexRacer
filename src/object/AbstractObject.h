#ifndef PROJECT_OBJECT__ABSTRACT_OBJECT_H
#define PROJECT_OBJECT__ABSTRACT_OBJECT_H

#include "ObjectBase.h"

#include "boost/serialization/access.hpp"
#include "boost/serialization/base_object.hpp"

namespace Project {
namespace Object {

/** Abstract base class for objects that supplies the ID functionality.
*/
class AbstractObject : public ObjectBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & id;
    }
private:
    int id;
public:
    AbstractObject(int id) : id(id) {}
    
    virtual int getID() const { return id; }
    
    virtual Physics::PhysicalObject *getPhysicalObject() = 0;
    virtual Render::RenderableObject *getRenderableObject() = 0;
};

}  // namespace Object
}  // namespace Project

#endif
