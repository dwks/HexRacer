#ifndef PROJECT_PAINT__PAINT_MANAGER_H
#define PROJECT_PAINT__PAINT_MANAGER_H

#include <vector>

#include "PaintCell.h"

#include "render/BaseRenderable.h"
#include "math/BSPTree3D.h"

#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/TypedObserver.h"

namespace Project {
namespace Paint {

class PaintManager
	: public Render::BaseRenderable {
private:
    class PaintEventHandler : public Event::TypedObserver<Event::PaintEvent> {
    private:
        PaintManager *paintManager;
    public:
        PaintEventHandler(PaintManager *paintManager)
            : paintManager(paintManager) {}
        
        virtual void observe(Event::PaintEvent *paintEvent);
    };
    
    class PaintCellsChangedHandler
        : public Event::TypedObserver<Event::PaintCellsChanged> {
    private:
        PaintManager *paintManager;
    public:
        PaintCellsChangedHandler(PaintManager *paintManager)
            : paintManager(paintManager) {}
        
        virtual void observe(Event::PaintCellsChanged *paintCellsChanged);
    };
private:

	std::vector<PaintCell*> paintList;
	Math::BSPTree3D* neutralPaintTree;
	Math::BSPTree3D* coloredPaintTree;
	int lastDrawnColor;

	static const int TREE_SPLIT_SIZE = 10;
	static const Math::BSPTree3D::SplitMethod TREE_SPLIT_METHOD = Math::BSPTree3D::MIN_OVERLAP;

	bool colorCell(PaintCell* cell, int new_color);

public:

	PaintManager();
	/** Set the paint cells that this class will manage
	*/
	void setPaintCells(const std::vector<PaintCell*>& paint_cells);

	void renderGeometry(Render::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object = NULL);

	/** Color paint cells by their numerical index
		@param cell_indices A vector with the indices of all paint cells to color
		@parma new_color The color which to color the paint cells
	*/
	void colorCellsByIndex(const std::vector<int> &cell_indices, int new_color);

	/** Color all neutral paint cells inside a sphere with center @a centroid and
		radius @a radius to the player color @a new_color
		Returns a vector of the indices of all paint cells that were colored
	*/
	std::vector<int> colorCellsInRadius(Math::Point centroid, double radius, int new_color);

	/** Returns a number representing the concentration of paint cells of color @a color
		inside the given radius. Increasing from zero depending on proximity and quantity.
		
		This number will be in the range [0.5, 2.0].
	*/
	double weightedCellsInRadius(Math::Point centroid, double radius, int color);
	
};

}  // namespace Paint
}  // namespace Project

#endif
