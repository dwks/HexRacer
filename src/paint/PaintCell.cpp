#include "PaintCell.h"
#include "math/Geometry.h"
#include "math/Values.h"
#include "opengl/MathWrapper.h"
#include <ostream>
#include <istream>
using namespace Project;
using namespace Math;

namespace Project {
namespace Paint {

	PaintCell::PaintCell(const Math::Point& _position) {
		playerColor =-1;
		displayList = 0;
		nextCell = NULL;
		position = _position;
	}

	PaintCell::~PaintCell() {
		if (displayList > 0)
			glDeleteLists(displayList, 1);
	}

}  // namespace Paint
}  // namespace Project
