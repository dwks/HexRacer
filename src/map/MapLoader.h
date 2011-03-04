#ifndef PROJECT_MAP__MAP_LOADER_H
#define PROJECT_MAP__MAP_LOADER_H

#include "HRMap.h"
#include "opengl/Material.h"
#include "paint/PaintManager.h"
#include "misc/ProgressTracker.h"
#include <vector>

namespace Project {
namespace Map {

class MapLoader
	: public Misc::ProgressTracker {

private:
	std::vector<OpenGL::Material*> meshTints;
	Misc::ProgressTracker mapLoadTracker;
	int totalSteps;
	int currentStep;
public:

    /** Both @a map and @a mapRenderable must be constructed before calling
        this function.
        
        @param mapRenderable can be NULL if no rendering is required.
    */
	void load(HRMap *map, Misc::ProgressTracker* progressTracker = NULL, Render::RenderList *mapRenderable = NULL, Paint::PaintManager* paintManager = NULL);

	/** Should be called whenever the map is unloaded
	*/
	void unload();

	//void setTotalSteps(int total_steps) { totalSteps = total_steps; }
	//void setCurrentStep(int current_step);
};

}  // namespace Map
}  // namespace Project

#endif
