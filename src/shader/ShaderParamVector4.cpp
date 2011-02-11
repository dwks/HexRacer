#include "ShaderParamVector4.h"
#include "ShaderParamSetter.h"

namespace Project {
namespace Shader {

	ShaderParamVector4::ShaderParamVector4(const char *_name, ShaderParameter::ShaderParamType _type, Project::OpenGL::Color _color)
		: ShaderParameter(_name, _type) {
		color = _color;
	}

	void ShaderParamVector4::set(const ShaderParamSetter& setter) {
		setter.setParamVector4(type, name, color);
	}

}  // namespace Render
}  // namespace Project
