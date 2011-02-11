#ifndef PROJECT_RENDER__RENDER_PROPERTIES_H
#define PROJECT_RENDER__RENDER_PROPERTIES_H

#include "math/Matrix.h"
#include "opengl/Material.h"
#include "TexturePack.h"
#include "shader/ShaderProgram.h"
#include "shader/ShaderParameter.h"
#include <string>
#include <vector>

namespace Project {
namespace Render {

/** Properties for rendering an object
*/
class RenderProperties {

private:

	//bool transformationSet;
	Project::Math::Matrix* transformation;

	OpenGL::Material* material;
	bool materialOverrideChildren;

	OpenGL::Material* materialTint;

	TexturePack* texture;
	bool textureOverrideChildren;

	int shader;
	bool shaderOverrideChildren;

	//bool colorSet;
	Project::OpenGL::Color* color;
	bool colorOverrideChildren;

	std::string wantsShaderName;
	std::vector<Shader::ShaderParameter*> shaderParams;

	bool empty;

	void changed();

public:

	RenderProperties();
	~RenderProperties();

	bool hasTransformation() const { return (transformation != NULL); }
	void clearTransformation();
	void setTransformation(const Math::Matrix& _transform);
	const Project::Math::Matrix& getTransformation() const { return *transformation; }

	bool hasColor() const { return (color != NULL); }
	void clearColor();
	void setColor(const OpenGL::Color& _color);
	const OpenGL::Color& getColor() const { return *color; }
	bool getColorOverride() const { return colorOverrideChildren; }
	void setColorOverride(bool override_children) { colorOverrideChildren = override_children; }

	bool hasMaterial() const { return (material != NULL); }
	void clearMaterial() { material = NULL; changed(); }
	void setMaterial(OpenGL::Material* _material) {
		material = _material;
		changed();
	}
	OpenGL::Material* getMaterial() const { return material; }
	bool getMaterialOverride() const { return materialOverrideChildren; }
	void setMaterialOverride(bool override_children) { materialOverrideChildren = override_children; }

	bool hasMaterialTint() const { return (materialTint != NULL); }
	void clearMaterialTint() { materialTint = NULL; changed(); }
	void setMaterialTint(OpenGL::Material* material_tint) {
		materialTint = material_tint;
		changed();
	}
	OpenGL::Material* getMaterialTint() const { return materialTint; }

	bool hasTexturePack() const { return (texture != NULL); }
	void clearTexturePack() { texture = NULL; changed(); }
	void setTexturePack(TexturePack* _texture) { texture = _texture; }
	TexturePack* getTexturePack() const { return texture; }
	bool getTexturePackOverride() const { return textureOverrideChildren; }
	void setTexturePackOverride(bool override_children) {
		textureOverrideChildren = override_children;
		changed();
	}

	bool hasShaderIndex() const { return (shader >= 0); }
	void clearShaderIndex() { shader = -1; changed(); }
	void setShaderIndex(int _shader) { shader = _shader; }
	int getShaderIndex() const { return shader; }
	bool getShaderOverride() const { return shaderOverrideChildren; }
	void setShaderOverride(bool override_children) {
		shaderOverrideChildren = override_children;
		changed();
	}

	bool wantsShader() const;
	std::string getWantsShaderName();
	void setWantsShaderName(std::string shader_name);

	bool hasShaderParams() const;
	std::vector<Shader::ShaderParameter*>& getShaderParams();
	void setShaderParams(std::vector<Shader::ShaderParameter*> params);
	void addShaderParameter(Shader::ShaderParameter* param);
	void clearShaderParams() { shaderParams.clear(); changed(); }

	bool isEmpty() const { return empty; }

};

}  // namespace Render
}  // namespace Project

#endif
