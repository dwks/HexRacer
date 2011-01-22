#ifndef PROJECT_PAINT__PAINT_MANAGER_H
#define PROJECT_PAINT__PAINT_MANAGER_H

#include "PaintCell.h"
#include "render/BaseRenderable.h"
#include "math/BSPTree3D.h"
#include <vector>

namespace Project {
namespace Paint {

class PaintManager
	: public Render::BaseRenderable {
private:

	Math::BSPTree3D* paintTree;
	int lastDrawnColor;

public:

	PaintManager();
	void setPaintCells(const std::vector<PaintCell*>& paint_cells);
	void renderGeometry(Render::ShaderParamSetter& setter, const  Math::BoundingObject* bounding_object = NULL);
	
};

}  // namespace Paint
}  // namespace Project

#endif
