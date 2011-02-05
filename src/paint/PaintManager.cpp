#include "PaintManager.h"
#include "opengl/MathWrapper.h"
#include "math/BoundingSphere.h"
#include "render/ColorConstants.h"
#include "event/ObserverList.h"
#include "settings/SettingsManager.h"
using namespace Project;
using namespace Math;
using namespace Render;

namespace Project {
namespace Paint {

    void PaintManager::PaintEventHandler::observe(Event::PaintEvent *paintEvent) {
        Math::Point position = paintEvent->getPoint();
        double radius = paintEvent->getRadius();
        int colour = paintEvent->getColour();
        
        paintManager->colorCellsInRadius(position, radius, colour);
    }
    
    void PaintManager::PaintCellsChangedHandler::observe(
        Event::PaintCellsChanged *paintCellsChanged) {
        
        paintManager->colorCellsByIndex(
            paintCellsChanged->getCells(),
            paintCellsChanged->getColour());
    }
    
	PaintManager::PaintManager() {
		neutralPaintTree = new BSPTree3D(BoundingBox3D(), TREE_SPLIT_METHOD, TREE_SPLIT_SIZE);
		coloredPaintTree = new BSPTree3D(BoundingBox3D(), TREE_SPLIT_METHOD, TREE_SPLIT_SIZE);
		getRenderProperties()->setWantsShaderName("paintShader");
        
        ADD_OBSERVER(new PaintEventHandler(this));
        ADD_OBSERVER(new PaintCellsChangedHandler(this));
	}

	PaintManager::~PaintManager() {
        delete(neutralPaintTree);
		delete(coloredPaintTree);
    }

	void PaintManager::setPaintCells(const std::vector<PaintCell*>& paint_cells) {

		neutralPaintTree->clear();
		coloredPaintTree->clear();
		paintList.clear();

		if (paint_cells.size() == 0)
			return;

		vector<ObjectSpatial*> temp_list;

		temp_list.push_back(paint_cells[0]);
		BoundingBox3D paint_bound(*paint_cells[0]);
		paint_cells[0]->index = 0;
		for (unsigned i = 1; i < paint_cells.size(); i++) {
			temp_list.push_back(paint_cells[i]);
			paint_bound.expandToInclude(*paint_cells[i]);
			paint_cells[i]->index = i;
		}

		neutralPaintTree->resize(paint_bound);
		coloredPaintTree->resize(paint_bound);
		
		neutralPaintTree->add(temp_list);
		paintList = vector<PaintCell*>(paint_cells);
	}

	void PaintManager::renderGeometry(ShaderParamSetter& setter, const Math::BoundingObject* bounding_object) {

		lastDrawnColor =-2;

		vector<ObjectSpatial*> visible_cells;

		if (bounding_object)
			coloredPaintTree->appendQuery(visible_cells, *bounding_object, SpatialContainer::NEARBY);
		else
			coloredPaintTree->appendAll(visible_cells);

		for (unsigned int i = 0; i < visible_cells.size(); i++) {

			PaintCell* cell = (PaintCell*) visible_cells[i];

			OpenGL::Color::glColor(ColorConstants::playerColor(cell->playerColor));

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

	void PaintManager::minimapRender(const Math::BoundingBox2D& bounding_box, float alpha) {

		vector<ObjectSpatial*> visible_cells;

		coloredPaintTree->appendQuery(visible_cells, bounding_box, SpatialContainer::NEARBY);

		GLfloat values [4];
		glGetFloatv(GL_VIEWPORT, values);
		float viewportWidth = values[3];
		glPointSize(viewportWidth/GET_SETTING("render.minimap.height", 60.0));
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < visible_cells.size(); i++) {

			PaintCell* cell = (PaintCell*) visible_cells[i];
			OpenGL::Color::glColor(ColorConstants::playerColor(cell->playerColor), alpha);
			OpenGL::MathWrapper::glVertex(cell->center);
			
		}
		glEnd();
		glPointSize(1.0f);

	}
	void PaintManager::colorCellsByIndex(const vector<int> &cell_indices, int new_color) {
		for (unsigned int i = 0; i < cell_indices.size(); i++) {
			colorCell(paintList[cell_indices[i]], new_color);
		}
	}

	vector<int> PaintManager::colorCellsInRadius(Point centroid, double radius, int new_color) {

		BoundingSphere query_sphere(centroid, radius);
		vector<int> colored_indices;
		vector<ObjectSpatial*> candidate_cells;

		if (new_color >= 0)
			neutralPaintTree->appendQuery(candidate_cells, query_sphere, SpatialContainer::NEARBY);
		else
			coloredPaintTree->appendQuery(candidate_cells, query_sphere, SpatialContainer::NEARBY);

		for (unsigned int i = 0; i < candidate_cells.size(); i++) {

			PaintCell* cell = (PaintCell*) candidate_cells[i];
			if (cell->center.distanceSquared(centroid) <= query_sphere.getRadiusSquared()) {
				if (colorCell(cell, new_color)) {
					colored_indices.push_back(cell->index);
				}
			}

		}

		return colored_indices;
	}

	double PaintManager::weightedCellsInRadius(Point centroid, double radius, int color) {
		double weighted_score = 0.0;
        int cell_count = 0;
        
		BoundingSphere query_sphere(centroid, radius);
		vector<ObjectSpatial*> candidate_cells;
		coloredPaintTree->appendQuery(candidate_cells, query_sphere, SpatialContainer::NEARBY);
        
		for (unsigned int i = 0; i < candidate_cells.size(); i++) {
			PaintCell *cell = static_cast<PaintCell *>(candidate_cells[i]);
			double dist = cell->center.distance(centroid);
            
			if(dist <= query_sphere.getRadius()) {
                cell_count ++;
                double colour_factor;
                
                if(cell->playerColor == color) {
                    colour_factor = 1.0;
                }
                else if(cell->playerColor < 0) {
                    colour_factor = 0.0;
                    continue;
                }
                else {
                    colour_factor = -0.5;
                }
                
                double distance_weight = (query_sphere.getRadius() - dist)
                    / query_sphere.getRadius();
                
                weighted_score += colour_factor * distance_weight;
			}
		}
        
        if(!cell_count) {
            return 1.0;
        }
        else {
            return 1.0 + weighted_score / cell_count;
        }
	}

	bool PaintManager::colorCell(PaintCell* cell, int new_color) {

		if (new_color >= 0 && cell->playerColor < 0) {

			cell->playerColor = new_color;
			neutralPaintTree->remove(cell);
			coloredPaintTree->add(cell);
			return true;

		}

		if (new_color < 0 && cell->playerColor >= 0) {

			cell->playerColor = new_color;
			coloredPaintTree->remove(cell);
			neutralPaintTree->add(cell);
			return true;

		}

		return false;
	}

}  // namespace Paint
}  // namespace Project
