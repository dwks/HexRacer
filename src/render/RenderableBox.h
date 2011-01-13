#ifndef PROJECT_RENDER__RENDERABLE_BOX_H
#define PROJECT_RENDER__RENDERABLE_BOX_H

#include "BaseRenderable.h"
#include "math/BoundingBox3D.h"

namespace Project {
namespace Render {

class RenderableBox
	: public BaseRenderable {
private:
	Math::BoundingBox3D box;
public:

	RenderableBox(Math::BoundingBox3D box);

	void render();
};

}  // namespace Render
}  // namespace Project

#endif
