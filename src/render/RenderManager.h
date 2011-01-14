#ifndef PROJECT_RENDER__RENDER_STATE_MANAGER_H
#define PROJECT_RENDER__RENDER_STATE_MANAGER_H

#include "ShaderParamSetter.h"
#include "Shader.h"
#include "RenderProperties.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

namespace Project {
namespace Render {

class RenderableObject;

class RenderManager {
public:

	RenderManager();
	~RenderManager();

	void setRenderProperties(const RenderableObject* renderable);
	void revertRenderProperties(const RenderableObject* renderable);
	ShaderParamSetter getShaderParamSetter();

	void loadShader(string name, string vertex_file, string fragment_file);

	void setUniformVector3(const char *name, Project::Math::Point point);
	void setUniformVector4(const char *name, Project::OpenGL::Color color);
	void setAttributeVector3(const char *name, Project::Math::Point point);
	void setAttributeVector4(const char *name, Project::OpenGL::Color color);

private:

	ShaderParamSetter paramSetter;

	vector<Shader*> shader;
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

};

}  // namespace Render
}  // namespace Project

#endif
