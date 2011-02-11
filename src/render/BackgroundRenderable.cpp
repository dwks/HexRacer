#include "BackgroundRenderable.h"
#include "opengl/MathWrapper.h"

namespace Project {
namespace Render {

	void BackgroundRenderable::renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object) {

		double z = camera->getFarPlane()*0.99;

		glBegin(GL_TRIANGLES);
		OpenGL::MathWrapper::glVertex( camera->cameraToWorld(-1.0, -4.0, z) );
		OpenGL::MathWrapper::glVertex( camera->cameraToWorld(-1.0, 4.0, z) );
		OpenGL::MathWrapper::glVertex( camera->cameraToWorld(4.0, 4.0, z) );
		glEnd();
	}

}  // namespace Render
}  // namespace Project
