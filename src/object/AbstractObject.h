#ifndef PROJECT_OBJECT__ABSTRACT_OBJECT_H
#define PROJECT_OBJECT__ABSTRACT_OBJECT_H

#include "ObjectBase.h"

namespace Project {
namespace Object {

/** Abstract base class for objects that supplies the ID functionality.
*/
class AbstractObject : public ObjectBase {
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
