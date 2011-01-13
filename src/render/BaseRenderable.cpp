#include "BaseRenderable.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Render {

	BaseRenderable::BaseRenderable() {
		hasTransformation = false;
	}

	bool BaseRenderable::hasTransformation() const {
		return hasTranformation = false;;
	}

	void BaseRenderable::clearTransformation() {
		hasTranformation = false;
	}

	void BaseRenderable::setTransformation(Matrix transform) {
		transformation = transform;
		hasTranformation = true;
	}

	Matrix BaseRenderable::getTransformation() const {
		return transformation;
	}

}  // namespace Render
}  // namespace Project
