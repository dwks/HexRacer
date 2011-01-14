#include "ShaderParamSetter.h"
#include "RenderManager.h"
using namespace Project;
using namespace Math;
using namespace OpenGL;

namespace Project {
namespace Render {

	ShaderParamSetter::ShaderParamSetter() {
		this->manager = 0;
	}

	ShaderParamSetter::ShaderParamSetter(RenderManager* manager) {
		this->manager = manager;
	}

	void ShaderParamSetter::setUniformVector3(const char *name, Point point) {
		manager->setUniformVector3(name, point);
	}

	void ShaderParamSetter::setUniformVector4(const char *name, Color color) {
		manager->setUniformVector4(name, color);
	}

	void ShaderParamSetter::setAttributeVector3(const char *name, Point point) {
		manager->setAttributeVector3(name, point);
	}

	void ShaderParamSetter::setAttributeVector4(const char *name, Color color) {
		manager->setAttributeVector4(name, color);
	}

}  // namespace Render
}  // namespace Project
