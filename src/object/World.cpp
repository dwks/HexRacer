#include "World.h"

namespace Project {
namespace Object {

void World::preRender() {
    for(objectListType::iterator i = objectList.begin(); i != objectList.end();
        ++ i) {
        
        (*i)->preRender();
    }
}

void World::addObject(ObjectBase *object) {
    objectList.push_back(object);
}

}  // namespace Object
}  // namespace Project
