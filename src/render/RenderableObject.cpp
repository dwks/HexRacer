#include "RenderableObject.h"
#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"
#include "math/Matrix.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	RenderableObject::RenderableObject() {
		transformation = NULL;
	}

	void RenderableObject::render() {

		if (hasTransformation()) {
			glPushMatrix();
			MathWrapper::glMultMatrix(getTransformation());
		}

		renderGeometry();

		if (hasTransformation()) {
			glPopMatrix();
		}
			
	}

	bool RenderableObject::hasTransformation() const {
		return (transformation != NULL);
	}

	void RenderableObject::clearTransformation() {
		if (hasTransformation()) {
			delete(transformation);
			transformation = NULL;
		}
	}

	void RenderableObject::setTransformation(const Matrix &transform) {
		transformation = new Matrix(transform);
	}

	Matrix RenderableObject::getTransformation() const {
		if (hasTransformation()) {
			return (*transformation);
		}
		else {
			return Matrix();
		}
	}

}  // namespace Render
}  // namespace Project
