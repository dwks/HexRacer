#ifndef PROJECT_RENDER__RENDERABLE_OBJECT_H
#define PROJECT_RENDER__RENDERABLE_OBJECT_H

#include "math/Matrix.h"

namespace Project {
namespace Render {

class RenderableObject {
public:

	RenderableObject();
	virtual ~RenderableObject() {}
	
	virtual void renderGeometry() = 0;

	void render();

	bool hasTransformation() const;
	void clearTransformation();
	void setTransformation(const Math::Matrix &transform);
	Math::Matrix getTransformation() const;

private:

	Math::Matrix* transformation;

};

}  // namespace Render
}  // namespace Project

#endif
