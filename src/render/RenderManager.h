#ifndef PROJECT_RENDER__RENDER_STATE_MANAGER_H
#define PROJECT_RENDER__RENDER_STATE_MANAGER_H

#include "ShaderParamSetter.h"
#include "Shader.h"
#include "RenderProperties.h"
#include "ShaderAttributeLocation.h"
#include "LightManager.h"
#include "opengl/OpenGL.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

#define SHADER_COLORMAP_UNIFORM_NAME "colorMap"
#define SHADER_NORMALMAP_UNIFORM_NAME "normalMap"
#define SHADER_HASTEXTURE_UNIFORM_NAME "hasTexture"
#define SHADER_NUMLIGHTS_UNIFORM_NAME "numLights"
#define RENDERER_NO_SHADER_NAME "none"

namespace Project {
namespace Render {

class RenderableObject;

/** Manages the rendering state when rendering renderable objects
*/
class RenderManager {
private:
	static const GLenum colorMapTexture = GL_TEXTURE0;
	static const int colorMapTextureNum = 0;
	static const GLenum normalMapTexture = GL_TEXTURE1;
	static const int normalMapTextureNum = 1;
	static const short GRAPHICS_HIGH = 2;
	static const short GRAPHICS_MED = 1;
	static const short GRAPHICS_LOW = 0;
	static const int noShaderIndex = 5000;
	LightManager* lightManager;
	short graphicsQuality;
public:

	RenderManager(const char* config_file_name = "");
	~RenderManager();

	/** Set the render state to the properties of @a renderable
	*/
	void setRenderProperties(RenderableObject* renderable);
	/** Revert the render state from the properties of @a renderable
	*/
	void revertRenderProperties(RenderableObject* renderable);
	ShaderParamSetter getShaderParamSetter();

	void loadShadersFile(string filename);
	void loadShader(string name, string fragment_file, string vertex_file);

	void setUniformInt(const char *name, GLint value);
	void setUniformIntArray(const char *name, int values[], int num_values);
	void setUniformVector3(const char *name, Project::Math::Point point);
	void setUniformVector4(const char *name, Project::OpenGL::Color color);
	void setAttributeVector3(const char *name, Project::Math::Point point);
	void setAttributeVector4(const char *name, Project::OpenGL::Color color);

	LightManager* getLightManager() const { return lightManager; }

private:

	ShaderParamSetter paramSetter;

	vector<Shader*> shader;
	vector< vector< ShaderAttributeLocation > > shaderAttributeLocation;
	vector<string> shaderName;

	int enabledShaderIndex;

	stack<int> shaderStack;
	stack<Material*> materialStack;
	stack<Texture*> textureStack;
	vector< vector<ShaderParameter*> > shaderParams;

	int numMaterialOverrides;
	int numShaderOverrides;
	int numTextureOverrides;

	bool materialsOverridden();
	bool texturesOverridden();
	bool shadersOverridden();

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

	void setShaderParameters();

};

}  // namespace Render
}  // namespace Project

#endif
