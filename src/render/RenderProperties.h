#ifndef PROJECT_RENDER__RENDER_PROPERTIES_H
#define PROJECT_RENDER__RENDER_PROPERTIES_H

#include "math/Matrix.h"
#include "opengl/Material.h"
#include "TexturePack.h"
#include "Shader.h"
#include "ShaderParameter.h"
#include <string>
#include <vector>

namespace Project {
namespace Render {

/** Properties for rendering an object
*/
class RenderProperties {

private:

	bool transformationSet;
	Project::Math::Matrix transformation;

	OpenGL::Material* material;
	bool materialOverrideChildren;

	TexturePack* texture;
	bool textureOverrideChildren;

	int shader;
	bool shaderOverrideChildren;

	bool colorSet;
	Project::OpenGL::Color color;
	bool colorOverrideChildren;

	bool empty;

	std::string wantsShaderName;
	std::vector<ShaderParameter*> shaderParams;

public:

	RenderProperties();

	bool hasTransformation() const { return transformationSet; }
	void clearTransformation() { transformationSet = false; }
	void setTransformation(Project::Math::Matrix _transform) {
		transformation = _transform;
		transformationSet = true;
		empty = false;
	}
	const Project::Math::Matrix& getTransformation() const { return transformation; }

	bool hasColor() const { return colorSet; }
	void clearColor() { colorSet = false; }
	void setColor(Project::OpenGL::Color _color) {
		color = _color;
		colorSet = true;
		empty = false;
	}
	const Project::OpenGL::Color& getColor() const { return color; }
	bool getColorOverride() const { return colorOverrideChildren; }
	void setColorOverride(bool override_children) { colorOverrideChildren = override_children; }

	bool hasMaterial() const { return (material != NULL); }
	void clearMaterial() { material = NULL; }
	void setMaterial(OpenGL::Material* _material) {
		material = _material;
		empty = false;
	}
	OpenGL::Material* getMaterial() const { return material; }
	bool getMaterialOverride() const { return materialOverrideChildren; }
	void setMaterialOverride(bool override_children) { materialOverrideChildren = override_children; }

	bool hasTexturePack() const { return (texture != NULL); }
	void clearTexturePack() { texture = NULL; }
	void setTexturePack(TexturePack* _texture) { texture = _texture; }
	TexturePack* getTexturePack() const { return texture; }
	bool getTexturePackOverride() const { return textureOverrideChildren; }
	void setTexturePackOverride(bool override_children) {
		textureOverrideChildren = override_children;
		empty = false;
	}

	bool hasShader() const { return (shader >= 0); }
	void clearShader() { shader = -1; }
	void setShaderIndex(int _shader) { shader = _shader; }
	int getShaderIndex() const { return shader; }
	bool getShaderOverride() const { return shaderOverrideChildren; }
	void setShaderOverride(bool override_children) {
		shaderOverrideChildren = override_children;
		empty = false;
	}

	bool wantsShader() const;
	std::string getWantsShaderName();
	void setWantsShaderName(std::string shader_name);

	bool hasShaderParams() const;
	std::vector<ShaderParameter*>& getShaderParams();
	void setShaderParams(std::vector<ShaderParameter*> params);
	void addShaderParameter(ShaderParameter* param);
	void clearShaderParams() { shaderParams.clear(); }

	bool isEmpty() const { return empty; }

};

}  // namespace Render
}  // namespace Project

#endif
