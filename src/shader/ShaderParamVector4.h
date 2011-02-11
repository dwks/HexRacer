#ifndef PROJECT_RENDER__SHADER_UNIFORM_VECTOR4_H
#define PROJECT_RENDER__SHADER_UNIFORM_VECTOR4_H

#include "ShaderParameter.h"
#include "opengl/Color.h"

namespace Project {
namespace Shader {

class ShaderParamVector4
	: public ShaderParameter {
private:
	Project::OpenGL::Color color;
public:

	ShaderParamVector4(const char *_name, ShaderParamType _type, Project::OpenGL::Color _color);
	void setColor(OpenGL::Color _color) { color = _color; }
	void set(const ShaderParamSetter& setter);
};

}  // namespace Render
}  // namespace Project

#endif
