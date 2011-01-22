#include "PaintManager.h"
#include "sdl/PlayerManager.h"
#include "opengl/MathWrapper.h"
using namespace Project;
using namespace Math;
using namespace Render;
using namespace SDL;

namespace Project {
namespace Paint {

	PaintManager::PaintManager() {
		paintTree = NULL;
		getRenderProperties()->setWantsShaderName("paintShader");
	}

	void PaintManager::setPaintCells(const std::vector<PaintCell*>& paint_cells) {

		if (paintTree)
			paintTree->clear();

		if (paint_cells.size() == 0) {
			return;
		}

		BoundingBox3D paint_bound(*paint_cells[0]);
		for (unsigned i = 1; i < paint_cells.size(); i++) {
			paint_bound.expandToInclude(*paint_cells[i]);
		}

		if (!paintTree)
			paintTree = new BSPTree3D(paint_bound, BSPTree3D::FAIR_XSTART, 20);
		else {
			paintTree->clear();
			paintTree->resize(paint_bound);
		}

		for (unsigned i = 0; i < paint_cells.size(); i++) {
			paintTree->add(paint_cells[i]);
		}
	}

	void PaintManager::renderGeometry(ShaderParamSetter& setter, const Math::BoundingObject* bounding_object) {

		if (!paintTree)
			return;

		lastDrawnColor =-2;

		vector<ObjectSpatial*> visible_cells;

		if (bounding_object)
			visible_cells = paintTree->query(*bounding_object, SpatialContainer::NEARBY);
		else
			visible_cells = paintTree->all();

		for (unsigned int i = 0; i < visible_cells.size(); i++) {

			PaintCell* cell = (PaintCell*) visible_cells[i];
			if (cell->playerColor >= 0) {

				if (lastDrawnColor != cell->playerColor) {
					setter.setUniformVector4("playerColor", PlayerManager::getPlayerColor(cell->playerColor));
					lastDrawnColor = cell->playerColor;
				}
				glBegin(GL_TRIANGLE_FAN);
				OpenGL::MathWrapper::glNormal(cell->normal);
				OpenGL::MathWrapper::glVertex(cell->center);
				for (int j = 0; j < Paint::PaintCell::CELL_VERTICES; j++) {
					OpenGL::MathWrapper::glVertex(cell->vertex[j]);
				}
				OpenGL::MathWrapper::glVertex(cell->vertex[0]);
				glEnd();

			}

		}

	}

}  // namespace Paint
}  // namespace Project
