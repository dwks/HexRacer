#ifndef PROJECT_RENDER__SHADER_UNIFORM_VECTOR4_H
#define PROJECT_RENDER__SHADER_UNIFORM_VECTOR4_H

#include "ShaderParameter.h"
#include "opengl/Color.h"

namespace Project {
namespace Render {

class ShaderUniformVector4
	: public ShaderParameter {
private:
	Project::OpenGL::Color color;
public:

	ShaderUniformVector4(const char *_name, Project::OpenGL::Color _color);
	void setColor(OpenGL::Color _color) { color = _color; }
	void setShaderParameters(ShaderParamSetter& setter);
};

}  // namespace Render
}  // namespace Project

#endif
