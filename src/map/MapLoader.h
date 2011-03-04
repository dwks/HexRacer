#ifndef PROJECT_MAP__MAP_LOADER_H
#define PROJECT_MAP__MAP_LOADER_H

#include "HRMap.h"
#include "opengl/Material.h"
#include "paint/PaintManager.h"
#include <vector>

namespace Project {
namespace Map {

class MapLoader {

private:
	std::vector<OpenGL::Material*> meshTints;
public:

    /** Both @a map and @a mapRenderable must be constructed before calling
        this function.
        
        @param mapRenderable can be NULL if no rendering is required.
    */
	void load(HRMap *map, Render::RenderList *mapRenderable = NULL, Paint::PaintManager* paintManager = NULL);

	/** Should be called whenever the map is unloaded
	*/
	void unload();
};

}  // namespace Map
}  // namespace Project

#endif
