#ifndef PROJECT_RENDER__RENDER_STATE_MANAGER_H
#define PROJECT_RENDER__RENDER_STATE_MANAGER_H

#include "ShaderParamSetter.h"
#include "Shader.h"
#include "RenderProperties.h"
#include "ShaderAttributeLocation.h"
#include "opengl/OpenGL.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

#define SHADER_COLORMAP_UNIFORM_NAME "colorMap"
#define SHADER_NORMALMAP_UNIFORM_NAME "normalMap"
#define SHADER_HASTEXTURE_UNIFORM_NAME "hasTexture"

namespace Project {
namespace Render {

class RenderableObject;

class RenderManager {
private:
	static const GLenum colorMapTexture = GL_TEXTURE0;
	static const int colorMapTextureNum = 0;
	static const GLenum normalMapTexture = GL_TEXTURE1;
	static const int normalMapTextureNum = 1;
public:

	RenderManager();
	~RenderManager();

	void setRenderProperties(RenderableObject* renderable);
	void revertRenderProperties(RenderableObject* renderable);
	ShaderParamSetter getShaderParamSetter();

	void loadShader(string name, string fragment_file, string vertex_file);

	void setUniformInt(const char *name, GLint value);
	void setUniformIntArray(const char *name, int values[], int num_values);
	void setUniformVector3(const char *name, Project::Math::Point point);
	void setUniformVector4(const char *name, Project::OpenGL::Color color);
	void setAttributeVector3(const char *name, Project::Math::Point point);
	void setAttributeVector4(const char *name, Project::OpenGL::Color color);

private:

	ShaderParamSetter paramSetter;

	vector<Shader*> shader;
	vector< vector< ShaderAttributeLocation > > shaderAttributeLocation;
	vector<string> shaderName;

	int enabledShaderIndex;

	stack<int> shaderStack;
	stack<Material*> materialStack;
	stack<Texture*> textureStack;

	int numMaterialOverrides;
	int numShaderOverrides;
	int numTextureOverrides;

	bool materialsOverridden();
	bool texturesOverridden();
	bool shadersOverridden();

	void bindTexture(Texture* texture);

	void enableShader(int shader_index);
	void disableShader(int shader_index);
	Shader* getShaderByIndex(int shader_index);
	int getShaderIndexByName(string name);

	void setRenderMaterial(RenderProperties* properties);
	void setRenderShader(RenderProperties* properties);
	void setRenderTexture(RenderProperties* properties);

	void revertRenderMaterial(RenderProperties* properties);
	void revertRenderShader(RenderProperties* properties);
	void revertRenderTexture(RenderProperties* properties);

	int getShaderAttributeLocation(int shader_index, string name);
	int getShaderUniformLocation(int shader_index, const char *name);

	int boolToInt(bool b);

};

}  // namespace Render
}  // namespace Project

#endif
