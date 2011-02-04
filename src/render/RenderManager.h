#ifndef PROJECT_RENDER__RENDER_STATE_MANAGER_H
#define PROJECT_RENDER__RENDER_STATE_MANAGER_H

#include "ShaderParamSetter.h"
#include "Shader.h"
#include "RenderProperties.h"
#include "ShaderAttributeLocation.h"
#include "LightManager.h"
#include "opengl/OpenGL.h"
#include "opengl/Camera.h"
#include "RenderSettings.h"
#include "TextureCube.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

#define SHADER_COLORMAP_UNIFORM_NAME "colorMap"
#define SHADER_NORMALMAP_UNIFORM_NAME "normalMap"
#define SHADER_GLOWMAP_UNIFORM_NAME "glowMap"
#define SHADER_CUBEMAP_UNIFORM_NAME "cubeMap"
#define SHADER_CAMERA_MATRIX_UNIFORM_NAME "cameraMatrix"
#define SHADER_HASTEXTURE_UNIFORM_NAME "hasTexture"
#define SHADER_NUMLIGHTS_UNIFORM_NAME "numLights"
#define SHADER_TANGENT_ATTRIBUTE_NAME "tangent"
#define SHADER_BITANGENT_ATTRIBUTE_NAME "bitangent"
#define RENDERER_NO_SHADER_NAME "none"

namespace Project {
namespace Render {

class RenderableObject;

/** Manages the rendering state when rendering renderable objects
*/
class RenderManager {
private:

	struct ShaderMapping {
		string name;
		string mapped_name;
	};

	static const GLenum colorMapTexture = GL_TEXTURE0;
	static const int colorMapTextureNum = 0;
	static const GLenum normalMapTexture = GL_TEXTURE1;
	static const int normalMapTextureNum = 1;
	static const GLenum glowMapTexture = GL_TEXTURE2;
	static const int glowMapTextureNum = 2;
	static const GLenum cubeMapTexture = GL_TEXTURE3;
	static const int cubeMapTextureNum = 3;
	static const int noShaderIndex = 5000;

	LightManager* lightManager;
	RenderSettings* settings;
	OpenGL::Camera* camera;
	TextureCube* cubeMap;

	bool shadersEnabled;
	bool texturesEnabled;
	bool lodLow;

	ShaderParamSetter paramSetter;

	vector<Shader*> shader;
	vector< vector< ShaderAttributeLocation > > shaderAttributeLocation;
	vector<string> shaderName;
	vector<ShaderMapping> shaderMappings;

	int enabledShaderIndex;

	stack<int> shaderStack;
	stack<Material*> materialStack;
	stack<Texture*> textureStack;
	stack<OpenGL::Color> colorStack;
	vector< vector<ShaderParameter*> > shaderParams;

	int tangentLocation;
	int bitangentLocation;

	int numTransformations;

	int numColorOverrides;
	int numMaterialOverrides;
	int numShaderOverrides;
	int numTextureOverrides;
	
public:

	RenderManager();
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
	void setUniformIntArray(const char *name, const int values[], int num_values);
	void setUniformVector3(const char *name, Math::Point point);
	void setUniformVector4(const char *name, OpenGL::Color color);
	void setUniformMatrix4(const char *name, GLboolean transpose, const GLfloat* matrix);
	void setAttributeVector3(const char *name, Math::Point point);
	void setAttributeVector4(const char *name, OpenGL::Color color);
	void setTangents(Math::Point tangent, Math::Point bitangent);

	void setShadersEnabled(bool enabled) { shadersEnabled = enabled; }
	void setTexturesEnabled(bool enabled) { texturesEnabled = enabled; }
	bool getLODLow() const { return lodLow; }
	void setLODLow(bool low) { lodLow = low; }

	LightManager* getLightManager() const { return lightManager; }

	void setCamera(OpenGL::Camera* _camera) { camera = _camera; }
	bool hasTransformation() const;
	const Math::BoundingObject* getBoundingObject() const;

	void setCubeMap(TextureCube* cube_map);
	

private:

	bool colorsOverridden() { return (numColorOverrides > 0); }
	bool materialsOverridden() { return (numMaterialOverrides > 0); }
	bool texturesOverridden() { return (numTextureOverrides > 0 || !texturesEnabled); }
	bool shadersOverridden() { return (numShaderOverrides > 0|| !shadersEnabled); }

	void enableShader(int shader_index);
	void disableShader(int shader_index);
	Shader* getShaderByIndex(int shader_index);
	int getShaderIndexByName(string name);

	void setRenderColor(RenderProperties* properties);
	void setRenderMaterial(RenderProperties* properties);
	void setRenderShader(RenderProperties* properties);
	void setRenderTexture(RenderProperties* properties);

	void revertRenderColor(RenderProperties* properties);
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
