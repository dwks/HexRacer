#include "ShaderParamSetter.h"
using namespace Project;
using namespace Math;
using namespace OpenGL;

namespace Project {
namespace Shader {

	ShaderParamSetter::ShaderParamSetter() {
		shaderProgram = NULL;
	}

	void ShaderParamSetter::setShaderProgram(ShaderProgram* program, int* standard_uni_locs, int* standard_attr_locs) {
		shaderProgram = program;
		standardUniformLocations = standard_uni_locs;
		standardAttributeLocations = standard_attr_locs;
		
	}

	void ShaderParamSetter::setParamInt(ShaderParameter::ShaderParamType type, const char* name, GLint value) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1i(shaderProgram->getUniLoc(name), value); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib1s(shaderProgram->getAttrLoc(name), (GLshort) value); break;
		}
	}

	void ShaderParamSetter::setParamFloat(ShaderParameter::ShaderParamType type, const char* name, GLfloat value) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1f(shaderProgram->getUniLoc(name), value); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib1f(shaderProgram->getAttrLoc(name), value); break;
		}
	}
	void ShaderParamSetter::setParamIntArray(ShaderParameter::ShaderParamType type, const char *name, GLint values[], int num_values) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1iv(shaderProgram->getUniLoc(name), num_values, values); break;
			default: break;
		}
	}

	void ShaderParamSetter::setParamFloatArray(ShaderParameter::ShaderParamType type, const char *name, GLfloat values[], int num_values) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1fv(shaderProgram->getUniLoc(name), num_values, values); break;
			default: break;
		}
	}
	void ShaderParamSetter::setParamVector3(ShaderParameter::ShaderParamType type, const char *name, const Math::Point& point) const {
		if (!shaderProgram)
			return;

		GLfloat x = static_cast<GLfloat>(point.getX());
		GLfloat y = static_cast<GLfloat>(point.getY());
		GLfloat z = static_cast<GLfloat>(point.getZ());

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform3f(shaderProgram->getUniLoc(name), x, y, z); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib3f(shaderProgram->getAttrLoc(name), x, y, z); break;
		}
	}

	void ShaderParamSetter::setParamVector4(ShaderParameter::ShaderParamType type, const char *name, const OpenGL::Color& color) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform4f(shaderProgram->getUniLoc(name), color.redf(), color.greenf(), color.bluef(), color.alphaf()); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib4f(shaderProgram->getAttrLoc(name), color.redf(), color.greenf(), color.bluef(), color.alphaf()); break;
		}
	}
	void ShaderParamSetter::setParamMatrix4(ShaderParameter::ShaderParamType type, const char *name, GLboolean transpose, const GLfloat* matrix) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniformMatrix4fv(shaderProgram->getUniLoc(name), 1, transpose, matrix); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib4fv(shaderProgram->getAttrLoc(name), matrix); break;
		}
	}



	void ShaderParamSetter::setStandardParamInt(ShaderParameter::ShaderParamType type, int standard_type, GLint value) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1i(standardUniformLocations[standard_type], value); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib1s(standardAttributeLocations[standard_type], (GLshort) value); break;
		}
	}
	void ShaderParamSetter::setStandardParamFloat(ShaderParameter::ShaderParamType type, int standard_type, GLfloat value) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1f(standardUniformLocations[standard_type], value); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib1f(standardAttributeLocations[standard_type], value); break;
		}
	}
	void ShaderParamSetter::setStandardParamIntArray(ShaderParameter::ShaderParamType type, int standard_type, GLint values[], int num_values) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1iv(standardUniformLocations[standard_type], num_values, values); break;
			default: break;
		}

	}
	void ShaderParamSetter::setStandardParamFloatArray(ShaderParameter::ShaderParamType type, int standard_type, GLfloat values[], int num_values) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform1fv(standardUniformLocations[standard_type], num_values, values); break;
			default: break;
		}
	}
	void ShaderParamSetter::setStandardParamVector3(ShaderParameter::ShaderParamType type, int standard_type, const Math::Point& point) const {
		if (!shaderProgram)
			return;

		GLfloat x = static_cast<GLfloat>(point.getX());
		GLfloat y = static_cast<GLfloat>(point.getY());
		GLfloat z = static_cast<GLfloat>(point.getZ());

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform3f(standardUniformLocations[standard_type], x, y, z); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib3f(standardAttributeLocations[standard_type], x, y, z); break;
		}
	}
	void ShaderParamSetter::setStandardParamVector4(ShaderParameter::ShaderParamType type, int standard_type, const OpenGL::Color& color) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniform4f(standardUniformLocations[standard_type], color.redf(), color.greenf(), color.bluef(), color.alphaf()); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib4f(standardAttributeLocations[standard_type], color.redf(), color.greenf(), color.bluef(), color.alphaf()); break;
		}
	}
	void ShaderParamSetter::setStandardParamMatrix4(ShaderParameter::ShaderParamType type, int standard_type, GLboolean transpose, const GLfloat* matrix) const {
		if (!shaderProgram)
			return;

		switch (type) {
			case ShaderParameter::UNIFORM: glUniformMatrix4fv(standardUniformLocations[standard_type], 1, transpose, matrix); break;
			case ShaderParameter::ATTRIBUTE: glVertexAttrib4fv(standardAttributeLocations[standard_type], matrix); break;
		}
	}

}  // namespace Render
}  // namespace Project
