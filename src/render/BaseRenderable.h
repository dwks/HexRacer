#ifndef PROJECT_RENDER__BASE_RENDERABLE_H
#define PROJECT_RENDER__BASE_RENDERABLE_H

#include "math/Matrix.h"

namespace Project {
namespace Render {

class BaseRenderable {
public:

	BaseRenderable();

	virtual void render() = 0;

private:

	bool hasTransformation;
	Project::Math::Matrix transform;

};

}  // namespace Render
}  // namespace Project

#endif
