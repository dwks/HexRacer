#ifndef PROJECT_RENDER__RENDER_PROPERTIES_H
#define PROJECT_RENDER__RENDER_PROPERTIES_H

#include "math/Matrix.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

namespace Project {
namespace Render {

class RenderProperties {
public:

	RenderProperties();

	bool hasTransformation() const;
	void clearTransformation();
	void setTransformation(Project::Math::Matrix _transform);
	Project::Math::Matrix getTransformation() const;

	bool hasMaterial() const;
	void clearMaterial();
	void setMaterial(Material* _material);
	Material* getMaterial() const;
	bool getMaterialOverride() const;
	void setMaterialOverride(bool override_children);

	bool hasTexture() const;
	void clearTexture();
	void setTexture(Texture* _texture);
	Texture* getTexture() const;
	bool getTextureOverride() const;
	void setTextureOverride(bool override_children);

	bool hasShader() const;
	void clearShader();
	void setShaderIndex(int _shader);
	int getShaderIndex() const;
	bool getShaderOverride() const;
	void setShaderOverride(bool override_children);

private:

	bool transformationSet;
	Project::Math::Matrix transformation;
	Material* material;
	bool materialOverrideChildren;
	Texture* texture;
	bool textureOverrideChildren;
	int shader;
	bool shaderOverrideChildren;


};

}  // namespace Render
}  // namespace Project

#endif
