#pragma once

#include <GL/glew.h>
#include <iostream>
using namespace std;

namespace Project {
namespace Shader {

/** A GLSL Shader Program
*/
class ShaderProgram {
public:
	ShaderProgram(const GLchar *fs, const GLchar *vs);

	void turnShaderOn() const;
	void turnShaderOff() const;
	int getUniLoc(const char *name);
	int getAttrLoc(const char *name);

	GLuint v, f, p;	//Handles for our vertex shader, fragment shader and the program object.

private:

	char *textFileRead(const char *fn);	//Methods that read and write shader files.
	int textFileWrite(const char *fn, const char *s);
};

}  // namespace Render
}  // namespace Project
