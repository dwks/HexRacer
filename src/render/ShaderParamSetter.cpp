#include "ShaderParamSetter.h"
#include "RenderManager.h"
using namespace Project;
using namespace Math;
using namespace OpenGL;

namespace Project {
namespace Render {

	ShaderParamSetter::ShaderParamSetter() {
		this->manager = 0;
		hasTangentSpace = false;
	}

	ShaderParamSetter::ShaderParamSetter(RenderManager* manager) {
		this->manager = manager;
		hasTangentSpace = false;
	}

	void ShaderParamSetter::setUniformInt(const char *name, GLint value) {
		manager->setUniformInt(name, value);
	}
	void ShaderParamSetter::setUniformIntArray(const char *name, GLint values[], int num_values) {
		manager->setUniformIntArray(name, values, num_values);
	}
	void ShaderParamSetter::setUniformVector3(const char *name, Point point) {
		manager->setUniformVector3(name, point);
	}

	void ShaderParamSetter::setUniformVector4(const char *name, Color color) {
		manager->setUniformVector4(name, color);
	}

	void ShaderParamSetter::setUniformMatrix4(const char *name, GLboolean transpose, const GLfloat* matrix) {
		manager->setUniformMatrix4(name, transpose, matrix);
	}

	void ShaderParamSetter::setAttributeVector3(const char *name, Point point) {
		manager->setAttributeVector3(name, point);
	}

	void ShaderParamSetter::setAttributeVector4(const char *name, Color color) {
		manager->setAttributeVector4(name, color);
	}

	void ShaderParamSetter::setTangents(Math::Point tangent, Math::Point bitangent) {
		manager->setTangents(tangent, bitangent);
	}

}  // namespace Render
}  // namespace Project
