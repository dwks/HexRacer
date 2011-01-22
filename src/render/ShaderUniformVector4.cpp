#include "ShaderUniformVector4.h"

namespace Project {
namespace Render {

	ShaderUniformVector4::ShaderUniformVector4(const char *_name, Project::OpenGL::Color _color) {
		name = _name;
		color = _color;
	}

	void ShaderUniformVector4::setShaderParameters(ShaderParamSetter& setter) {
		setter.setUniformVector4(name, color);
	}

}  // namespace Render
}  // namespace Project
