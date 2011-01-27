#include <algorithm>  // for std::find

#include "World.h"
#include "settings/ProgramSettings.h"

namespace Project {
namespace Object {

// the world always has an ID of 0
World::World() : AbstractObject(0) {
    if(Settings::ProgramSettings::getInstance()->isClient()) {
        renderable = new Render::RenderableWorld(objectList);
    }
    else {
        renderable = NULL;
    }
}

void World::preRender() {
    for(objectListType::iterator i = objectList.begin(); i != objectList.end();
        ++ i) {
        
        (*i)->preRender();
    }
}

void World::addObject(ObjectBase *object) {
    objectList.push_back(object);
}

void World::removeObject(ObjectBase *object) {
    objectListType::iterator found
        = std::find(objectList.begin(), objectList.end(), object);
    
    objectList.erase(found);
}

}  // namespace Object
}  // namespace Project
