#include "PaintManager.h"
#include "opengl/MathWrapper.h"
#include "math/BoundingSphere.h"
#include "render/ColorConstants.h"
#include "event/EventSystem.h"

using namespace Project;
using namespace Math;
using namespace Render;

namespace Project {
namespace Paint {

    void PaintManager::paintEventHandler(Event::PaintEvent *paintEvent) {
        Math::Point position = paintEvent->getPoint();
        double radius = paintEvent->getRadius();
        int colour = paintEvent->getColour();
        
        colorCellsInRadius(position, radius, colour);
    }
    
    void PaintManager::paintCellsChangedHandler(
        Event::PaintCellsChanged *paintCellsChanged) {
        
        colorCellsByIndex(
            paintCellsChanged->getCells(),
            paintCellsChanged->getColour());
    }
    
	PaintManager::PaintManager() {
		neutralPaintTree = new BSPTree3D(BoundingBox3D(), TREE_SPLIT_METHOD, TREE_SPLIT_SIZE);
		coloredPaintTree = new BSPTree3D(BoundingBox3D(), TREE_SPLIT_METHOD, TREE_SPLIT_SIZE);
		getRenderProperties()->setWantsShaderName("paintShader");
		OpenGL::Material* material = new OpenGL::Material("paintMaterial");
		material->setShininess(4.0f);
		getRenderProperties()->setMaterial(material);

		fadePlaneNear = 0.0f;
		fadePlaneFar = 0.0f;
		targetList = 0;
        
        METHOD_OBSERVER(&PaintManager::paintEventHandler);
        METHOD_OBSERVER(&PaintManager::paintCellsChangedHandler);
	}

	PaintManager::~PaintManager() {
        delete(neutralPaintTree);
		delete(coloredPaintTree);
		delete(getRenderProperties()->getMaterial());
    }

	void PaintManager::setPaintCells(const std::vector<PaintCell*>& paint_cells) {

		neutralPaintTree->clear();
		coloredPaintTree->clear();
		paintList.clear();

		if (paint_cells.size() == 0)
			return;

		vector<ObjectSpatial*> temp_list;

		BoundingBox3D paint_bound;
		for (unsigned i = 0; i < paint_cells.size(); i++) {

			paint_cells[i]->setDisplayList();

			if (i == 0)
				paint_bound.setToObject(*paint_cells[i]);
			else
				paint_bound.expandToInclude(*paint_cells[i]);

			temp_list.push_back(paint_cells[i]);
			paint_cells[i]->index = i;
			paint_cells[i]->optimize();
		}

		neutralPaintTree->resize(paint_bound);
		coloredPaintTree->resize(paint_bound);
		
		neutralPaintTree->add(temp_list);
		paintList = vector<PaintCell*>(paint_cells);
	}

	void PaintManager::renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object, const Render::RenderSettings& settings) {

		renderMinimap = false;

		GLfloat fade_planes [2] = {fadePlaneNear, fadePlaneFar};
		setter.setParamFloatArray(
			Shader::ShaderParameter::UNIFORM,
			"fadePlanes",
			fade_planes,
			2);

		if (!settings.getRedrawMode()) {
			redrawBuffer.clear();
			if (bounding_object)
				coloredPaintTree->operateQuery(*this, *bounding_object, SpatialContainer::NEARBY);
			else
				coloredPaintTree->operateAll(*this);

		}
		else {
			for (unsigned int i = 0; i < redrawBuffer.size(); i++) {
				renderCell(redrawBuffer[i]);
			}
		}

	}

	void PaintManager::minimapRender(const Math::BoundingObject& bounding_object, float view_height, float alpha) {

		renderMinimap = true;
		renderAlpha = alpha;

		GLfloat values [4];
		glGetFloatv(GL_VIEWPORT, values);
		float viewportWidth = values[3];
		glPointSize(viewportWidth/view_height);
		glBegin(GL_POINTS);

		coloredPaintTree->operateQuery(*this, bounding_object, SpatialContainer::NEARBY);

		glEnd();
		glPointSize(1.0f);
		

	}
	void PaintManager::colorCellsByIndex(const vector<int> &cell_indices, int new_color, bool force_color) {
		for (unsigned int i = 0; i < cell_indices.size(); i++) {
			colorCell(paintList[cell_indices[i]], new_color, force_color);
		}
	}

	void PaintManager::colorCellByIndex(int cell_index, int new_color, bool force_color) {
		colorCell(paintList[cell_index], new_color, force_color);
	}

	vector<int> PaintManager::colorCellsInRadius(Point centroid, double radius, int new_color, bool force_color) {

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
				if (colorCell(cell, new_color, force_color)) {
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

	bool PaintManager::colorCell(PaintCell* cell, int new_color, bool force_color) {

		if (new_color >= 0) {
			
			if (cell->playerColor < 0) {

				cell->playerColor = new_color;
				neutralPaintTree->remove(cell);
				coloredPaintTree->add(cell);
				return true;

			}
			else if (force_color) {
				cell->playerColor = new_color;
				return true;
			}

		}

		if (new_color < 0) {
			
			if (cell->playerColor >= 0) {

				cell->playerColor = new_color;
				coloredPaintTree->remove(cell);
				neutralPaintTree->add(cell);
				return true;

			}
			else if (force_color) {
				cell->playerColor = new_color;
				return true;
			}

		}

		return false;
	}

	void PaintManager::operateOnObject(Math::ObjectSpatial* object) {
		if (renderMinimap)
			renderCellMinimap((PaintCell*)object);
		else {
			renderCell((PaintCell*)object);
			redrawBuffer.push_back((PaintCell*)object);
		}
	}

	void PaintManager::renderCell(PaintCell* cell) {

		OpenGL::Color cell_color;

		if (cell->playerColor >= 0)
			cell_color = ColorConstants::playerColor(cell->playerColor);
		else {
			cell_color = OpenGL::Color::WHITE;
			cell_color.setAlphaf(0.4f);
		}

		OpenGL::Color::glColor(cell_color);
		OpenGL::MathWrapper::glVertex(cell->center);
		cell_color *= 0.85f;
		GLfloat values [4] = {cell_color.redf(), cell_color.greenf(), cell_color.bluef(), cell_color.alphaf()};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, values);
		cell_color = cell_color*0.5f+OpenGL::Color(OpenGL::Color::DARKGREY)*0.25f;
		values[0] = cell_color[0];
		values[1] = cell_color[1];
		values[2] = cell_color[2];
		values[3] = cell_color[3];
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, values);
		glCallList(cell->displayList);
	}

	void PaintManager::renderCellMinimap(PaintCell* cell) {
		OpenGL::Color::glColor(ColorConstants::playerColor(cell->playerColor), renderAlpha);
		OpenGL::MathWrapper::glVertex(cell->center);
	}

	void PaintManager::renderEraseEffect(Math::Point centroid, double radius) {

		BoundingSphere query_sphere(centroid, radius);
		vector<int> colored_indices;
		vector<ObjectSpatial*> candidate_cells;

		neutralPaintTree->appendQuery(candidate_cells, query_sphere, SpatialContainer::NEARBY);

		for (unsigned int i = 0; i < candidate_cells.size(); i++) {
			PaintCell* cell = (PaintCell*) candidate_cells[i];
			if (cell->center.distanceSquared(centroid) <= query_sphere.getRadiusSquared()) {
				renderCell(cell);
			}
		}

	}

}  // namespace Paint
}  // namespace Project
