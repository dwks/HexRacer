#ifndef PROJECT_MAP__MAP_LOADER_H
#define PROJECT_MAP__MAP_LOADER_H

#include "HRMap.h"

namespace Project {
namespace Map {

class MapLoader {
public:
    /** Both @a map and @a mapRenderable must be constructed before calling
        this function.
        
        @param mapRenderable can be NULL if no rendering is required.
    */
    void load(HRMap *map, Render::RenderList *mapRenderable);
};

}  // namespace Map
}  // namespace Project

#endif
