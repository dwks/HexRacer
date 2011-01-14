//#if(0)

#pragma once

#include <GL/glew.h>
#include <iostream>
using namespace std;

namespace Project {
namespace Render {

class Shader {
public:
	Shader(GLchar *fs, GLchar *vs);

	void turnShaderOn() const;
	void turnShaderOff() const;
	int getUniLoc(const char *name);
	int getAttrLoc(const char *name);

	GLuint v, f, p;	//Handles for our vertex shader, fragment shader and the program object.

private:

	char *textFileRead(const char *fn);	//Methods that read and write shader files.
	int textFileWrite(char *fn, char *s);
};

}  // namespace Render
}  // namespace Project

//#endif