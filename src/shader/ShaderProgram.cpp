#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

#include "ShaderProgram.h"
#include "log/Logger.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace Shader {

ShaderProgram::ShaderProgram(const GLchar *fs, const GLchar *vs) {
    // Build the shader program
    
	char *vsf = NULL;
	char *fsf = NULL;
	vsf = textFileRead(vs);
	fsf = textFileRead(fs);
	const char *ff = fsf;
	const char *vv = vsf;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);
	glCompileShader(v);
	glCompileShader(f);
	p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);
	glUseProgram(p);

	free(vsf);
	free(fsf);

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	if (GET_SETTING("render.shaderoutput", false)) {
		glGetProgramiv(p, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
		{

			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(p, infologLength, &charsWritten, infoLog);
			//printf("%s\n",infoLog);
			//if(*infoLog != 0) {
				LOG(OPENGL, infoLog);
			//}
			free(infoLog);
		}
	}

	glUseProgram(NULL);
}

ShaderProgram::~ShaderProgram() {

	glDetachShader(p, f);
	glDetachShader(p, v);
	glDeleteProgram(p);
	glDeleteShader(f);
	glDeleteShader(v);

}
void ShaderProgram::turnShaderOn() const{
	glUseProgram(p);
}

void ShaderProgram::turnShaderOff() const{
	glUseProgram(NULL);
}

char *ShaderProgram::textFileRead(const char *fn) {
	FILE *fp;
	char *content = NULL;

	int count = 0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int ShaderProgram::textFileWrite(const char *fn, const char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {

			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

/*
* Function: getUniLoc
* Purpose: gets the location of a uniform variable in a shader program
* Paramaters: program -> shader program
name -> the name of the variable to get
return -> the location of the variable

*/
int ShaderProgram::getUniLoc(const char *name) {
	int loc;
	loc = glGetUniformLocation(p, name);
	return loc;
}

/*
* Function: getUniLoc
* Purpose: gets the location of an attribute variable in a shader program
* Paramaters: program -> shader program
name -> the name of the variable to get
return -> the location of the variable
if location = -1 there was an error

*/
int ShaderProgram::getAttrLoc(const char *name) {
	int loc;
	loc = glGetAttribLocation(p, name);

	/*
	if (loc == -1)
		printf("No such uniform named \"%s\"\n", name);
	*/

	return loc;
}

}  // namespace Render
}  // namespace Project
