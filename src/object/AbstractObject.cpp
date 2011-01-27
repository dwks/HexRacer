#include "AbstractObject.h"

namespace Project {
namespace Object {

void AbstractObject::preRender() {
    if(getRenderableObject()) {
        getRenderableObject()->preRenderUpdate(
            getPhysicalObject()->getTransformation());
    }
}

}  // namespace Object
}  // namespace Project
