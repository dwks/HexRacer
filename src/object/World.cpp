#include "World.h"

namespace Project {
namespace Object {

void World::addObject(ObjectBase *object) {
    objectList.push_back(object);
}

}  // namespace Object
}  // namespace Project
