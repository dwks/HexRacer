#ifndef PROJECT_RENDER__RENDER_STATE_MANAGER_H
#define PROJECT_RENDER__RENDER_STATE_MANAGER_H

#include "shader/ShaderManager.h"
#include "shader/ShaderParameter.h"
#include "RenderProperties.h"
//#include "ShaderAttributeLocation.h"
#include "LightManager.h"
#include "opengl/OpenGL.h"
#include "opengl/Camera.h"
#include "RenderSettings.h"
#include "opengl/TextureCube.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

namespace Project {
namespace Render {

class RenderableObject;

/** Manages the rendering state when rendering renderable objects
*/
class RenderManager {

public:

	static const int NUM_STANDARD_TEXTURES = 4;
	int hasTexture [NUM_STANDARD_TEXTURES];

	static const GLenum COLOR_MAP_TEXTURE_UNIT = GL_TEXTURE0;
	static const int COLOR_MAP_TEXTURE_INDEX = 0;
	static const GLenum NORMAL_MAP_TEXTURE_UNIT = GL_TEXTURE1;
	static const int NORMAL_MAP_TEXTURE_INDEX = 1;
	static const GLenum GLOW_MAP_TEXTURE_UNIT = GL_TEXTURE2;
	static const int GLOW_MAP_TEXTURE_INDEX = 2;
	static const GLenum SHADOW_MAP_TEXTURE_UNIT = GL_TEXTURE3;
	static const int SHADOW_MAP_TEXTURE_INDEX = 3;

	static const GLenum CUBE_MAP_TEXTURE_UNIT = GL_TEXTURE4;
	static const int CUBE_MAP_TEXTURE_INDEX = 4;

private:

	struct ShaderMapping {
		string name;
		string mapped_name;
	};

	RenderSettings* settings;
	LightManager* lightManager;
	Shader::ShaderManager* shaderManager;
	
	OpenGL::Camera* camera;
	OpenGL::TextureCube* cubeMap;
	GLuint shadowMap;

	stack<int> shaderStack;
	stack<OpenGL::Material*> materialStack;
	stack<OpenGL::Material*> materialTintStack;
	stack<TexturePack*> textureStack;
	stack<OpenGL::Color> colorStack;
	vector< vector<Shader::ShaderParameter*> > shaderParams;

	bool activeShaderChanged;

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
	void setRenderProperties(RenderProperties* properties);
	/** Revert the render state from the properties of @a renderable
	*/
	void revertRenderProperties(RenderProperties* properties);

	//void loadShadersFile(string filename);
	//void loadShader(string name, string fragment_file, string vertex_file);

	LightManager* getLightManager() const { return lightManager; }
	Shader::ShaderManager* getShaderManager() const { return shaderManager; }

	void setCamera(OpenGL::Camera* _camera) { camera = _camera; }
	bool hasTransformation() const;
	const Math::BoundingObject* getBoundingObject() const;
	void setCubeMap(OpenGL::TextureCube* cube_map);
	void setShadowMapTexture(GLuint shadow_map) { shadowMap = shadow_map; }

	RenderSettings* getRenderSettings() const { return settings; }

	void cleanup() const;

private:

	bool colorsOverridden() { return (numColorOverrides > 0); }
	bool materialsOverridden() { return (numMaterialOverrides > 0); }
	bool texturesOverridden() { return (numTextureOverrides > 0); }
	bool shadersOverridden() { return (numShaderOverrides > 0); }

	void setRenderColor(RenderProperties* properties);
	void setRenderMaterial(RenderProperties* properties);
	void setRenderShader(RenderProperties* properties);
	void setRenderTexture(RenderProperties* properties);

	void revertRenderColor(RenderProperties* properties);
	void revertRenderMaterial(RenderProperties* properties);
	void revertRenderShader(RenderProperties* properties);
	void revertRenderTexture(RenderProperties* properties);

	void setShaderParameters();

};

}  // namespace Render
}  // namespace Project

#endif
