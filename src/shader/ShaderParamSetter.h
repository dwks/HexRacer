#ifndef PROJECT_RENDER__SHADER_PARAM_SETTER_H
#define PROJECT_RENDER__SHADER_PARAM_SETTER_H

#include "math/Point.h"
#include "opengl/Color.h"
#include "ShaderProgram.h"
#include "ShaderParameter.h"

namespace Project {
namespace Shader {

/** An accessor for setting shader parameters
*/
class ShaderParamSetter {

private:

	ShaderProgram* shaderProgram;
	int shaderIndex;
	int* standardUniformLocations;
	int* standardAttributeLocations;
	bool hasNormalMap;

public:

	ShaderParamSetter();

	void setShaderProgram(ShaderProgram* program, int shader_index, int* standard_uni_locs, int* standard_attr_locs);

	void setParamInt(ShaderParameter::ShaderParamType type, const char* name, GLint value) const;
	void setParamFloat(ShaderParameter::ShaderParamType type, const char* name, GLfloat value) const;
	void setParamIntArray(ShaderParameter::ShaderParamType type, const char *name, GLint values[], int num_values) const;
	void setParamFloatArray(ShaderParameter::ShaderParamType type, const char *name, GLfloat values[], int num_values) const;
	void setParamVector3(ShaderParameter::ShaderParamType type, const char *name, const Math::Point& point) const;
	void setParamVector4(ShaderParameter::ShaderParamType type, const char *name, const OpenGL::Color& color) const;
	void setParamMatrix4(ShaderParameter::ShaderParamType type, const char *name, GLboolean transpose, const GLfloat* matrix) const;

	void setStandardParamInt(ShaderParameter::ShaderParamType type, int standard_type, GLint value) const;
	void setStandardParamFloat(ShaderParameter::ShaderParamType type, int standard_type, GLfloat value) const;
	void setStandardParamIntArray(ShaderParameter::ShaderParamType type, int standard_type, GLint values[], int num_values) const;
	void setStandardParamFloatArray(ShaderParameter::ShaderParamType type, int standard_type, GLfloat values[], int num_values) const;
	void setStandardParamVector3(ShaderParameter::ShaderParamType type, int standard_type, const Math::Point& point) const;
	void setStandardParamVector4(ShaderParameter::ShaderParamType type, int standard_type, const OpenGL::Color& color) const;
	void setStandardParamMatrix4(ShaderParameter::ShaderParamType type, int standard_type, GLboolean transpose, const GLfloat* matrix) const;

	void setHasNormalMap(bool has) { hasNormalMap = has; }
	bool getHasTangentSpace() const;
	bool shaderIndexActive(int index) const;

};

}  // namespace Render
}  // namespace Project

#endif
