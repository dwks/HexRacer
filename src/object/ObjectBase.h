#ifndef PROJECT_OBJECT__OBJECT_BASE_H
#define PROJECT_OBJECT__OBJECT_BASE_H

#include "physics/PhysicalObject.h"
#include "render/RenderableObject.h"

namespace Project {
namespace Object {

/** Abstract interface specification for all objects in the game.
*/
class ObjectBase {
public:
    virtual ~ObjectBase() {}
    
    /** Returns a unique identifier for this object.
    */
    virtual int getID() const = 0;
    
    /** Returns the physical component of this object.
    */
    virtual Physics::PhysicalObject *getPhysicalObject() = 0;
    
    /** Returns the renderable component of this object.
    */
    virtual Render::RenderableObject *getRenderableObject() = 0;
};

}  // namespace Object
}  // namespace Project

#endif
