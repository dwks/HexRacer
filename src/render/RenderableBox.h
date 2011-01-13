#ifndef PROJECT_RENDER__RENDERABLE_BOX_H
#define PROJECT_RENDER__RENDERABLE_BOX_H

#include "RenderableObject.h"
#include "math/BoundingBox3D.h"

namespace Project {
namespace Render {

class RenderableBox
	: public RenderableObject {
private:
	Math::BoundingBox3D box;
public:

	RenderableBox(Math::BoundingBox3D box);

	void renderGeometry();
};

}  // namespace Render
}  // namespace Project

#endif
